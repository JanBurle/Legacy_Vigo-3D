// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef VIGOPROJECT_AGENT_HPP_INCLUDED
#define VIGOPROJECT_AGENT_HPP_INCLUDED

  #include <vigosim>
  #include "scene.hpp"

  using namespace vigo;

//-----------------------------------------------------------------------------

  typedef space::Node<double,
          parts::Parts<
	      parts::PP<double>,
	      parts::R0,
	      parts::T0<double>,
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
    Agent(Scene& s)
      : super(s,true)
    {
      SetShape(*new shape::Sphere(1.0));
      SetAppearance(*new shape::Appearance(0,1,0));
    }

   ~Agent()
    {
    }

    bool Act()
    {
      switch(ActPhase())
      {
      case 0: // move
        MoveBy(Vector(Point::Random(Point::_0,MAX_STEP_SIZE)));
        break;
      }

      return true;
    }
  };


//-----------------------------------------------------------------------------

#endif

// eof
