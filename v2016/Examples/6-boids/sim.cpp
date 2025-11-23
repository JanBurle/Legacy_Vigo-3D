// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "sim.hpp"
  #include "scene.hpp"
  #include "params.hpp"

  typedef Scene TheScene; // Ouch! there is a conflict with Sim::Scene()

//-----------------------------------------------------------------------------

  Sim::Sim()
    : super(),
      m_actFlock(cb_dock, this, &m_flock), m_flock(params::FLOCKING::flock),
      m_act_gee (cb_dock, this, &m_gee),   m_gee(50),
      m_act_Cs  (cb_dock, this, &m_Cs),    m_Cs(50),
      m_act_Ca  (cb_dock, this, &m_Ca),    m_Ca(50),
      m_act_Cc  (cb_dock, this, &m_Cc),    m_Cc(50),
      m_act_Cg  (cb_dock, this, &m_Cg),    m_Cg(50)
  {
    #define PAR(par) GetDock()->AddSlider(#par,m_act_##par,0,100,50,true);
    PAR(gee)
    GetDock()->AddCheckBox("Flock", m_actFlock);
    PAR(Cs)
    PAR(Ca)
    PAR(Cc)
    PAR(Cg)
    #undef PAR
  }


  void Sim::DockUpdate()
  {
    TheScene *s = (TheScene*)super::Scene();
    if(!s) return;

    #define PAR(par) s->par_physics.par = m_##par * params::PHYSICS::par / 50.;
    PAR(gee)
    #undef PAR
    #define PAR(par) s->par_flocking.par = m_##par * params::FLOCKING::par / 50.;
    PAR(Cg)
    s->par_flocking.flock = m_flock;
    PAR(Cs)
    PAR(Ca)
    PAR(Cc)
    #undef PAR
  }

// eof
