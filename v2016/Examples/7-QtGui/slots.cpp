// Adapted from Trolltech hellogl example

#include "slots.hpp"

//-----------------------------------------------------------------------------

  void Slots::setX(int x)
  {
    WATCH(x)
    // connect to Vigo
    // m_cvs.DoSomething(), m_sim.DoSomething() ... etc
  }

  void Slots::setY(int y)
  {
    WATCH(y)
  }

  void Slots::setZ(int z)
  {
    WATCH(z)
  }


// eof
