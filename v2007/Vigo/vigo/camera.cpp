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
* $Id: camera.cpp 123 2006-08-11 16:57:14Z ian_b $
\*****************************************************************************/

  #include "./camera.hpp"
  #include "./simulation.hpp"
  #include "./canvas.hpp"
  #include "./dock.hpp"
  #include "./menu.hpp"
  #include "../space/scene.hpp"
  #include "../common/ogl.hpp"

NAMESPACE_VIGO_() //***********************************************************

  uint Camera::g_cameraIndex = 0;

  //>>> code copied below, clean up!
  Camera::Camera()
    : m_activeCanvas(NULL),
      m_far(0.), m_near(0.),
      m_eye(0,0,0), m_center(0,0,0), m_up(0,0,0),
      m_fov(0), m_zoom(0.),
      // the above members are se in ResetMembers()
      m_clearColor(.1,.1,.1),
      m_lights(),
      m_canvasses(*this), m_sim(NULL),
      m_cameraIndex(++g_cameraIndex), m_grabIndex(0),
      m_menu(new Menu("&Camera")),
      m_shapes(true),  m_actShapes("S&hapes",        cb_update,this,&m_shapes,'H'),
      m_labels(true),  m_actLabels("&Labels",        cb_update,this,&m_labels,'L'),
      m_grid(false),   m_actGrid  ("&Grid",          cb_update,this,&m_grid,  'G'),
      m_wire(false),   m_actWire  ("&Wire",          cb_update,this,&m_wire,  'W'),
      m_blend(false),  m_actBlend ("&Blend",         cb_update,this,&m_blend, 'B'),
      m_smooth(false), m_actSmooth("Sm&ooth",        cb_update,this,&m_smooth,'O'),
                       m_actGrab  ("Screen &Capture",cb_grab,  this,NULL     ,'C'),
      m_grabin(false), m_actMovie ("&Movie",         cb_movie, this,&m_grabin,'M'),
                       m_actReset ("Reset",          cb_reset,this)
  {
    ResetMembers();

    m_menu->Add(m_actShapes);
    m_menu->Add(m_actLabels);
    m_menu->AddSeparator();
    m_menu->Add(m_actGrid);
    m_menu->Add(m_actWire);
    m_menu->Add(m_actBlend);
    m_menu->Add(m_actSmooth);
    m_menu->AddSeparator();
    m_menu->Add(m_actGrab);
    m_menu->Add(m_actMovie);
    m_menu->AddSeparator();
    m_menu->Add(m_actReset);
  }


  Camera::~Camera()
  {
    delete m_menu;
  }


  //>>> copied code, clean up!
  Camera::Camera(Canvas& cvs,Simulation& sim)
    : m_activeCanvas(NULL),
      m_far(0.), m_near(0.),
      m_eye(0,0,0), m_center(0,0,0), m_up(0,0,0),
      m_fov(0), m_zoom(0.),
      // the above members are se in ResetMembers()
      m_clearColor(.1,.1,.1),
      m_lights(),
      m_canvasses(*this), m_sim(NULL),
      m_cameraIndex(++g_cameraIndex), m_grabIndex(0),
      m_menu(new Menu("&Camera")),
      m_shapes(true),  m_actShapes("S&hapes",cb_update,this,&m_shapes,'H'),
      m_labels(true),  m_actLabels("&Labels",cb_update,this,&m_labels,'L'),
      m_grid(false),   m_actGrid  ("&Grid",  cb_update,this,&m_grid,  'G'),
      m_wire(false),   m_actWire  ("&Wire",  cb_update,this,&m_wire,  'W'),
      m_blend(false),  m_actBlend ("&Blend", cb_update,this,&m_blend, 'B'),
      m_smooth(false), m_actSmooth("Sm&ooth",cb_update,this,&m_smooth,'O'),
                       m_actGrab  ("Grab",   cb_grab,  this),
      m_grabin(false), m_actMovie ("Movie",  cb_movie, this,&m_grabin),
                       m_actReset ("Reset",cb_reset,this)
  {
    ResetMembers();

    m_menu->Add(m_actShapes);
    m_menu->Add(m_actLabels);
    m_menu->AddSeparator();
    m_menu->Add(m_actGrid);
    m_menu->Add(m_actWire);
    m_menu->Add(m_actBlend);
    m_menu->Add(m_actSmooth);
    m_menu->AddSeparator();
    m_menu->Add(m_actGrab);
    m_menu->Add(m_actMovie);
    m_menu->AddSeparator();
    m_menu->Add(m_actReset);
    
    Attach(cvs); AttachTo(sim);
    Reset();
  }

  
  bool Camera::Attach(Canvas& cvs)
  {
    return m_canvasses.Attach(cvs);
  }


  bool Camera::Detach(Canvas& cvs)
  {
    return m_canvasses.Detach(cvs);
  }


  bool Camera::AttachTo(Simulation* sim)
  {
    if(sim!=m_sim)
    {
      if(m_sim) m_sim->Detach(*this);
      m_sim = sim;
      if(m_sim) m_sim->Attach(*this);
      WindowsPolish();
      return true;
    }

    return false;
  }


  void Camera::Reset()
  {
    ResetMembers();

    if(!m_sim) return;
    space::SceneBase *s = m_sim->Scene();
    if(!s) return;

    SetNear  (s->CameraNear());
    SetFar   (s->CameraFar());
    SetEye   (s->CameraEye());
    SetCenter(s->CameraCenter());
    SetUp    (s->CameraUp());
    SetFov   (s->CameraFov());
  }


  bool Camera::Rotate(int dx,int dy,int dz)
  {
    if(0==dx && 0==dy && 0==dz) return false;

    algebra3d::Vectord dir   = m_center-m_eye;
    algebra3d::Vectord right = (dir^m_up).Unit();

    // roll
    if(0 != dz)
    {
      double const SENSITIVITY = 0.001;
      m_up += ((double)dz * right * SENSITIVITY).Unit();
      Rotate(dx,dy,0);
      return true;
    }

    // rotate around center
    double radius = dir.Length();

    double const SENSITIVITY = 0.003;
    double delta = radius * SENSITIVITY;

    algebra3d::Vectord dirToEye
      = (m_eye + delta*((double)dx*right + (double)dy*m_up) - m_center).Unit();

    algebra3d::Pointd  newEye
      = m_center + radius*dirToEye;
    algebra3d::Vectord newRight
      = m_center + (newEye + right - m_center).Unit()*radius - newEye;

    m_eye = newEye;
    m_up  = newRight ^ (m_center-newEye).Unit();

    return true;
  }


  bool Camera::Translate(int dx,int dy)
  {
    algebra3d::Vectord dir   = m_center-m_eye;
    algebra3d::Vectord right = (dir^m_up).Unit();

    double const SENSITIVITY = 0.005;

    algebra3d::Vectord delta
      = ((m_center-m_eye).Length()*SENSITIVITY) * ((double)dy*m_up + (double)dx*right);

    m_eye    += delta;
    m_center += delta;

    return true;
  }


  bool Camera::Zoom(int d)
  {
    double const SENSITIVITY = 0.005;
    double const MIN_ZOOM=.001, MAX_ZOOM=1/MIN_ZOOM;
    SetZoomF(min(MAX_ZOOM,max(MIN_ZOOM,ZoomF()*(1+d*SENSITIVITY))));

    return true;
  }


  bool Camera::Dolly(int d)
  {
    double const SENSITIVITY = 0.002;
    algebra3d::Vectord dir   = m_center-m_eye;
    algebra3d::Vectord delta = dir*d*SENSITIVITY;

    m_eye    += delta;
    m_center += delta;

    return true;
  }


  static void UpdateCanvas(Canvas& cvs,pvoid planes)
  {
    cvs.UpdateGL((uint)planes);
  }


  bool Camera::Select(algebra3d::Lined const& ray, int kind, uint mod)
  {
    if(!IsShapes()) return false; // nothing to see, nothing to select
    // normalize the ray and move it to the camera eye
    algebra3d::Lined eyeRay(Eye(),Eye()+(ray.b-ray.a).Unit());
    return m_sim && m_sim->SelectObject(eyeRay,kind,mod);
  }
  

  void Camera::UpdateGL(uint planes)
  {
    m_canvasses.ForEach(UpdateCanvas,(pvoid)planes);
  }


  void Camera::PaintText(algebra3d::Pointd const& pos, pcstr text)
  {
    Canvas* cvs = m_canvasses.First(); //>>> wrong!, works only for one canvas
    if(cvs) cvs->PaintText(pos.x,pos.y,pos.z,text);
  }


  static float* get_caf(shape::ColorAlpha const& ca)
  {
    static float f[4];
    f[0] = (float)ca.r;
    f[1] = (float)ca.g;
    f[2] = (float)ca.b;
    f[3] = (float)ca.a;
    return f;
  }

  static float* get_cap(algebra3d::Pointd const& p,bool directional)
  {
    static float f[4];
    f[0] = (float)p.x;
    f[1] = (float)p.y;
    f[2] = (float)p.z;
    f[3] = directional ? 0.0f : 1.0f;
    return f;
  }

  void Camera::PaintGL(Canvas& cvs)
  {
    m_activeCanvas = &cvs;
    
    if(m_grid)
      PaintGridGL();

    if(m_sim)
    {
      GLint maxLights;                                                                                 
      glGetIntegerv(GL_MAX_LIGHTS, &maxLights);  
    
      // lights on
      glEnable(GL_LIGHTING);
      glEnable(GL_COLOR_MATERIAL);

      maxLights = min((int)m_lights.size(), (int)maxLights);

      // glPushMatrix();
      // glMultMatrixf(m_lightsRotation);
      for(int i=0; i<maxLights; ++i)
      {
        light const& l = m_lights[i]; GLenum li = GL_LIGHT0+i;
        glEnable (li);
        glLightfv(li, GL_AMBIENT,  get_caf(l.ambient));
        glLightfv(li, GL_DIFFUSE,  get_caf(l.diffuse));
        glLightfv(li, GL_POSITION, get_cap(l.position,l.directional));
      }
      // glPopMatrix();

      m_sim->PaintGL(*this);

      for(int i=0; i<maxLights; ++i)
      {
        glDisable(GL_LIGHT0+i);
      }

      // lights off
      glDisable(GL_LIGHTING);
      glDisable(GL_COLOR_MATERIAL);
    }
    
    m_activeCanvas = NULL;
  }


  void Camera::PaintGridGL()
  {
    double span = m_far, step = m_far/12.0;
    int const numSteps = int(span/step);
    shape::Color const
      xCol(0.0,0.0,0.4),
      yCol(0.0,0.4,0.0),
      zCol(0.4,0.0,0.0),
      _Col(0.6,0.6,0.6);

    glPushMatrix();

    for(int x=-numSteps; x<=numSteps; ++x)
    for(int y=-numSteps; y<=numSteps; ++y)
    for(int z=-numSteps; z<=numSteps; ++z)
    {
      shape::Color const *c;
      if(0==x || numSteps==abs(x))
        c = &xCol;
      else
      if(0==y || numSteps==abs(y))
        c = &yCol;
      else
      if(0==z || numSteps==abs(z))
        c = &zCol;
      else
        c = &_Col;

      glColor3d(c->r,c->g,c->b);

      double vx = x*step, vy = y*step, vz = z*step;

      glBegin(GL_POINTS);
      glVertex3d(vx,vy,vz);
      glEnd();
    }

    glColor3d(xCol.r,xCol.g,xCol.b);
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(span,0,0);
    glEnd();
    PaintText(algebra3d::Pointd(span/10.,0,0),"X");//>>> somehow <>0 coordinate seems too big

    glColor3d(yCol.r,yCol.g,yCol.b);
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(0,span,0);
    glEnd();
    PaintText(algebra3d::Pointd(0,span/10.,0),"Y");//>>> somehow <>0 coordinate seems too big

    glColor3d(zCol.r,zCol.g,zCol.b);
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(0,0,span);
    glEnd();
    PaintText(algebra3d::Pointd(0,0,span/10.),"Z");//>>> somehow <>0 coordinate seems too big

    // mark CameraCenter() >>>display as crosshairs?
    GLUquadricObj *q = ActiveCanvas()->QuadricObj();
    gluQuadricDrawStyle(q,GLU_LINE); // wire
    gluQuadricNormals(q,GLU_NONE);
    uint   const nSlices = 4;
    double const radius  = span/1000.;
    glTranslated(m_center.x,m_center.y,m_center.z);
    gluSphere(q,radius,nSlices,nSlices);

    glPopMatrix();
  }


  void Camera::PaintDone(Canvas& cvs)
  {
    if(m_grabin) CapturePng(cvs);
  }


  void Camera::ResetMembers()
  {
    m_far    = 1.0;
    m_near   = m_far/100.0;
    m_eye    = algebra3d::Pointd(0,0,1);
    m_center = algebra3d::Pointd::_0;
    m_up     = algebra3d::Vectord::Y;
    m_fov    = 45.0;
    m_zoom   = 1.0;
  }


  static void CanvasPolish(Canvas& cvs)
  {
    cvs.WindowPolish();
  }


  void Camera::WindowsPolish()
  {
    m_canvasses.ForEach(CanvasPolish);
  }


  void Camera::ClearLights()
  {
    m_lights.clear();
  }
    
    
  void Camera::AddLight(light const& l)
  {
    m_lights.push_back(l);
  }


  Menu* Camera::GetSimulationMenu() const
  {
    return m_sim ? m_sim->GetMenu() : NULL;
  }


  control::Dock* Camera::GetSimulationDock() const
  {
    return m_sim ? m_sim->GetDock() : NULL;
  }


  pvoid Camera::cb_update(pvoid This_)
  {
    Camera *This = static_cast<Camera*>(This_);
    This->UpdateGL(0xFFFFFFFF);
    return NULL;
  }


  pvoid Camera::cb_grab(pvoid This_)
  {
    Camera *This = static_cast<Camera*>(This_);
    This->Grab();
    return NULL;
  }


  pvoid Camera::cb_movie(pvoid This_)
  {
    Camera *This = static_cast<Camera*>(This_);
    This->Movie();
    return NULL;
  }


  pvoid Camera::cb_reset(pvoid This_)
  {
    Camera *This = static_cast<Camera*>(This_);
    This->Reset();
    This->UpdateGL(0xFFFFFFFF);
    return NULL;
  }


  void Camera::Grab()
  {
    if(!GetSimulation()) return;
    if(m_grabin) return; // movie is in progress
    
    ++m_grabIndex;
    m_grabin = true;
      UpdateGL(0xFFFFFFFF);
    m_grabin=false;
  }


  void Camera::Movie()
  {
    if(!GetSimulation()) return;
    if(m_grabin) // start a movie
    {
      ++m_grabIndex;
    }
  }


  void Camera::CapturePng(Canvas& cvs)
  {
    Simulation *sim = GetSimulation();
    ASSERT_EXC(sim)

    std::string fileName = sformat("cam%02u_%04u_%07.2f.png",m_cameraIndex,m_grabIndex,sim->SimTime());
    cvs.CaptureToPng(fileName.c_str());
  }

NAMESPACE_END_() //** eof *****************************************************
