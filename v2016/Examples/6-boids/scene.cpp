// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "scene.hpp"
  #include "boid.hpp"
  #include "params.hpp"
  #include "object.hpp"
  #include <iostream>

//-----------------------------------------------------------------------------

  Scene::Scene()
    : boidShape(new vigo::shape::Polygon),
      boidAppearance(new vigo::shape::Appearance),
      m_objects()
  {
    #define PAR(par) par_camera.par = params::CAMERA::par;
    PAR(far)
    #undef PAR
    par_camera.eye            = Pointd(params::CAMERA::eye);
    par_camera.center         = Pointd(params::CAMERA::center);
    par_camera.up             = Vectord(params::CAMERA::up);
    par_camera.lightAmbColor  = ColorAlpha(vigo::util::double3d(params::CAMERA::lightAmbColor));
    par_camera.lightDiffColor = ColorAlpha(vigo::util::double3d(params::CAMERA::lightDiffColor));
    par_camera.lightPosition  = Pointd(params::CAMERA::lightPosition);

    #define PAR(par) par_physics.par = params::PHYSICS::par;
    PAR(Cd)
    PAR(Cl)
    PAR(rho2)
    PAR(gee)
    #undef PAR

    #define PAR(par) par_flocking.par = params::FLOCKING::par;
    PAR(flock)
    PAR(Cs)
    PAR(Ca)
    PAR(Cc)
    PAR(Cg)
    #undef PAR

    #define PAR(par) par_boid.par = params::BOID::par;
    PAR(size)
    PAR(mass)
    par_boid.A = params::BOID::rA * vigo::sqr(par_boid.size);
    par_boid.S = params::BOID::rS * vigo::sqr(par_boid.size);
    PAR(perc)
    PAR(vision)
    #undef PAR

    // this creates the "paper airplane" look
    float const UD = par_boid.size/24; // up-down
    float const LR = par_boid.size/6;  // left-right
    float const FB = par_boid.size/2;  // front-back

    boidShape->Push(Point(0,0,FB));
    boidShape->Push(Point(LR,UD,-FB));
    boidShape->Push(Point(0,-UD,-FB));
    boidShape->Push(Point(-LR,UD,-FB));

    vigo::util::tuple3<double> color(params::BOID::color);
    boidAppearance->SetColorAlpha(new vigo::shape::ShapeColorAlpha(color.x,color.y,color.z));

    SetNumActPhases(2);
    Create();
  }


  std::string Scene::StatusText() const
  {
    return super::StatusText();
  }


  void Scene::Create()
  {
    LoadObjects();
    AddBoids();
  }

  void Scene::LoadObjects()
  {
    vigo::extras::Config::cfg_items items = params::config.GetGroup("OBJECTS");

    for(vigo::extras::Config::cfg_items::iterator it=items.begin(), itEnd=items.end();
        it!=itEnd; it++)
    {
      std::string const /*&tag  = it->first,*/
                        &data = it->second;
      PolygonObject &o = PolygonObject::FromString(*this,data);
      m_objects.push_back(rPolygonObject(&o));
      Root().Add(o);
    }
  }

  void Scene::AddBoids()
  {
    Point source(params::APP::boidSource);
    float spread(params::APP::boidSpread);

    for(uint i=0; i<params::APP::numBoids; ++i)
    {
      Boid &b = *new Boid(*this);
      b.SetPos(Point::Random(source,spread));
      Root().Add(b);

      if(params::config.Exists("flight","goal"))
      {
        Point goal(params::config.GetString("flight","goal",""));
        b.SteerToGoal(goal);
      }
    }
  }

// eof
