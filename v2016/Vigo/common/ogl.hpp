// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief OpenGL support.
 *
 */

#ifndef VIGO_OGL_HPP_INCLUDED
#define VIGO_OGL_HPP_INCLUDED

  #include "./types.hpp"

  //include OpenGL header files
  #if defined(__MACX__)         // MAC OS X
    #include <GLUT/glut.h>
  #else                         // everything else
    #include <GL/gl.h>
    #include <GL/glu.h>
    #ifdef _MSC_VER             // even Redmond ??
      // #include <windows.h>   //>>>
    #endif
  #endif

NAMESPACE_VIGO(ogl) //=========================================================
/** \defgroup grp_ogl_hpp OpenGL
@{*/

/** \def CHECK_GL()
  * Checks the OpenGL state and throws an exception if there is an error.
  */
/** \def CHECK_GL1(msg)
  * Checks the OpenGL state and throws an exception if there is an error.
  */

#ifdef _DEBUG
  void CheckGl(pcstr file, int line, pcstr msg=NULL) throw(exc);
#else
  void CheckGl(pcstr msg=NULL) throw(exc);
#endif

#ifdef _DEBUG
  #define CHECK_GL()      vigo::ogl::CheckGl(__FILE__, __LINE__);
  #define CHECK_GL1(msg)  vigo::ogl::CheckGl( __FILE__, __LINE__, msg );
#else
  #define CHECK_GL()      vigo::ogl::CheckGl();
  #define CHECK_GL1(msg)  vigo::ogl::CheckGl(msg);
#endif

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
