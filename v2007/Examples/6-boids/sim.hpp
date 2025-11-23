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

#ifndef BOIDS_SIM_HPP_INCLUDED
#define BOIDS_SIM_HPP_INCLUDED

  #include <vigosim>
  class Scene;

//-----------------------------------------------------------------------------

  class Sim: public vigo::Simulation
  {
    SUPER(vigo::Simulation)
    typedef Sim *THIS;
    
  public:
    Sim();

  private:
    static pvoid cb_dock(pvoid This) { ((THIS)This)->DockUpdate(); return NULL; }
    void DockUpdate();

    vigo::ToggleAction    m_actFlock;  bool m_flock;
    vigo::DataAction<int> m_act_gee;   int  m_gee;

    vigo::DataAction<int> m_act_Cs;    int  m_Cs;
    vigo::DataAction<int> m_act_Ca;    int  m_Ca;
    vigo::DataAction<int> m_act_Cc;    int  m_Cc;
    vigo::DataAction<int> m_act_Cg;    int  m_Cg;
  };


//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
