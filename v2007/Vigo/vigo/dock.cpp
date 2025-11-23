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
* $Id: dock.cpp 172 2006-11-04 20:57:28Z ian_b $
\*****************************************************************************/

  #include "./dock.hpp"
  #include "../QtVigo/dock.hpp"

NAMESPACE_VIGO(control) //*****************************************************

  Dock::Dock(pcstr title)
    : internal::pimpl<QtVigoDock_>(new QtVigoDock_(*this,title))
  {
  }


  Dock::~Dock()
  {
  }


#define PIMPL_FACE_CLASS Dock
#define PIMPL_IMPL_CLASS QtVigoDock_
#include "../common/pimpl.inc"

  PIMPL_IMPL(AddPushButton, PushButton*, (pcstr label,ToggleAction& action), (label,action),)
  PIMPL_IMPL(AddCheckBox,   CheckBox*,   (pcstr label,ToggleAction& action), (label,action),)
  PIMPL_IMPL(AddSlider,     Slider*,     (pcstr label,DataAction<int>& action,
                           int min,int max,int init,bool tr), (label,action,min,max,init,tr),)
  PIMPL_IMPL(AddSpinBox,    SpinBox*,    (pcstr label,DataAction<int>& action,
                           int min,int max,int init), (label,action,min,max,init),)

  PIMPL_IMPL(Show,   void, (bool b), (b),)
  PIMPL_IMPL(Widget, pvoid,(),(),)

NAMESPACE_END() //** eof ******************************************************
