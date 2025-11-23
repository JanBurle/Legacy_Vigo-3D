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

//** eof **********************************************************************
