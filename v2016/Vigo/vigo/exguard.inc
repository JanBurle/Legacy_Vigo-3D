// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  Guards exceptions from propagating to Qt slots, text output
 */

// named .inc rather than .hpp to escape moc compilation
#ifndef VIGO_QTTEXT_EXGUARD_INC_INCLUDED
#define VIGO_QTTEXT_EXGUARD_INC_INCLUDED

  #include "../common/types.hpp"
  #include <iostream>
  #include <iomanip>


  #define EXGUARD_BEGIN() \
    try {

  #define EXGUARD_END() \
    } catch(vigo::exc& e) { \
      std::cerr << e.What() << std::endl; \
      throw; \
    }

#endif

// eof
