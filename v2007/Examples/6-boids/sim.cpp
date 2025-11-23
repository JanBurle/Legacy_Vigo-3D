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
* $Id: main.cpp 46 2006-06-21 10:33:26Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "sim.hpp"
  #include "scene.hpp"
  #include "params.hpp"
 
  typedef Scene TheScene; // Ouch! there is a conflict with Sim::Scene()

//-----------------------------------------------------------------------------

  Sim::Sim()
    : super(),
      m_actFlock(cb_dock, this, &m_flock), m_flock(params::FLOCKING::flock),
      m_act_gee (cb_dock, this, &m_gee),   m_gee(50),
      m_act_Cs  (cb_dock, this, &m_Cs),    m_Cs(50),
      m_act_Ca  (cb_dock, this, &m_Ca),    m_Ca(50),
      m_act_Cc  (cb_dock, this, &m_Cc),    m_Cc(50),
      m_act_Cg  (cb_dock, this, &m_Cg),    m_Cg(50)
  {
    #define PAR(par) GetDock()->AddSlider(#par,m_act_##par,0,100,50,true);
    PAR(gee)
    GetDock()->AddCheckBox("Flock", m_actFlock);
    PAR(Cs)
    PAR(Ca)
    PAR(Cc)
    PAR(Cg)
    #undef PAR
  }


  void Sim::DockUpdate()
  { 
    TheScene *s = (TheScene*)super::Scene();
    if(!s) return;

    #define PAR(par) s->par_physics.par = m_##par * params::PHYSICS::par / 50.;
    PAR(gee)
    #undef PAR
    #define PAR(par) s->par_flocking.par = m_##par * params::FLOCKING::par / 50.;
    PAR(Cg)
    s->par_flocking.flock = m_flock;
    PAR(Cs)
    PAR(Ca)
    PAR(Cc)
    #undef PAR
  }

//** eof **********************************************************************
