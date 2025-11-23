// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef TONNETZ_SIM_HPP_INCLUDED
#define TONNETZ_SIM_HPP_INCLUDED

  #include <vigosim>

//-----------------------------------------------------------------------------

  /// Common simulation base class
  class Simulation: public vigo::Simulation
  {
    SUPER(vigo::Simulation)
    typedef Simulation *THIS;

  public:
    Simulation();

  private:
    virtual void SetFreq(float freq,bool just) = 0;
    static pvoid cb_set_eq(pvoid This) { ((THIS)This)->SetFreq(440.,false); return NULL; }
    static pvoid cb_set_jt(pvoid This) { ((THIS)This)->SetFreq(440.,true);  return NULL; }
    vigo::MenuAction m_setEq, m_setJt;
  };


//-----------------------------------------------------------------------------

#endif

// eof
