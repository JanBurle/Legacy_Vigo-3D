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

  #include "sim.hpp"

//-----------------------------------------------------------------------------

  Scene::Scene()
  {
    Create();
  }

  void Scene::Create()
  {
    vigo::shape::Shape *sh = new vigo::shape::Sphere(WALKER_SIZE);
    for(uint i=0; i<NUM_WALKERS; ++i)
    {
      Walker *w = new Walker(*this);
      w->SetShape(*sh);
      Root().Add(*w);
    }
  }

//-----------------------------------------------------------------------------

  bool Walker::Act()
  {
  	MoveBy(Vector(Point::Random(Point::_0,MAX_STEP_SIZE)));
	  return true;
  }


//** eof **********************************************************************
