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
* $Id: main.cpp 192 2007-01-13 03:54:25Z ian_b $
\*****************************************************************************/

/** \file
 */

  // include Vigo application classes
  #include <vigoapp>
  // include Vigo simulation classes
  #include <vigosim>
  // include OpenGL headers
  #include <vigoogl>

  // miscelaneous includes
  #include <iostream>

//-----------------------------------------------------------------------------

  typedef float spct;   // space coordinates type

  typedef vigo::space::Leaf<spct, vigo::parts::Parts<
                              vigo::parts::P2<spct>,
                              vigo::parts::R0,
                              vigo::parts::T0<spct>,
                              vigo::parts::SS,
                              vigo::parts::A1
                            >
          > Ball;

//-----------------------------------------------------------------------------

  class MyScene: public vigo::space::Scene<spct>
  {
    SUPER(vigo::space::Scene<spct>)
    
  public:
    MyScene()
      : m_moebius(new vigo::geom::MoebiusBoundary(1,.1)),
        m_drawLines(false)
    {
      Create();
    }
    
    double CameraFar()  const { return 40; }
    vigo::algebra3d::Pointd CameraEye()  const { return vigo::algebra3d::Pointd(0,0,8); }
    
    SETTER(SetDrawLines, bool, m_drawLines)
    void SetMoebius(double r, double h)
    {
      m_moebius->Set(r,h);
    }
    
  private:
    vigo::util::refptr<vigo::geom::MoebiusBoundary> m_moebius;
    std::vector<Ball*> m_balls;

    bool m_drawLines;
    
    static uint const NUM_BALLS = 1000; // make it an even number
    
    void Create()
    {
      vigo::shape::Shape      &sh  = *new vigo::shape::Sphere(.01);
      vigo::shape::Appearance &app = *new vigo::shape::Appearance(1,0,0);

      for(uint i=0; i<NUM_BALLS; ++i)
      {
        Ball &b = *new Ball(*this,false);
        b.SetCurve(*m_moebius);
        b.SetU(double(i)/NUM_BALLS);
        b.SetShape(sh); b.SetAppearance(app);
        Root().Add(b);
        m_balls.push_back(&b);
      }
    }

    void DisplayRoot(vigo::Camera const& cam,vigo::space::sceneGL::eDisplay ed)
    {
      if(m_drawLines)
      {
        glColor3d(1,1,1);
        glBegin(GL_LINES);
        for(uint i=0; i<NUM_BALLS/2; ++i)
        {
          vigo::algebra3d::Point<spct>
            p1 = m_balls[i]->Pos(),
            p2 = m_balls[(i+1)%NUM_BALLS]->Pos(),
            p3 = m_balls[(i+NUM_BALLS/2)%NUM_BALLS]->Pos();
        
          glVertex3d(p1.x,p1.y,p1.z);
          glVertex3d(p2.x,p2.y,p2.z);
          glVertex3d(p1.x,p1.y,p1.z);
          glVertex3d(p3.x,p3.y,p3.z);
        }
        glEnd();
      }
      
      super::DisplayRoot(cam,ed);
    }
  };

//-----------------------------------------------------------------------------

  class Simulation: public vigo::Simulation
  {
    SUPER(vigo::Simulation)
    typedef Simulation *THIS;
    
  public:
    Simulation(): super(),
                  m_pushMe  (cb_pushMe,  this,NULL),
                  m_toggleMe(cb_toggleMe,this,&m_toggle), m_toggle(false),
                  m_checkMe (cb_checkMe, this,&m_check),  m_check (false),
                  m_slideMe (cb_slideMe, this,&m_slide),  m_slide (0),
                  m_spinMe  (cb_spinMe,  this,&m_spin),   m_spin  (0)
    {
      GetDock()->AddPushButton("Push Me",  m_pushMe);
      GetDock()->AddPushButton("Toggle Me",m_toggleMe);
      GetDock()->AddCheckBox  ("Check Me", m_checkMe);
      GetDock()->AddSlider    ("Slide Me", m_slideMe,0,100,0,true);
      GetDock()->AddSpinBox   ("Spin Me",  m_spinMe, 0,100,0);
    }
    
  private:
    void Pushed()  { TRACE("Pushed", 0)         }
    void Toggled() { ((MyScene*)Scene())->SetDrawLines(m_toggle);     UpdateGL(); }
    void Checked() { TRACE("Checked", m_check)  }
    void Slid()    { ((MyScene*)Scene())->SetMoebius(m_slide/50.,.1); UpdateGL(); }
    void Spun()    { TRACE("Spun", m_spin)      }
    
    static pvoid cb_pushMe  (pvoid This) { ((THIS)This)->Pushed();  return NULL; }
    static pvoid cb_toggleMe(pvoid This) { ((THIS)This)->Toggled(); return NULL; }
    static pvoid cb_checkMe (pvoid This) { ((THIS)This)->Checked(); return NULL; }
    static pvoid cb_slideMe (pvoid This) { ((THIS)This)->Slid();    return NULL; }
    static pvoid cb_spinMe  (pvoid This) { ((THIS)This)->Spun();    return NULL; }
    
    vigo::ToggleAction    m_pushMe;
    vigo::ToggleAction    m_toggleMe; bool m_toggle;
    vigo::ToggleAction    m_checkMe;  bool m_check;
    vigo::DataAction<int> m_slideMe;  int  m_slide;
    vigo::DataAction<int> m_spinMe;   int  m_spin;
  };

//-----------------------------------------------------------------------------

  int main(int argc, pstr argv[])
  {
    try
    {
      vigo::Application app("dock",argc,argv);

      try
      {
        app.ProcessArgs();
      }
      catch(vigo::exc& e)
      {
        std::cerr << e.What()   << std::endl;
        std::cout << app.Help() << std::endl;
        return -1;
      }

      vigo::Window wnd("My Window"), wnd2("Another Window");
      vigo::Canvas cvs(wnd), cvs2(wnd2);

      Simulation sim;
      MyScene scene;
      sim.SetScene(scene);

      vigo::Camera cam;
      cam.Attach(cvs); cam.Attach(cvs2);
      sim.Attach(cam);
      cam.Reset();

      return app.Exec();
    }
    catch(vigo::exc& e)
    {
      std::cerr << e.What() << std::endl;
      return -1;
    }
  }

//** eof **********************************************************************
