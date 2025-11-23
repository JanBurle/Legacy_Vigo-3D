/*****************************************************************************\
*    Copyright (c) 2006 by Marcin Pilat                      http://pilat.org *                                                                       *
*-----------------------------------------------------------------------------*
* $Id: $
\*****************************************************************************/

/** \file
 */

  #include "agent.hpp"

//-----------------------------------------------------------------------------

  double const AGENT_RADIUS  = 1.0;
  double const VISION_RADIUS = 10.0;
  
  Agent::Agent(Scene& s)
    : super(s,true), m_body(0), m_geom(0)
  {
    Init();
  }

  Agent::Agent(Scene& s, Point const& pos)
    : super(s,true), m_body(0), m_geom(0)
  {
    SetPos(pos);
    Init();
  }

  Agent::~Agent()
  {
  }

  void Agent::Init()
  {
    InitODE(); 

    SetShape(*new shape::Sphere(AGENT_RADIUS));
    SetAppearance(*new shape::Appearance(0,1,0));
  }

  void Agent::InitODE()
  {  
	  Scene &s = (Scene&)GetScene();

    m_body = dBodyCreate(s.World());

    dMatrix3 R;
    dRFromAxisAndAngle(R,0.0,0.0,0.0,vigo::math::rand.Double(0,vigo::math::pi));
    dBodySetRotation(m_body,R);

    Point pos = Pos();
    dBodySetPosition(m_body, pos.x, pos.y, pos.z);

    dMass m;
    dMassSetSphere(&m, 1.0, AGENT_RADIUS);
    dBodySetMass(m_body, &m);

    m_geom = dCreateSphere(s.Space(), AGENT_RADIUS);
    dGeomSetBody(m_geom, m_body);
  }

  bool Agent::Act()
  {
	  // flocking behaviour
    switch(ActPhase())
    {
    case 0:
      // look for neighbours
      space::tracked_collection<double> nearMe;
      Nbr(VISION_RADIUS,nearMe,100/*maximum observed*/,true);

      // test cohesion
      uint ns = nearMe.size(); // how many do I see
      if(ns > 0)               // any?
      {
        Vector coh;
        for(uint i=0; i<ns; ++i)
          coh += nearMe[i]->Pos();
  
        coh /= ns;
        coh -= Pos();
        coh = coh.Unit();
                
        dBodyAddForce(m_body, coh.x/2, coh.y/2, coh.z/2);
      }
      // end test         
       
      // synchronize Vigo position with the one calculated by ODE                     
      SetPos(GetGeomPos());
                
      // add random force
      Vector rnd = Point::Random(Point::_0,3.0);
      dBodyAddForce(m_body, rnd.x/2, rnd.y/2, rnd.z/2);
      break;
    }

    return true;
  }


  Point Agent::GetGeomPos() const
  {
    dReal const* r = dGeomGetPosition(m_geom);
    return Point(r[0],r[1],r[2]);
  }

  void Agent::SetGeomPos(Point const& pos)
  {
     dGeomSetPosition(m_geom,pos.x,pos.y,pos.z);
  }

//** eof **********************************************************************
