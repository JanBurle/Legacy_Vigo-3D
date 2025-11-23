// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "tsim.hpp"
  #include "tscene.hpp"

//-----------------------------------------------------------------------------

  TonnetzSim::TonnetzSim()
    : super()
  {
  }


  void TonnetzSim::SetFreq(float freq,bool just)
  {
    TonnetzScene* ms = dynamic_cast<TonnetzScene*>(Scene());
    if(ms)
    {
      ms->SetFreq(freq,just);
      UpdateGL();
    }
  }


// eof
