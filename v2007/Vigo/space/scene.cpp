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
* $Id:scene.cpp 30 2006-04-06 19:13:28Z ian_b $
\*****************************************************************************/

  #include "./scene.hpp"
  #include "../common/ogl.hpp"

NAMESPACE_VIGO(space) //=======================================================

  void SceneBase::UpdateFrame() //>>> should perhaps only flag the scene
  {
    if(m_simulation) m_simulation->UpdateGL(0xFFFFFFFF);
  }

//-----------------------------------------------------------------------------

  void sceneGL::GLPush() { glPushMatrix(); }
  void sceneGL::GLPop()  { glPopMatrix();  }

  void sceneGL::GLTranslate(algebra3d::Pointd const& pos)
  {
    glTranslated(pos.x,pos.y,pos.z);
  }

  void sceneGL::GLTranslate(algebra3d::Pointf const& pos)
  {
    glTranslatef(pos.x,pos.y,pos.z);
  }
  
  void sceneGL::GLTranslate(algebra3d::Pointi const& pos)
  {
    glTranslatef(pos.x,pos.y,pos.z);
  }

  void sceneGL::GLRotate(algebra3d::Rotation const& rot) 
  { 
    glRotated(algebra3d::rad2deg(rot.th),rot.v.x,rot.v.y,rot.v.z); 
  }

  void sceneGL::GLEnableLighting()  { glEnable (GL_LIGHTING); }
  void sceneGL::GLDisableLighting() { glDisable(GL_LIGHTING); }

  void sceneGL::GLColor(shape::ColorAlpha const &c)
  {
    glColor4d(c.r,c.g,c.b,c.a);
  }

  void sceneGL::GLColor(double r, double g, double b, double a)
  {
    glColor4d(r,g,b,a);
  }

NAMESPACE_END() //** eof ******************************************************
