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
* $Id: simulation.hpp 45 2006-06-21 10:20:33Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Vigo simulation
 */

#ifndef VIGO_VIGO_SIMULATION_HPP_INCLUDED
#define VIGO_VIGO_SIMULATION_HPP_INCLUDED

  #include "../common/patterns.hpp"
  #include "../space/algebra.hpp"
  #include "./menu.hpp"
  #include "./dock.hpp"
  #include "./timer.hpp"

  NAMESPACE_VIGO(space)
    class SceneBase;
  NAMESPACE_END()

NAMESPACE_VIGO_() //===========================================================
/** \defgroup grp_simulation_hpp Simulation
@{*/

  class Camera;

  class Simulation
  {
    NO_GEN(Simulation)

  public:
    Simulation();
    virtual ~Simulation();

  // cameras
  public:
    bool Attach(Camera&);
    bool Detach(Camera&);
  private:
    internal::has_many<Simulation,Camera> m_cameras;

  // widgets
  public:
    GETTER(GetMenu, Menu*, m_menu)
    GETTER(GetDock, control::Dock*, m_dock)
  private:
    Menu *m_menu;
    MenuAction m_actStep; bool m_isRunning; MenuAction m_actRun;
    static pvoid cb_step(pvoid);
    static pvoid cb_run(pvoid);
    control::Dock *m_dock;

  // run control
  public:
    void Start();
    void Stop();
    void SingleShot();
    GETTER(IsRunning, bool, m_isRunning)
    
    virtual void Advance() { Frame(); }
    
  private:
    void Frame();

  // timing
  public:
    // (max.) number of frames / sec
    void   SetFrameRate(double);
    double FrameRate();

    // (min.) length of one frame period
    void   SetTimeout(double);  // timeout, 1./framerate
    double Timeout();

    // number of simulation steps / one frame
    void   SetFrameSteps(uint);
    uint   FrameSteps() const;

    // simulation time / one step
    GETTER(DeltaT, double, m_deltaT) //>>>? review "division of labour" w. Scene
    void   SetDeltaT(double);

    double SimTime() const;

  private:
    CallbackAction m_timeout;
    Timer  m_timer;
    double m_deltaT;
    uint   m_frameSteps;
    static pvoid cb_timeout(pvoid);

  // display
  public:
    // override this
    virtual std::string StatusText() const;
    // refresh screen
    void UpdateGL(uint planes=0xFFFFFFFF);
    // paint screen
    void PaintGL(Camera const&);

  // interaction
  public:
    /// Select an object close to the eye->p line
    bool SelectObject(algebra3d::Lined const& eyeRay, int kind, uint mod);

  // scene
  public:
    GETTER(Scene, space::SceneBase*, m_scene);
    void SetScene(space::SceneBase*);
    void SetScene(space::SceneBase& scene)
    {
      SetScene(&scene);
    }

  private:
    space::SceneBase *m_scene;
  };


/**@} (defgroup)*/
NAMESPACE_END_() //============================================================

#endif

//** eof **********************************************************************
