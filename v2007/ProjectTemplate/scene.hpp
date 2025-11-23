/*****************************************************************************\
*    Copyright (c) 2006 by ...........                 .....@cpsc.ucalgary.ca *                                                                       *
*-----------------------------------------------------------------------------*
* $Id: $
\*****************************************************************************/

/** \file
 */

#ifndef VIGOPROJECT_SCENE_HPP_INCLUDED
#define VIGOPROJECT_SCENE_HPP_INCLUDED

  #include <vigosim>
  typedef vigo::algebra3d::Pointd  Point;
  typedef vigo::algebra3d::Vectord Vector;

//-----------------------------------------------------------------------------
    
  double const SCENE_RADIUS = 100.0;
  
  class Scene: public vigo::space::Scened
  {
    SUPER(vigo::space::Scened)

  public:
    Scene();

    double CameraFar()  const { return 4*SCENE_RADIUS;          }
    Point  CameraEye()  const { return Point(0,0,SCENE_RADIUS); }

    std::string StatusText() const;

  private:
    void Create();
  };

//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
