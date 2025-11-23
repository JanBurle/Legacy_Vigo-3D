// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

// eof
