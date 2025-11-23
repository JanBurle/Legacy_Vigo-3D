// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  Guards exceptions from propagating to Qt slots
 */

// named .inc rather than .hpp to escape moc compilation
#ifndef VIGO_QT_EXGUARD_INC_INCLUDED
#define VIGO_QT_EXGUARD_INC_INCLUDED

  #include "../common/types.hpp"
  #include "../vigo/application.hpp"

  #define EXGUARD_BEGIN() \
    try {

  #define EXGUARD_END() \
    } catch(vigo::exc& e) { \
      vigo::Application &app = vigo::Application::Get(); \
      app.ErrorPopup(e.What()); \
      throw; \
    }

#endif

// eof
