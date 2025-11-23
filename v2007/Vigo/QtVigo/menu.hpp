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
* $Id: menu.hpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 */

#ifndef VIGO_QTVIGO_MENU_HPP_INCLUDED
#define VIGO_QTVIGO_MENU_HPP_INCLUDED

  #include "../vigo/menu.hpp"
  #include "../vigo/actions.hpp"
  #include "../common/pimpl.hpp"
  #include <QtGui/QMenu>
  #include <QtGui/QAction>

//-----------------------------------------------------------------------------

  class QtVigoMenuAction_: public QAction,
                           protected vigo::internal::pimpl_iface<vigo::MenuAction>
  {
    SUPER(QAction)
    NO_GEN(QtVigoMenuAction_)
    Q_OBJECT
    
  public:
    enum {modCTRL = 0x0100, modALT = 0x0200, modSHIFT = 0x0400};
    QtVigoMenuAction_(vigo::MenuAction& iface,pcstr text,
                      vigo::MenuAction::kind,uint shortcut);
   ~QtVigoMenuAction_();
   
    void Polish();
    
    QtVigoMenuAction_* Widget()    { return this; }

  private slots:
    void Triggered(bool);
    
  private:
    vigo::MenuAction::kind m_kind; 
    uint m_shortcut;
  };
  
//-----------------------------------------------------------------------------

  class QtVigoMenu_: public QMenu,
                     protected vigo::internal::pimpl_iface<vigo::Menu>
  {
    SUPER(QMenu)
    NO_GEN(QtVigoMenu_)
    Q_OBJECT

  public:
    QtVigoMenu_(vigo::Menu& iface, pcstr title);
   ~QtVigoMenu_();

    void Add   (vigo::MenuAction&);
    void Remove(vigo::MenuAction&);

    QtVigoMenu_* Widget()       { return this;           }
  };

//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
