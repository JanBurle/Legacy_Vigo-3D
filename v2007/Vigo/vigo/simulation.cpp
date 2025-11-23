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
* $Id: simulation.cpp 45 2006-06-21 10:20:33Z ian_b $
\*****************************************************************************/

  #include "./simulation.hpp"
  #include "./camera.hpp"
  #include "../space/scene.hpp"
  #include "./exguard.inc"

NAMESPACE_VIGO_() //***********************************************************

  Simulation::Simulation()
    : m_cameras(*this),
      m_menu(new Menu("&Simulation")),
      m_actStep("S&tep",cb_step,this,NULL,'S'),
      m_isRunning(false), m_actRun("&Run",cb_run,this,&m_isRunning,'R'),
      m_dock(new control::Dock("Dock")),
      m_timeout(cb_timeout,this), //>>> change to CallbackAction->member fun ptr
      m_timer(1000u,m_timeout), m_deltaT(1./18.5), m_frameSteps(0),
      m_scene(NULL)
  {
    m_menu->Add(m_actStep);
    m_menu->Add(m_actRun);
    SetFrameRate(1/m_deltaT);
    SetFrameSteps(1);
  }


  Simulation::~Simulation()
  {
    m_cameras.DetachAll(); // before deleting the dock
    delete m_dock;
    delete m_menu;
  }


  bool Simulation::Attach(Camera& cam)
  {
    return m_cameras.Attach(cam);
  }


  bool Simulation::Detach(Camera& cam)
  {
    return m_cameras.Detach(cam);
  }


  pvoid Simulation::cb_step(pvoid This_)
  {
    Simulation *This = static_cast<Simulation*>(This_);
    This->SingleShot();
    return NULL;
  }


  pvoid Simulation::cb_run(pvoid This_)
  {
    Simulation *This = static_cast<Simulation*>(This_);
    if(This->IsRunning())
      This->Start();
    else
      This->Stop();
    return NULL;
  }


  void Simulation::Start()
  {
    m_isRunning = true;
    m_timer.Start();
  }


  void Simulation::Stop()
  {
    m_timer.Stop();
    m_isRunning = false;
    UpdateGL(0x2); // redraw the status text
  }


  void Simulation::SingleShot()
  {
    Stop();
    m_timer.SingleShot();
  }


  void Simulation::Frame()
  {
    bool res = false;
    if(m_scene)
    {
      for(uint step=0; step<m_frameSteps; ++step)
      {
        for(uint phase=0; phase<m_scene->NumActPhases(); ++phase)
        {
          m_scene->SetActPhase(phase);
          res = m_scene->Act() || res;
        }

        m_scene->AdvanceTime();
      }
    }

    UpdateGL(res ? 0xFFFFFFFF : 0x2/*status*//*>>>enums*/);
  }


  void Simulation::SetFrameRate(double rate)
  {
    SetTimeout(1./rate);
  }


  double Simulation::FrameRate()
  {
    return 1./Timeout();
  }


  void Simulation::SetTimeout(double sec)
  {
    m_timer.SetTimeout((uint)(.5 + sec*1000.));
  }


  double Simulation::Timeout()
  {
    return m_timer.Timeout()/1000.;
  }


  void Simulation::SetFrameSteps(uint steps)
  {
    m_frameSteps = steps;
  }


  uint Simulation::FrameSteps() const
  {
    return m_frameSteps;
  }


  void Simulation::SetDeltaT(double deltaT)
  {
    m_deltaT = deltaT;
    if(m_scene) m_scene->SetDeltaT(deltaT);
  }


  double Simulation::SimTime() const
  {
    return m_scene ? m_scene->SimTime() : 0.0;
  }


  pvoid Simulation::cb_timeout(pvoid This_)
  {
    Simulation *This = static_cast<Simulation*>(This_);
    EXGUARD_BEGIN()
    This->Advance();
    EXGUARD_END() //>>> not enough! segmentation fault if 'throw' propagates
    return NULL;
  }


  std::string Simulation::StatusText() const
  {
    char runIcon =
      IsRunning() ? ((FrameSteps()>1) ? '*' : '+') : ' ';

    std::string simTime = 
      m_scene ? sformat("%06.2f",m_scene->SimTime()) : "";

    std::string sceneText = m_scene ? m_scene->StatusText() : "";
   
    return sformat("%c%s %s",runIcon,simTime.c_str(),sceneText.c_str());
  }


  static void UpdateCamera(Camera& cam,pvoid planes)
  {
    cam.UpdateGL((uint)planes);
  }


  void Simulation::UpdateGL(uint planes)
  {
    m_cameras.ForEach(UpdateCamera,(pvoid)planes);
  }


  void Simulation::PaintGL(Camera const& cam)
  {
    if(m_scene) m_scene->Display(*const_cast<Camera*>(&cam)/*>>>fix*/);
  }


  bool Simulation::SelectObject(algebra3d::Lined const& eyeRay,int kind, uint mod)
  {
    return (NULL!=m_scene) && m_scene->SelectObject(eyeRay,kind,mod);
  }


  void Simulation::SetScene(space::SceneBase* scene)
  {
    if(m_scene)
    {
      m_scene->m_simulation = NULL;
    }

    m_scene = scene;

    if(m_scene)
    {
      m_scene->SetDeltaT(m_deltaT);
      m_scene->m_simulation = this;
    }

    UpdateGL(0xFFFFFFFF);
  }


NAMESPACE_END_() //** eof *****************************************************
