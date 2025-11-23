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
* $Id: menu.cpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "./menu.hpp"
  #include <QtGui/QAction>
  #include <QtCore/QVariant>
  #include <QtGui/QKeySequence>

//-----------------------------------------------------------------------------

  QtVigoMenuAction_::QtVigoMenuAction_(vigo::MenuAction& iface,pcstr text,
                                       vigo::MenuAction::kind k,uint shortcut)
    : super(text,NULL), vigo::internal::pimpl_iface<vigo::MenuAction>(iface),
      m_kind(k), m_shortcut(shortcut)
  {
    if(k==vigo::MenuAction::SEPARATOR)
    {
      setSeparator(true);
      return;
    }

    if(!connect(this,SIGNAL(triggered(bool)),this,SLOT(Triggered(bool))))
      vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                "connect SLOT(Triggered(bool)) failed"));
    Polish();
  }
  

  QtVigoMenuAction_::~QtVigoMenuAction_()
  {
  }
  
  
  void QtVigoMenuAction_::Polish()
  {
    if(vigo::MenuAction::SEPARATOR==m_kind)
      return;

    bool checkable = false;
    bool hasPBool  = (NULL!=m_iface.PBool());
    
    switch(m_kind)
    {
    case vigo::MenuAction::SMART:
    case vigo::MenuAction::TOGGLE:
      checkable = hasPBool;
      break;
    default:
      break;
    }

    setCheckable(checkable);

    if(checkable)
      setChecked(hasPBool && *m_iface.PBool());
    
    if(m_shortcut&0xFF)
    {
      std::string s = vigo::sformat("%c",(char)(m_shortcut&0xFF));
      if(m_shortcut&modALT)   s = "Alt+"   + s;
      if(m_shortcut&modCTRL)  s = "Ctrl+"  + s;
      if(m_shortcut&modSHIFT) s = "Shift+" + s;
      setShortcut(QKeySequence(s.c_str()));
    }
    else
    {
      setShortcut(QKeySequence(""));
    }
  }
  

  void QtVigoMenuAction_::Triggered(bool b)
  {
    m_iface.Triggered(b);
  }
  

//-----------------------------------------------------------------------------

  QtVigoMenu_::QtVigoMenu_(vigo::Menu& iface, pcstr title)
    : super(title), vigo::internal::pimpl_iface<vigo::Menu>(iface)
  {
  }


  QtVigoMenu_::~QtVigoMenu_()
  {
  }


  void QtVigoMenu_::Add(vigo::MenuAction& act)
  {
    addAction(static_cast<QAction*>(act.Widget()));
    act.Polish();
  }
  
  
  void QtVigoMenu_::Remove(vigo::MenuAction& act)
  {
    removeAction(static_cast<QAction*>(act.Widget()));
  }
  
/*** eof ********************************************************************/
