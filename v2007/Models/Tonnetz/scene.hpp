/*****************************************************************************\
*    Copyright (c) 2006 by Ian G Burleigh           burleigh@cpsc.ucalgary.ca *
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
* $Id: scene.hpp 47 2006-06-21 10:34:49Z ian_b $
\*****************************************************************************/

/** \file
 */

#ifndef TONNETZ_SCENE_HPP_INCLUDED
#define TONNETZ_SCENE_HPP_INCLUDED

  #include <vigosim>
  using vigo::algebra3d::Pointf;
  using vigo::algebra3d::Pointd;
  using vigo::algebra3d::Vectord;

//-----------------------------------------------------------------------------
    
  double const SCENE_RADIUS = 100.0;
  
  class Scene: public vigo::space::Scenef
  {
    SUPER(vigo::space::Scenef)

  public:
    Scene();

    double CameraFar()  const { return 4*SCENE_RADIUS;           }
    Pointd CameraEye()  const { return Pointd(0,0,SCENE_RADIUS); }

    std::string StatusText() const;
  };

//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
