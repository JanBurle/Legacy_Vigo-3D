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
* $Id: scene.cpp 47 2006-06-21 10:34:49Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "scene.hpp"
  #include "tagent.hpp"

//-----------------------------------------------------------------------------

  Scene::Scene()
  {
    SetNumActPhases(1);
  }


  std::string Scene::StatusText() const
  {
    TAgent *ta = dynamic_cast<TAgent*>(SelectedObject());
    if(!ta) return "";
    return vigo::sformat("f=%.2f",ta->Freq());
  }

  
//** eof **********************************************************************
