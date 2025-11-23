/*****************************************************************************\
*    Copyright (c) 2004--2007 by Ian G Burleigh                               *
*    https://github.com/phi53/Vigo-3D                                         *
*                                                                             *
*    This program is free software; you can redistribute it and/or modify     *
*    it under the terms of the GNU General Public License as published by     *
*    the Free Software Foundation; either version 2 of the License, or        *
*    (at your option) any later version.                                      *
*                                                                             *
*    This program is distributed in the hope that it will be useful,          *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*    GNU General Public License for more details.                             *
*                                                                             *
*    You should have received a copy of the GNU General Public License        *
*    along with this program; if not, write to the                            *
*    Free Software Foundation, Inc.,                                          *
*    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                *
*-----------------------------------------------------------------------------*
* $Id: main.cpp 46 2006-06-21 10:33:26Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "boid.hpp"
  #include "object.hpp"
  #include "params.hpp"

//-----------------------------------------------------------------------------

  #define PARB(par) params::BOID::par
  Boid::Boid(Scene& s)
    : super(s,true),
      m_yaw(0), m_pitch(0), m_roll(0), m_heading(),
      m_desiredRoll(0),  m_maxRoll(PARB(maxRoll)),   m_rollD(PARB(rollD)),
      m_desiredPitch(0), m_maxPitch(PARB(maxPitch)), m_pitchD(PARB(pitchD)),
      m_goal(), m_goalSet(false),
      m_thrustF(PARB(thrust)), m_dragF(0), m_liftF(0), m_liftV(),
      m_nbrSize(0),
      m_Us(), m_Ua(), m_Uc(), m_U(),
      m_V()
  {
    SetVel(Vector(Point::Random()).Unit()/1000.); // to force random heading
    CalculatePhysics(); // to calculate the rest

    SetShape(*GetScene().boidShape);
    SetAppearance(*GetScene().boidAppearance);
  }

  Boid::~Boid()
  {
  }


  static float calcStep(float desired, float actual, float delta)
  {
    assert(delta>=0.0);
    float d = vigo::min(vigo::abs(desired-actual),delta);
    // a bit of randomness, need to break ties
    float range2 = delta/10.0;
    float r = vigo::math::rand.Double(-range2,+range2);
    return ((desired-actual>0) ? d : -d) + r;
  }

  void Boid::PerceiveAndSteer()
  {
    Scene &s = GetScene();
  
    m_U = Vector::_0;
    m_V = m_heading.Unit() * s.par_boid.vision;

  // first, check collisions
    {
      Scene::polygon_objects const &o = GetScene().GetObjects();

      for(Scene::polygon_objects::const_iterator it=o.begin(), itEnd=o.end();
          it!=itEnd; ++it)
      {
        Point Ri;
        if((*it)->IntersectByRay(Pos(),m_V,Ri))
        {
          // evade: find which way (left/right)
          uint const STEPS=8; bool found = false;
          for(uint i=1; !found && i<=STEPS; ++i)
          {
            float angle = ((float)vigo::math::pi2/(float)STEPS)*(float)i;
            if(!(*it)->IntersectByRay(Pos(),m_V.Rotate(Rotation(0,angle,0)),Ri))
            {
              SteerRight(-1);
              found = true;
            }
            else
            if(!(*it)->IntersectByRay(Pos(),m_V.Rotate(Rotation(0,-angle,0)),Ri))
            {
              SteerRight(+1);
              found = true;
            }
            else
            if(!(*it)->IntersectByRay(Pos(),m_V.Rotate(Rotation(angle,0,0)),Ri))
            {
              SteerUp(+1);
              found = true;
            }
            else
            if(!(*it)->IntersectByRay(Pos(),m_V.Rotate(Rotation(-angle,0,0)),Ri))
            {
              SteerUp(-1);
              found = true;
            }
          }
    
          if(!found)
            SteerRight(vigo::math::rand.Bool() ? +1 : -1); // do whatever
    
          m_roll  = m_desiredRoll;  // fast
          m_pitch = m_desiredPitch; // fast
          return;
        }
      }
    }

    // next, flocking behaviour
    if(s.par_flocking.flock)
    {
    // get info about others
      vigo::space::tracked_collection<float> nbrs;
      Nbr(GrpId(),s.par_boid.perc,nbrs);
      m_nbrSize = nbrs.size();
    // compute urges
      m_Us = m_Ua = m_Uc = Vector::_0;
      for(uint i=0; i<m_nbrSize; ++i)
      {
        Boid const *nbr = dynamic_cast<Boid*>(nbrs[i]);
        if(nbr)
        {
          Vector dist = nbr->Pos() - Pos();
          // a hack to avoid division by 0
          if(dist.Length() < vigo::algebra3d::EPS)
            dist = m_heading*vigo::algebra3d::EPS;
    
          // sensitivity function
          Vector d = dist;
          Vector h = m_heading;
          float  a = d.Unit()*h.Unit() + 1;                   // the angle part
          float  p = vigo::max(1.0-dist.Length()/s.par_boid.perc,0.0); // the distance part
          float  sens = a*p;
    
          m_Us -= sens * d/d.Magnitude();
          m_Ua += sens * nbr->m_heading;
          m_Uc += sens * d;
        }
      }
    
      m_U += s.par_flocking.Cs*m_Us
           + s.par_flocking.Ca*m_Ua 
           + s.par_flocking.Cc*m_Uc;
    }
    else // do not flock
    {
      m_nbrSize = 0;
    }

    if(m_goalSet)
    {
      Vector toGoal = (m_goal - Pos()).Unit();
      m_U += s.par_flocking.Cg*toGoal*pow(toGoal.Length(),0.4)/100.0;
    }

    // steer
    {
      Vector Uxz(m_U.x,0,m_U.z);
      Vector Hxz(m_heading.x,0,m_heading.z);
      double angle = Uxz.Angle(Hxz);
      SteerRight(vigo::sgn((Uxz^Hxz).y)*angle);

      angle = m_U.Angle(Uxz);
      if(m_U.y>0)
        SteerUp(angle);
      else
        SteerUp(-angle*2.0); // need to oversteer down, otherwise they keep rising
    }

    m_roll  += calcStep(m_desiredRoll, m_roll, m_rollD);
    m_pitch += calcStep(m_desiredPitch,m_pitch,m_pitchD);
  }


  void Boid::CalculatePhysics()
  {
    Scene &s = GetScene();
    
    Vector vel = Vel();
    // orientation
    Vector footprint(vel.x,0,vel.z);
    m_yaw = footprint.Angle(Vector::Z);
    if(footprint.x<0) m_yaw = -m_yaw;

    // this is the aggregate rotation
    Rotation r = Rotation(0,m_yaw,0)
               + Rotation(-/*>>>why?*/m_pitch,0,0)
               + Rotation(0,0,m_roll);

    m_heading = Vector::Z.Rotate(r);  // unit vector

    Vector sideways = Vector::X.Rotate(r);
    m_liftV = (vel^sideways).Unit();

  // physics
    float SA = vigo::max(s.par_boid.A * (float)sin(vel.Angle(m_heading)),s.par_boid.S); // effective wing area
    float v2 = vel.Magnitude();
    m_dragF  = s.par_physics.Cd * s.par_physics.rho2 * v2 * SA;
    m_liftF  = s.par_physics.Cl * s.par_physics.rho2 * v2 * s.par_boid.A;

    Vector accFwd (m_heading * m_thrustF / s.par_boid.mass),
           accDrag(-vel.Unit() * m_dragF / s.par_boid.mass),
           accGrav(-Vector::Y * s.par_physics.gee),
           accLift(m_liftV * m_liftF / s.par_boid.mass);

    SetAcc(accFwd + accLift + accDrag + accGrav);
    SetRot(r);
  }


  bool Boid::Act()
  {
    switch(ActPhase())
    {
    case 0:
      PerceiveAndSteer();
      return false;
    case 1:
      CalculatePhysics();
      super::Act(); // move
      break;
    }

    return true;
  }


  void Boid::SteerRight(float r)
  {
    m_desiredRoll = r * m_maxRoll;
  }


  void Boid::SteerUp(float r)
  {
    m_desiredPitch = r * m_maxPitch;
  }


  void Boid::SteerToGoal(Point const& goal)
  {
    m_goal    = goal;
    m_goalSet = true;
  }


  void Boid::DoNotSteer()
  {
    m_goalSet      = false;
    m_desiredRoll  = m_roll;
    m_desiredPitch = m_pitch;
  }


//** eof **********************************************************************
