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
* $Id: main.cpp 123 2006-08-11 16:57:14Z ian_b $
\*****************************************************************************/

/** \file
 */

#ifndef QT_GUI_SIM_HPP_INCLUDED
#define QT_GUI_SIM_HPP_INCLUDED

  #include <vigosim>
  #include <iostream>

  typedef vigo::algebra3d::Pointd  Point;
  typedef vigo::algebra3d::Vectord Vector;

  class Scene;

//-----------------------------------------------------------------------------

  double const SCENE_RADIUS   = 8.0;
  double const WALKER_SIZE    = 0.01;
  double const MAX_STEP_SIZE  = 0.002;
  uint   const NUM_WALKERS    = 40;

//-----------------------------------------------------------------------------

  class Scene: public vigo::space::Scened
  {
    SUPER(vigo::space::Scened)
	
  public:
    Scene();
    double CameraFar()  const { return SCENE_RADIUS*2;          }
    Point  CameraEye()  const { return Point(0,0,SCENE_RADIUS); }

    void Create();
  };

//-----------------------------------------------------------------------------

  class Simulation: public vigo::Simulation
  {
    SUPER(vigo::Simulation)
    
  public:
    Simulation(): super()
    {
    }
  };

//-----------------------------------------------------------------------------
// a walker

  typedef vigo::space::Node<double,
            vigo::parts::Parts<
          vigo::parts::PP<double>,
          vigo::parts::R0,
          vigo::parts::TC<double>,
          vigo::parts::SS,
          vigo::parts::A1
        >
      > SimpleAgent;

  class Walker: public SimpleAgent
  {
    SUPER(SimpleAgent)

  public:
    Walker(Scene& s) : super(s,true)
    {
    }

	bool Act();
  };

#endif

//** eof **********************************************************************
