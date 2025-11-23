// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "sim.hpp"

//-----------------------------------------------------------------------------

  Simulation::Simulation()
    : super(),
      m_setEq("Eq 12",cb_set_eq,this), m_setJt("Just",cb_set_jt,this)
  {
    //GetMenu()->Add(m_setJt);
    //GetMenu()->Add(m_setEq);
  }


// eof
