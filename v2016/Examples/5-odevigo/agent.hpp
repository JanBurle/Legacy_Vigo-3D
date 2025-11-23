// Copyright (c) 2006 by Marcin Pilat http://pilat.org

/** \file
 */

#ifndef ODEVIGO_TAGENT_HPP_INCLUDED
#define ODEVIGO_TAGENT_HPP_INCLUDED

  #include <vigosim>
  #include "scene.hpp"

  using namespace vigo;

//-----------------------------------------------------------------------------

  typedef space::Node<double,
          parts::Parts<
          parts::PP<double>,
          parts::R0,
          parts::TC<double>,
          parts::SS,
          parts::A1
        >
      > AgentNode;

//-----------------------------------------------------------------------------

  double const MAX_STEP_SIZE = 1.0;

  class Agent: public AgentNode
  {
    SUPER(AgentNode)

  public:
    Agent(Scene& s);
    Agent(Scene& s, Point const& pos);
   ~Agent();

    bool Act();

  private:
    // ODE ids
    dBodyID m_body;
    dGeomID m_geom;

    void  Init();
    void  InitODE();

    // ODE geometry position
    Point GetGeomPos() const;
    void  SetGeomPos(Point const& pos);
  };


//-----------------------------------------------------------------------------

#endif

// eof
