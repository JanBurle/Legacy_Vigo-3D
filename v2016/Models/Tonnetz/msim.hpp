// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef TONNETZ_MSIM_HPP_INCLUDED
#define TONNETZ_MSIM_HPP_INCLUDED

  #include "sim.hpp"

//-----------------------------------------------------------------------------

  class MoebiusSim: public Simulation
  {
    SUPER(Simulation)
    typedef MoebiusSim *THIS;

  public:
    MoebiusSim();

  private:
    void SetFreq(float freq,bool just);
    void RHN();
    static pvoid cb_rhn(pvoid This) { ((THIS)This)->RHN();   return NULL; }
    vigo::DataAction<int> m_actR, m_actH, m_actN;
    int m_slideR, m_slideH, m_slideN;

    void Play(int);
    static pvoid cb_ups(pvoid This) { ((THIS)This)->Play(0); return NULL; }
    static pvoid cb_dns(pvoid This) { ((THIS)This)->Play(1); return NULL; }
    static pvoid cb_up5(pvoid This) { ((THIS)This)->Play(2); return NULL; }
    static pvoid cb_dn5(pvoid This) { ((THIS)This)->Play(3); return NULL; }
    vigo::MenuAction m_actPlayUpSemi, m_actPlayDnSemi,
                     m_actPlayUp5, m_actPlayDn5;
  };


//-----------------------------------------------------------------------------

#endif

// eof
