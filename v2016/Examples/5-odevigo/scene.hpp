// Copyright (c) 2006 by Marcin Pilat http://pilat.org

/** \file
 */

#ifndef ODEVIGO_SCENE_HPP_INCLUDED
#define ODEVIGO_SCENE_HPP_INCLUDED

  #define VIGO_ODE_LINK
  #include <ode/ode.h>
  #include <vigosim>

  typedef vigo::algebra3d::Pointd  Point;
  typedef vigo::algebra3d::Vectord Vector;

//-----------------------------------------------------------------------------

  double const SCENE_RADIUS = 300.0;

  class Scene: public vigo::space::Scened
  {
    SUPER(vigo::space::Scened)

  public:
    Scene();
   ~Scene();

    double CameraFar()  const { return 4*SCENE_RADIUS;          }
    Point  CameraEye()  const { return Point(SCENE_RADIUS,0,0); }
    Vector CameraUp()   const { return Vector::Z;               }

    bool Act();

    // The status line text
    std::string StatusText() const;

    // Access to ODE ids
    GETTER(World,       dWorldID,     m_world)
    GETTER(Space,       dSpaceID,     m_space)
    GETTER(ContactGroup,dJointGroupID,m_contactgroup)

  private:
    void Populate();

    // ODE-detected contacts
    static void nearCallback(pvoid This, dGeomID, dGeomID);
    void nearCallback(dGeomID, dGeomID);

    // ODE ids
    dWorldID      m_world;
    dSpaceID      m_space;
    dJointGroupID m_contactgroup;
  };

//-----------------------------------------------------------------------------

#endif

// eof
