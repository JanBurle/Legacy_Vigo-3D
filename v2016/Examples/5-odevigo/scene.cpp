// Copyright (c) 2006 by Marcin Pilat http://pilat.org

/** \file
 */

  #include "scene.hpp"
  #include "agent.hpp"
  #include "params.hpp"

//-----------------------------------------------------------------------------

  Scene::Scene()
    : m_world(dWorldCreate()), m_space(dHashSpaceCreate(0)),
      m_contactgroup(dJointGroupCreate(0))
  {
    // set some rules
    dWorldSetERP(m_world,0.2);
    dWorldSetCFM(m_world,1e-10);
    dWorldSetGravity(m_world,0,0,0.0);
    dWorldSetAutoDisableFlag(m_world,1);
    dWorldSetContactMaxCorrectingVel(m_world,0.1);
    dWorldSetContactSurfaceLayer(m_world,0.001);

    SetNumActPhases(1);
    Populate();
  }


  Scene::~Scene()
  {
	  // ODE cleanup
    if(m_contactgroup) dJointGroupDestroy(m_contactgroup);
    if(m_space)        dSpaceDestroy(m_space);
    if(m_world)        dWorldDestroy(m_world);
  }


  bool Scene::Act()
  {
    switch(ActPhase())
    {
	  case 0:
      dSpaceCollide(m_space, this, &Scene::nearCallback);
      dWorldQuickStep(m_world,0.5);
      dJointGroupEmpty(m_contactgroup);
      break;
    }

    return super::Act();
  }


  std::string Scene::StatusText() const
  {
	  // display a selected node's coordinates
    vigo::space::AbstractNode<double> *n = SelectedObject();
    if(!n) return super::StatusText();
    Point pos = n->Pos();
    return vigo::sformat("%4.1f:%4.1f:%4.1f",pos.x,pos.y,pos.z);
  }


  void Scene::Populate()
  {
    uint const NUM_AGENTS = params::APP::numAgents;
    bool const RAND_POS   = params::APP::randPos;
    for(uint i=0; i<NUM_AGENTS; ++i)
    {
      Point pos =
        RAND_POS ? Point::Random(Point::_0,5.0) : Point::_0;
      Root().Add(*new Agent(*this,pos));
    }
  }

  void Scene::nearCallback (pvoid This, dGeomID o1, dGeomID o2)
  {
    ((Scene*)This)->nearCallback(o1,o2);
  }

  void Scene::nearCallback(dGeomID o1, dGeomID o2)
  {
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    if(b1==b2)
      return; // no contact withi tself

    uint const MAX_CONTACTS = 8;
    dContact contact[MAX_CONTACTS]; // up to MAX_CONTACTS contacts per box-box
    for(uint i=0; i<MAX_CONTACTS; ++i)
    {
	    dSurfaceParameters &p = contact[i].surface;
      p.mode       = dContactBounce | dContactSoftCFM;
      p.mu         = dInfinity;
      p.mu2        = 0;
      p.bounce     = 0.1;
      p.bounce_vel = 0.1;
      p.soft_cfm   = 0.01;
    }

    if(uint numc = dCollide(o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact)))
    {
      for(uint i=0; i<numc; ++i)
      {
        dJointID c = dJointCreateContact(World(),ContactGroup(),contact+i);
        dJointAttach(c,b1,b2);
      }
    }
  }

// eof
