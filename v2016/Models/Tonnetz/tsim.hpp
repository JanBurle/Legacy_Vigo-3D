// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef TONNETZ_TSIM_HPP_INCLUDED
#define TONNETZ_TSIM_HPP_INCLUDED

  #include "sim.hpp"

//-----------------------------------------------------------------------------

  class TonnetzSim: public Simulation
  {
    SUPER(Simulation)
    typedef TonnetzSim *THIS;

  public:
    TonnetzSim();

  private:
    void SetFreq(float freq,bool just);
  };


//-----------------------------------------------------------------------------

#endif

// eof
