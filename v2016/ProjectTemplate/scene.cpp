// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "scene.hpp"
  #include "agent.hpp"

//-----------------------------------------------------------------------------

  Scene::Scene()
  {
    SetNumActPhases(1);
    Create();
  }


  std::string Scene::StatusText() const
  {
    vigo::space::AbstractNode<double> *n = SelectedObject();
    if(!n) return super::StatusText();
    Point pos = n->Pos();
    return vigo::sformat("%4.1f:%4.1f:%4.1f",pos.x,pos.y,pos.z);
  }


  void Scene::Create()
  {
	uint const NUM_AGENTS = 9;
	for(uint i=0; i<NUM_AGENTS; ++i)
	{
      Root().Add(*new Agent(*this));
    }
  }

// eof
