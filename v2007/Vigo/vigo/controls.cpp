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
* $Id: controls.cpp 172 2006-11-04 20:57:28Z ian_b $
\*****************************************************************************/

  #include "./controls.hpp"
  #include "../QtVigo/controls.hpp"

NAMESPACE_VIGO(control) //*****************************************************

  PushButton::PushButton(pcstr label,ToggleAction& action)
    : internal::pimpl<QtVigoPushButton_>(new QtVigoPushButton_(*this,label,NULL!=action.PData())),
      m_action(action)
  {
  }


  PushButton::~PushButton()
  {
  }


#define PIMPL_FACE_CLASS PushButton
#define PIMPL_IMPL_CLASS QtVigoPushButton_
#include "../common/pimpl.inc"

  PIMPL_IMPL(Widget,pvoid,(),(),)

#undef PIMPL_FACE_CLASS
#undef PIMPL_IMPL_CLASS

//-----------------------------------------------------------------------------

  CheckBox::CheckBox(pcstr label,ToggleAction& action)
    : internal::pimpl<QtVigoCheckBox_>(new QtVigoCheckBox_(*this,label)),
      m_action(action)
  {
  }


  CheckBox::~CheckBox()
  {
  }


#define PIMPL_FACE_CLASS CheckBox
#define PIMPL_IMPL_CLASS QtVigoCheckBox_
#include "../common/pimpl.inc"

  PIMPL_IMPL(Widget,pvoid,(),(),)

#undef PIMPL_FACE_CLASS
#undef PIMPL_IMPL_CLASS

//-----------------------------------------------------------------------------

  Slider::Slider(pcstr label,DataAction<int>& action,int min,int max,int initValue,bool tracking)
    : internal::pimpl<QtVigoSlider_>(new QtVigoSlider_(*this,label)),
      m_action(action)
  {
    Set(min,max);
    SetValue(initValue);
    SetTracking(tracking);
  }
  
  
  Slider::~Slider()
  {
  }


#define PIMPL_FACE_CLASS Slider
#define PIMPL_IMPL_CLASS QtVigoSlider_
#include "../common/pimpl.inc"

  PIMPL_IMPL(Set,         void, (int min,int max),(min,max),)
  PIMPL_IMPL(SetTracking, void, (bool t),         (t),)
  PIMPL_IMPL(SetValue,    void, (int value),      (value),)

  PIMPL_IMPL(Widget,pvoid,(),(),)

#undef PIMPL_FACE_CLASS
#undef PIMPL_IMPL_CLASS
  
//-----------------------------------------------------------------------------

  SpinBox::SpinBox(pcstr label,DataAction<int>& action,int min,int max,int initValue)
    : internal::pimpl<QtVigoSpinBox_>(new QtVigoSpinBox_(*this,label)),
      m_action(action)
  {
    Set(min,max);
    SetValue(initValue);
  }
  
  
  SpinBox::~SpinBox()
  {
  }


#define PIMPL_FACE_CLASS SpinBox
#define PIMPL_IMPL_CLASS QtVigoSpinBox_
#include "../common/pimpl.inc"

  PIMPL_IMPL(Set,         void, (int min,int max),(min,max),)
  PIMPL_IMPL(SetValue,    void, (int value),      (value),)

  PIMPL_IMPL(Widget,pvoid,(),(),)

#undef PIMPL_FACE_CLASS
#undef PIMPL_IMPL_CLASS
  
NAMESPACE_END() //** eof ******************************************************
