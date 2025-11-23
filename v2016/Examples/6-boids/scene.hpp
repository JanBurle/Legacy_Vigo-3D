// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef BOIDS_SCENE_HPP_INCLUDED
#define BOIDS_SCENE_HPP_INCLUDED

  #include <vigosim>
  typedef vigo::algebra3d::Pointf   Point;
  typedef vigo::algebra3d::Pointd   Pointd;
  typedef vigo::algebra3d::Vectorf  Vector;
  typedef vigo::algebra3d::Vectord  Vectord;
  using vigo::algebra3d::Rotation;
  using vigo::shape::ColorAlpha;


  class PolygonObject;

//-----------------------------------------------------------------------------

  class Scene: public vigo::space::Scenef
  {
    SUPER(vigo::space::Scenef)

  public:
    // camera
    struct
    {
      double      far;
      Pointd      eye;
      Pointd      center;
      Vectord     up;
      ColorAlpha  lightAmbColor;
      ColorAlpha  lightDiffColor;
      Pointd      lightPosition;
    } par_camera;

    // physics
    struct
    {
      float Cd,     // drag coefficient
            Cl,     // lift coefficient
            rho2,   // fluid (air) density/2
            gee;    // g
    } par_physics;

    // flocking coefficients
    struct
    {
      bool  flock;  // flocking behaviour on/off
      float Cs,     // separation
            Ca,     // alignment
            Cc,     // cohesion
            Cg;     // global goal urge
    } par_flocking;

    // boid attributes
    struct
    {
      float size,
            mass,
            A,      // wing area
            S,      // cross-section
            perc,   // perception radius
            vision; // forward vision
    } par_boid;

    // boid shape and appearance
    vigo::util::refptr<vigo::shape::Polygon>    boidShape;
    vigo::util::refptr<vigo::shape::Appearance> boidAppearance;

  public:
    Scene();

    double  CameraFar()     const { return par_camera.far;    }
    Pointd  CameraEye()     const { return par_camera.eye;    }
    Pointd  CameraCenter()  const { return par_camera.center; }
    Vectord CameraUp()      const { return par_camera.up;     }

    std::string StatusText() const;

    // objects
    typedef vigo::util::refptr<PolygonObject> rPolygonObject;
    typedef std::vector<rPolygonObject>       polygon_objects;
    GETTER(GetObjects, polygon_objects const&, m_objects)

  private:
    polygon_objects m_objects;

    void Create();
    void LoadObjects();
    void AddBoids();
  };

//-----------------------------------------------------------------------------

#endif

// eof
