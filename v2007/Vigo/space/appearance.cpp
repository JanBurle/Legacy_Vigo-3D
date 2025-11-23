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
* $Id:appearance.cpp 30 2006-04-06 19:13:28Z ian_b $
\*****************************************************************************/

  #include "./shape.hpp"
  #include "../common/ogl.hpp"
  #include <math.h>

#ifdef __SUNOS__
  #include <stdio.h>
#endif

  NAMESPACE_VIGO(shape)

//----------------------------------------------------------------------------

  DrawFlags::DrawFlags(): flags(0x0)
  {
  }

  DrawFlags::DrawFlags(uint f): flags(f)
  {
  }

  void DrawFlags::SetFlag(uint f, bool set)
  {
    SetBits(f,set);
  }

  void DrawFlags::SetBits(uint f,bool set)
  {
    if(set)
      flags |= f;
    else
      flags &= ~f;
  }

  void DrawFlags::SetDraw(DrawFlags const* carryFrom) const
  {
    uint draw = carryFrom ? (carryFrom->flags&0xF000) : 0x0;

    if(IsFlag(flagSOLID))   draw |=  drawSOLID;
    else
    if(IsFlag(flagWIRE))    draw &= ~drawSOLID;

    if(IsFlag(flagLIGHT))   draw |=  drawLIGHT;
    else
    if(IsFlag(flagNOLIGHT)) draw &= ~drawLIGHT;

    const_cast<DrawFlags*>(this)->flags = (flags&0x0FFF) | draw;
  }

//-----------------------------------------------------------------------------

  Texture::Texture(pcstr fileName, uint width, uint height)
    : super(), m_texture(0)
  {
    if(empty(fileName) || 0==width || 0==height)
      return;

    FILE *file = fopen(fileName,"rb");
    if(!file)
      return;

    glGenTextures(1,(GLuint*)&m_texture); // sizeof(uint)==sizeof(GLuint)
    glBindTexture(GL_TEXTURE_2D,m_texture);

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL/*GL_MODULATE*/);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    pbyte data = (pbyte)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
    fclose(file);
    gluBuild2DMipmaps(GL_TEXTURE_2D,3,width,height,GL_RGB,GL_UNSIGNED_BYTE,data);
    free(data);
  }


  Texture::~Texture()
  {
    glDeleteTextures(1,(GLuint*)&m_texture);
  }


  void Texture::Activate() const
  {
    if(m_texture)
    {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D,m_texture);
    }
  }


  void Texture::Deactivate() const
  {
    if(m_texture)
    {
      glDisable(GL_TEXTURE_2D);
    }
  }


NAMESPACE_END() //** eof ******************************************************
