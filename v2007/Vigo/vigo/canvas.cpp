/*****************************************************************************\
* This file is a part of Vigo::3D, a multi-agent simulation and visualization *
* system.                                                                     *
*                                                                             *
* Copyright (c) 2002--2006 by Ian G. Burleigh       burleigh@cpsc.ucalgary.ca *
* burleigh@cpsc.ucalgary.ca                         http://ard-galen.org/vigo *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for   *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the                                     *
*   Free Software Foundation, Inc.,                                           *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
*-----------------------------------------------------------------------------*
* $Id: canvas.cpp 225 2007-05-21 12:49:53Z ian_b $
\*****************************************************************************/

  #include "./canvas.hpp"
  #include "./window.hpp"
  #include "./camera.hpp"
  #include "./simulation.hpp"
  #include "./resource.hpp"
  #include "./fixfont.hpp"
  #include "../QtVigo/canvas.hpp"
  #include "../space/shape.hpp"
  #include <math.h>
  #include <png.h>

NAMESPACE_VIGO_() //***********************************************************

  Canvas::Canvas(Window& wnd,bool setWidget)
    : internal::pimpl<QtVigoCanvas_>(new QtVigoCanvas_(*this)),
      m_mouseLastPos(0), m_isCameraOp(false),
      m_w(0), m_h(0),
      m_wnd(&wnd), m_cam(NULL),
      m_modelMatrix(), m_projMatrix(),
      m_quadricObj(NULL)
  {
    m_wnd->Attach(*this,setWidget);
  }


  Canvas::~Canvas()
  {
    if(m_cam)        m_cam->Detach(*this);
    if(m_quadricObj) gluDeleteQuadric(m_quadricObj);
  }


  bool Canvas::AttachTo(Camera* cam)
  {
    if(cam!=m_cam)
    {
      if(m_cam) m_cam->Detach(*this);
      m_cam = cam;
      if(m_cam) m_cam->Attach(*this);
      WindowPolish();
      return true;
    }

    return false;
  }


  Camera* Canvas::GetCamera()
  {
    return m_cam;
  }


  Menu* Canvas::GetCameraMenu()
  {
    return m_cam ? &(m_cam->GetMenu()) : NULL;
  }


  Simulation* Canvas::GetSimulation()
  {
    return m_cam ? m_cam->GetSimulation() : NULL;
  }


  Menu* Canvas::GetSimulationMenu()
  {
    return m_cam ? m_cam->GetSimulationMenu() : NULL;
  }


  control::Dock* Canvas::GetSimulationDock()
  {
    return m_cam ? m_cam->GetSimulationDock() : NULL;
  }


  void Canvas::InitializeGL()
  {
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
    glEnable(GL_NORMALIZE);
  }


  void Canvas::ClearGL()
  {
    static shape::Color black(0,0,0);

    shape::Color const &c = m_cam ? m_cam->ClearColor() : black;

    glClearColor(c.r,c.g,c.b,1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  }


  void Canvas::TransformGL()
  {
    if(!m_cam || 0>=m_w || 0>=m_h) return;

    // projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double ratio = (double)m_w / (double)m_h;
    double rads  = algebra3d::deg2rad(m_cam->Fov()/2.0);
    double tb    = m_cam->Near()/*>>>not sure about this*/ * tan(rads) * m_cam->ZoomF();
    double lr    = ratio * tb;
    glFrustum(-lr,lr,-tb,tb,m_cam->Near(),m_cam->Far());
    glGetDoublev(GL_PROJECTION_MATRIX,m_projMatrix);

    // model
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    algebra3d::Pointd const &eye = m_cam->Eye();
    algebra3d::Pointd const &ctr = m_cam->Center();
    algebra3d::Pointd const &up  = m_cam->Up();
    gluLookAt(eye.x,eye.y,eye.z,
              ctr.x,ctr.y,ctr.z,
              up.x, up.y, up.z);

    // remember, for DoubleClick-UnProject
    assert(sizeof(GLdouble)==sizeof(m_modelMatrix[0]));
    glGetDoublev(GL_MODELVIEW_MATRIX,m_modelMatrix);
  }


  void Canvas::PaintGL()
  {
    if(!m_cam || 0>=m_w || 0>=m_h) return;

    glShadeModel(m_cam->IsSmooth() ? GL_SMOOTH : GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    bool isBlend = m_cam->IsBlend();
    if(isBlend)
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    m_cam->PaintGL(*this);

    if(isBlend)
    {
      glDisable(GL_BLEND);
    }
  }


  void Canvas::ResizeGL(int w, int h)
  {
    if(w!=m_w || h!=m_h)
    {
      m_w = w; m_h = h;
      glViewport(0,0,w,h);
      UpdateGL(0xFFFFFFFF);
    }
  }


  void Canvas::DrawStatusGL()
  {
    if(!m_cam || 0>=m_w || 0>=m_h) return;

    glMatrixMode(GL_PROJECTION); //
    glLoadIdentity();
    gluOrtho2D(0,m_w,0,m_h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glShadeModel(GL_FLAT);
    glDisable(GL_DEPTH_TEST);

    static shape::ColorAlpha background(.3,.3,.5,.1);
    static shape::ColorAlpha foreground(.8,.8,.8,.1);
    SetColor(background);
    PaintBox(0,0,m_w,CharHeight());

    uint lineCapacity = m_w / CharWidth();
    Simulation *sim = GetSimulation();
    std::string statusText = sim ? sim->StatusText() : "";
    if(m_cam->IsGrabin()) statusText = 'G' + statusText;
    uint statusLgt = statusText.length();

    std::string vigoText = res::VigoText();
    uint vigoLgt = vigoText.length();

    SetColor(foreground);
    SetTextPos(0,0);
    PaintText(statusText);

    if(statusLgt + vigoLgt + 2/*pad*/ <= lineCapacity)
    {
      SetTextPos(lineCapacity-vigoLgt,0);
      PaintText(vigoText);
    }

    m_cam->PaintDone(*this);
  }


  void Canvas::Select(int x, int y, int kind, uint mod)
  {
    if(m_cam)
    {
      if(!m_isCameraOp) // try to select an object first
      {
        GLint viewPort[4] = {0,0,m_w,m_h};
        algebra3d::Lined ray;

        assert(sizeof(GLdouble)==sizeof(double));
        gluUnProject(x, m_h-y, 0, m_modelMatrix, m_projMatrix, viewPort,
                     &ray.a.x, &ray.a.y, &ray.a.z); // near clipping plane
        gluUnProject(x, m_h-y, 1, m_modelMatrix, m_projMatrix, viewPort,
                     &ray.b.x, &ray.b.y, &ray.b.z); // fvfar clipping plane

        if(m_cam->Select(ray,kind,mod))
        {
          m_cam->UpdateGL(0xFFFFFFFF);
        }
      }

      // proceed with a camera op, even if an object was selected
      switch(kind)
      {
      case +1:
        m_isCameraOp   = true;
        break;
      case -1:
        m_isCameraOp   = false;
        break;
      case 0:
        if(m_isCameraOp && CameraOp(x,y,mod))
        {
          m_cam->UpdateGL(0xFFFFFFFF);
          break;
        }
      }
    }

    m_mouseLastPos = util::int2d(x,y);
  }


  bool Canvas::CameraOp(int x, int y, uint mod)
  {
    int dx = x - m_mouseLastPos.u;
    int dy = y - m_mouseLastPos.v;
    if(0==dx && 0==dy) return false;

    if(modCTRL == mod)
      return CameraTranslate(-dx,dy);
    if(modALT == mod)
      return CameraRotate(0,0,dy);
    if((modCTRL|modALT) == mod || modMETA == mod)
      return CameraZoom(dy);
    if(modSHIFT == mod)
      return CameraDolly(-dy);
    //else
    return CameraRotate(-dx,dy,0);
  }

  bool Canvas::CameraRotate(int dx,int dy,int dz)
  {
    return (NULL!=m_cam) && m_cam->Rotate(dx,dy,dz);
  }


  bool Canvas::CameraTranslate(int dx,int dy)
  {
    return (NULL!=m_cam) && m_cam->Translate(dx,dy);
  }


  bool Canvas::CameraZoom(int dx)
  {
    return (NULL!=m_cam) && m_cam->Zoom(dx);
  }


  bool Canvas::CameraDolly(int d)
  {
    return (NULL!=m_cam) && m_cam->Dolly(d);
  }

  void Canvas::SetColor(shape::ColorAlpha const& c)
  {
    glColor4d(c.r, c.g, c.b, c.a);
  }


  void Canvas::PaintRect(int x1,int y1,int x2,int y2)
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glRecti(x1, y1, x2, y2);
    CHECK_GL()
  }


  void Canvas::PaintBox(int x1,int y1,int x2,int y2)
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glRecti(x1, y1, x2, y2);
    CHECK_GL()
  }


//----------------------------------------------------------------------------

  uint Canvas::CharWidth()
  {
    return 8; // fixed
  }


  uint Canvas::CharHeight()
  {
    return 14; // fixed
  }


  void Canvas::SetTextPos(uint x, uint y)
  {
    glRasterPos3d(x*CharWidth()+2,y*CharHeight()+3,0.9999);
  }


  void Canvas::PaintText(char c)
  {
    BitmapCharacter(c);
  }


  void Canvas::PaintText(pcstr p)
  {
    if(p)
      for( ; *p; ++p)
        PaintText(*p);
  }


  void Canvas::WindowPolish()
  {
    if(m_wnd) m_wnd->Polish();
  }


  GLUquadricObj* Canvas::QuadricObj()
  {
    // create on demand, after all GL init. is done
    if(!m_quadricObj)
      m_quadricObj = gluNewQuadric();
    return m_quadricObj;
  }


#define PIMPL_FACE_CLASS Canvas
#define PIMPL_IMPL_CLASS QtVigoCanvas_
#include "../common/pimpl.inc"

  PIMPL_IMPL(CaptureToPng,bool,  (pcstr f),(f),const)
  PIMPL_IMPL(PaintText,   void,  (double x,double y,double z,pcstr text),(x,y,z,text),)
  PIMPL_IMPL(Widget,      pvoid, (),(),)
  PIMPL_IMPL(UpdateGL,    void,  (uint planes),(planes),)

NAMESPACE_END_() //** eof *****************************************************
