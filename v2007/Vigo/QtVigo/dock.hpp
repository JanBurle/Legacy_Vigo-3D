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
* $Id: dock.hpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 */

#ifndef VIGO_QTVIGO_DOCK_HPP_INCLUDED
#define VIGO_QTVIGO_DOCK_HPP_INCLUDED

  #include "../vigo/dock.hpp"
  #include "../vigo/controls.hpp"
  #include "../common/pimpl.hpp"
  #include <QtGui/QDockWidget>

//-----------------------------------------------------------------------------
  
  class QtVigoDock_: public QDockWidget,
                     protected vigo::internal::pimpl_iface<vigo::control::Dock>
  {
    SUPER(QDockWidget)
    NO_GEN(QtVigoDock_)
    Q_OBJECT
    
  public:
    QtVigoDock_(vigo::control::Dock& iface, pcstr title);
   ~QtVigoDock_();

    vigo::control::PushButton* AddPushButton(pcstr label,vigo::ToggleAction&);
    vigo::control::CheckBox*   AddCheckBox  (pcstr label,vigo::ToggleAction&);
    vigo::control::Slider*     AddSlider    (pcstr label,vigo::DataAction<int>&,
                                             int min=0,int max=100,int initValue=0,
					     bool tracking=false);
    vigo::control::SpinBox*    AddSpinBox   (pcstr label,vigo::DataAction<int>&,
                                             int min=0,int max=100,int initValue=0);
  public:
    void Show(bool);
  
    QtVigoDock_* Widget()       { return this; }
    
  private:
    void AddControl(vigo::control::Control*);
    std::list<vigo::control::Control*> m_controls;
  };
  

//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
