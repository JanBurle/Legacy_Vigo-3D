// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef TONNETZ_SCENE_HPP_INCLUDED
#define TONNETZ_SCENE_HPP_INCLUDED

  #include <vigosim>
  using vigo::algebra3d::Pointf;
  using vigo::algebra3d::Pointd;
  using vigo::algebra3d::Vectord;

//-----------------------------------------------------------------------------

  double const SCENE_RADIUS = 100.0;

  class Scene: public vigo::space::Scenef
  {
    SUPER(vigo::space::Scenef)

  public:
    Scene();

    double CameraFar()  const { return 4*SCENE_RADIUS;           }
    Pointd CameraEye()  const { return Pointd(0,0,SCENE_RADIUS); }

    std::string StatusText() const;
  };

//-----------------------------------------------------------------------------

#endif

// eof
