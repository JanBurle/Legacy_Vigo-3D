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
* $Id: dock.cpp 197 2007-01-15 19:32:55Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "./dock.hpp"
#undef foreach //>>>
  #include "../vigo/window.hpp"
  #include "../QtVigo/window.hpp"
 
  #include <QtGui/QLayout>
  #include <QtGui/QLabel>

//-----------------------------------------------------------------------------

  QtVigoDock_::QtVigoDock_(vigo::control::Dock& iface, pcstr title)
    : super(title,NULL/*>>>QtVigoWindow_::Get().Widget()*/ /*parent*/),
      vigo::internal::pimpl_iface<vigo::control::Dock>(iface),
      m_controls()
  {
    setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
  }
  
    
  QtVigoDock_::~QtVigoDock_()
  {
    while(!m_controls.empty())
    {
      delete *m_controls.begin();
      m_controls.pop_front();
    }
  }
  
  
  vigo::control::PushButton* QtVigoDock_::AddPushButton(pcstr label,vigo::ToggleAction& action)
  {
    vigo::control::PushButton* p = new vigo::control::PushButton(label,action);
    AddControl(p);
    return p;
  }

  
  vigo::control::CheckBox* QtVigoDock_::AddCheckBox(pcstr label,vigo::ToggleAction& action)
  {
    vigo::control::CheckBox* p = new vigo::control::CheckBox(label,action);
    AddControl(p);
    return p;
  }

  
  vigo::control::Slider* QtVigoDock_::AddSlider(pcstr label,vigo::DataAction<int>& action,
                                                int min,int max,int initValue,
		    		                bool tracking)
  {
    vigo::control::Slider* p = new vigo::control::Slider(label,action,min,max,initValue,tracking);
    if(!vigo::empty(label)) layout()->addWidget(new QLabel(label));
    AddControl(p);
    return p;
  }

  
  vigo::control::SpinBox* QtVigoDock_::AddSpinBox(pcstr label,vigo::DataAction<int>& action,
                                                int min,int max,int initValue)
  {
    vigo::control::SpinBox* p = new vigo::control::SpinBox(label,action,min,max,initValue);
    if(!vigo::empty(label)) layout()->addWidget(new QLabel(label));
    AddControl(p);
    return p;
  }

  
  void QtVigoDock_::Show(bool b)
  {
    if(b) show(); else hide();
  }


  void QtVigoDock_::AddControl(vigo::control::Control* c)
  {
    layout()->addWidget((QWidget*)c->Widget());
    adjustSize();
    m_controls.push_back(c);
  }

/*** eof ********************************************************************/
