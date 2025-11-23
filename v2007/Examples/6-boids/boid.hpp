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

#ifndef BOIDS_BOID_HPP_INCLUDED
#define BOIDS_BOID_HPP_INCLUDED

  #include <vigosim>
  #include "scene.hpp"

//-----------------------------------------------------------------------------

  typedef vigo::space::Node<float,
          vigo::parts::Parts<
          vigo::parts::PA<float>,
          vigo::parts::RR,
          vigo::parts::TC<float>,
          vigo::parts::SS,
          vigo::parts::A1
        >
      > BoidNode;

//-----------------------------------------------------------------------------

  class Boid: public BoidNode
  {
    SUPER(BoidNode)
    
  private:
    // orientation
    float  m_yaw, m_pitch, m_roll;
    Vector m_heading;

    // steering
    float m_desiredRoll,  m_maxRoll,  m_rollD;  // D=delta
    float m_desiredPitch, m_maxPitch, m_pitchD;

    Point m_goal;
    bool  m_goalSet;

    // forces
    float  m_thrustF, m_dragF, m_liftF;
    Vector m_liftV;

    // stored neighbourhood size
    uint   m_nbrSize;

    // urges
    Vector m_Us, m_Ua, m_Uc, m_U; // separation. alignment, cohesion, aggregate
    // vision
    Vector m_V;

  public:
    Boid(Scene&);
   ~Boid();

    GETTER(GetScene, Scene&, (Scene&)super::GetScene())

    GETTER(Heading, Vector, m_heading)
    GETTER(Up,      Vector, m_liftV)

    void PerceiveAndSteer();
    void CalculatePhysics();

    bool Act();

    void SteerRight(float);
    void SteerUp(float);
    void SteerToGoal(Point const& goal);
    void DoNotSteer();
  };


//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
