// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "sim.hpp"

//-----------------------------------------------------------------------------

  Scene::Scene()
  {
    Create();
  }

  void Scene::Create()
  {
    vigo::shape::Shape *sh = new vigo::shape::Sphere(WALKER_SIZE);
    for(uint i=0; i<NUM_WALKERS; ++i)
    {
      Walker *w = new Walker(*this);
      w->SetShape(*sh);
      Root().Add(*w);
    }
  }

//-----------------------------------------------------------------------------

  bool Walker::Act()
  {
  	MoveBy(Vector(Point::Random(Point::_0,MAX_STEP_SIZE)));
	  return true;
  }


// eof
