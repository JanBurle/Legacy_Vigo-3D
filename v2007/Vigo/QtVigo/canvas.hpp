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
* $Id: canvas.hpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 */

#ifndef VIGO_QTVIGO_CANVAS_HPP_INCLUDED
#define VIGO_QTVIGO_CANVAS_HPP_INCLUDED

  #include "../vigo/canvas.hpp"
  #include "../common/pimpl.hpp"
  #include <QtCore/QObject>
  #include <QtOpenGL/QGLWidget>

//-----------------------------------------------------------------------------

  class QMouseEvent;

  class QtVigoCanvas_: public QGLWidget,
                       protected vigo::internal::pimpl_iface<vigo::Canvas>
  {
    SUPER(QGLWidget)
    NO_GEN(QtVigoCanvas_)
    Q_OBJECT

  public:
    QtVigoCanvas_(vigo::Canvas& iface);
   ~QtVigoCanvas_();
   
    QSize minimumSizeHint() const { return QSize(50,50);   }
    QSize sizeHint()        const { return QSize(400,400); }

    void UpdateGL(uint planes);
    bool CaptureToPng(pcstr fileName) const;
    
    void PaintText(double x, double y, double z, pcstr text);

    QtVigoCanvas_* Widget()       { return this;           }
    
  private:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

  // manipulate
  private:
    void mouseMoveEvent       (QMouseEvent*);
    void mousePressEvent      (QMouseEvent*);
    void mouseReleaseEvent    (QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);

    void MouseEvent(QMouseEvent*,int kind);
    
  private:
    uint m_updatingPlanes;
  };

//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
