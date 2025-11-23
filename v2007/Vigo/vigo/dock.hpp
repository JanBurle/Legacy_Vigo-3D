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
* $Id: dock.hpp 172 2006-11-04 20:57:28Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Vigo window
 */

#ifndef VIGO_VIGO_DOCK_HPP_INCLUDED
#define VIGO_VIGO_DOCK_HPP_INCLUDED

  #include "./controls.hpp"
  #include "../common/pimpl.hpp"

  class QtVigoDock_;

NAMESPACE_VIGO(control) //=====================================================
/** \defgroup grp_dock_hpp Dock
@{*/

  class Dock: protected internal::pimpl<QtVigoDock_>
  {
    NO_GEN(Dock)

  public:
    Dock(pcstr title);
   ~Dock();

    PushButton* AddPushButton(pcstr label,ToggleAction&);
    CheckBox*   AddCheckBox  (pcstr label,ToggleAction&);
    Slider*     AddSlider    (pcstr label,DataAction<int>&,int min,int max,int init,bool tracking);
    SpinBox*    AddSpinBox   (pcstr label,DataAction<int>&,int min,int max,int init);

    void Show(bool);

    pvoid Widget();
  };

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

//** eof **********************************************************************
