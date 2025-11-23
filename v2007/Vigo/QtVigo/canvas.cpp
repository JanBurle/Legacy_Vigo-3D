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
* $Id: canvas.cpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "./canvas.hpp"
  #include "../common/ogl.hpp"
  #include "./exguard.inc"
  #include <QtGui/QMouseEvent>
  #include <QtGui/QImage>

//-----------------------------------------------------------------------------

  QtVigoCanvas_::QtVigoCanvas_(vigo::Canvas& iface)
    : super(/*QGLFormat(QGL::DoubleBuffer)*/),
      vigo::internal::pimpl_iface<vigo::Canvas>(iface),
      m_updatingPlanes(0)
  {
    setMouseTracking(true); //>>> make optional
  }


  QtVigoCanvas_::~QtVigoCanvas_()
  {
  }


  void QtVigoCanvas_::UpdateGL(uint planes)
  {
    m_updatingPlanes = planes;
    updateGL();
    m_updatingPlanes = 0;
  }
  
  
  bool QtVigoCanvas_::CaptureToPng(pcstr fileName) const
  {
    return const_cast<QtVigoCanvas_*>(this)
      ->grabFrameBuffer(false).save(fileName,"PNG");
  }


  void QtVigoCanvas_::PaintText(double x, double y, double z, pcstr text)
  {
    if(text)
      renderText(x,y,z,text);
  }

  
  void QtVigoCanvas_::initializeGL()
  {
    EXGUARD_BEGIN()
    m_iface.InitializeGL();
    CHECK_GL()
    EXGUARD_END()
  }
  
  
  void QtVigoCanvas_::paintGL()
  {
    EXGUARD_BEGIN()
    if(0x1 & m_updatingPlanes)
    {   
      m_iface.ClearGL();
      m_iface.TransformGL();
      m_iface.PaintGL();
    }
    m_iface.DrawStatusGL();
    CHECK_GL()
    EXGUARD_END()
  }


  void QtVigoCanvas_::resizeGL(int w, int h)
  {
    EXGUARD_BEGIN()
    m_iface.ResizeGL(w,h);
    CHECK_GL()
    EXGUARD_END()
  }
  

  void QtVigoCanvas_::mouseMoveEvent(QMouseEvent* event)
  {
    MouseEvent(event,0);
  }


  void QtVigoCanvas_::mousePressEvent(QMouseEvent* event)
  {
    MouseEvent(event,+1);
  }


  void QtVigoCanvas_::mouseReleaseEvent(QMouseEvent* event)
  {
    MouseEvent(event,-1);
  }


  void QtVigoCanvas_::mouseDoubleClickEvent(QMouseEvent* event)
  {
    MouseEvent(event,+2);
  }


  void QtVigoCanvas_::MouseEvent(QMouseEvent* event,int kind)
  {
    Qt::KeyboardModifiers qmod = event->modifiers();
    uint mod = 0;
    if(qmod & Qt::ControlModifier) mod |= vigo::Canvas::modCTRL;
    if(qmod & Qt::ShiftModifier)   mod |= vigo::Canvas::modSHIFT;
    if(qmod & Qt::AltModifier)     mod |= vigo::Canvas::modALT;
    if(qmod & Qt::MetaModifier)    mod |= vigo::Canvas::modMETA;
    
    m_iface.Select(event->x(), event->y(), kind, mod);
  }


/*** eof ********************************************************************/
