// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef BOIDS_SIM_HPP_INCLUDED
#define BOIDS_SIM_HPP_INCLUDED

  #include <vigosim>
  class Scene;

//-----------------------------------------------------------------------------

  class Sim: public vigo::Simulation
  {
    SUPER(vigo::Simulation)
    typedef Sim *THIS;

  public:
    Sim();

  private:
    static pvoid cb_dock(pvoid This) { ((THIS)This)->DockUpdate(); return NULL; }
    void DockUpdate();

    vigo::ToggleAction    m_actFlock;  bool m_flock;
    vigo::DataAction<int> m_act_gee;   int  m_gee;

    vigo::DataAction<int> m_act_Cs;    int  m_Cs;
    vigo::DataAction<int> m_act_Ca;    int  m_Ca;
    vigo::DataAction<int> m_act_Cc;    int  m_Cc;
    vigo::DataAction<int> m_act_Cg;    int  m_Cg;
  };


//-----------------------------------------------------------------------------

#endif

// eof
