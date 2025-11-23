// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef QT_GUI_SIM_HPP_INCLUDED
#define QT_GUI_SIM_HPP_INCLUDED

  #include <vigosim>
  #include <iostream>

  typedef vigo::algebra3d::Pointd  Point;
  typedef vigo::algebra3d::Vectord Vector;

  class Scene;

//-----------------------------------------------------------------------------

  double const SCENE_RADIUS   = 8.0;
  double const WALKER_SIZE    = 0.01;
  double const MAX_STEP_SIZE  = 0.002;
  uint   const NUM_WALKERS    = 40;

//-----------------------------------------------------------------------------

  class Scene: public vigo::space::Scened
  {
    SUPER(vigo::space::Scened)

  public:
    Scene();
    double CameraFar()  const { return SCENE_RADIUS*2;          }
    Point  CameraEye()  const { return Point(0,0,SCENE_RADIUS); }

    void Create();
  };

//-----------------------------------------------------------------------------

  class Simulation: public vigo::Simulation
  {
    SUPER(vigo::Simulation)

  public:
    Simulation(): super()
    {
    }
  };

//-----------------------------------------------------------------------------
// a walker

  typedef vigo::space::Node<double,
            vigo::parts::Parts<
          vigo::parts::PP<double>,
          vigo::parts::R0,
          vigo::parts::TC<double>,
          vigo::parts::SS,
          vigo::parts::A1
        >
      > SimpleAgent;

  class Walker: public SimpleAgent
  {
    SUPER(SimpleAgent)

  public:
    Walker(Scene& s) : super(s,true)
    {
    }

	bool Act();
  };

#endif

// eof
