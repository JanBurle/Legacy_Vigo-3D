/*****************************************************************************\
*    Copyright (c) 2004--2007 by Ian G Burleigh                               *
*    https://github.com/phi53/Vigo-3D                                         *
*                                                                             *
*    This program is free software; you can redistribute it and/or modify     *
*    it under the terms of the GNU General Public License as published by     *
*    the Free Software Foundation; either version 2 of the License, or        *
*    (at your option) any later version.                                      *
*                                                                             *
*    This program is distributed in the hope that it will be useful,          *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*    GNU General Public License for more details.                             *
*                                                                             *
*    You should have received a copy of the GNU General Public License        *
*    along with this program; if not, write to the                            *
*    Free Software Foundation, Inc.,                                          *
*    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                *
*-----------------------------------------------------------------------------*
* $Id: ogl.hpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

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

//** eof **********************************************************************
