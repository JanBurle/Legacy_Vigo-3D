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
* $Id:ogl.cpp 30 2006-04-06 19:13:28Z ian_b $
\*****************************************************************************/

/** \file
*/

  #include "./ogl.hpp"

  NAMESPACE_VIGO(ogl)

//-----------------------------------------------------------------------------

#ifdef _DEBUG
  void CheckGl(pcstr file, int line, pcstr msg) throw(exc)
#else
  void CheckGl(pcstr msg) throw(exc)
#endif
  {
    static uint const NUM_ERRORS = 6;
    static pcstr error_txt[NUM_ERRORS] =
    {
      "GL_INVALID_ENUM",
      "GL_INVALID_VALUE",
      "GL_INVALID_OPERATION",
      "GL_STACK_OVERFLOW",
      "GL_STACK_UNDERFLOW",
      "GL_OUT_OF_MEMORY"
    };

    GLenum r = glGetError();
    if(GL_NO_ERROR == r)
        return;

    std::string txt = (r-GL_INVALID_ENUM < NUM_ERRORS)
                      ? error_txt[r - GL_INVALID_ENUM]
                      : "";

#ifdef _DEBUG
    std::string smsg = sformat("GL error %u (%s) at %s, line %i",
                                r, txt.c_str(), file, line);
#else
    std::string smsg = sformat("GL error %u (%s)",
                                r, txt.c_str());
#endif
    if(!empty(msg))
        (smsg += "\n") += msg;

    THROW_EXC2(EXC_RUNTIME, smsg)
  }

NAMESPACE_END() //** eof ******************************************************
