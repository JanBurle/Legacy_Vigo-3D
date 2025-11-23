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
* $Id: menu.hpp 45 2006-06-21 10:20:33Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Vigo menu
 */

#ifndef VIGO_VIGO_MENU_HPP_INCLUDED
#define VIGO_VIGO_MENU_HPP_INCLUDED

  #include "../common/pimpl.hpp"
  #include "./actions.hpp"

  class QtVigoMenu_;
  class QtVigoMenuAction_;

NAMESPACE_VIGO_() //===========================================================
/** \defgroup grp_menu_hpp Menu
@{*/

  class MenuAction;

  class Menu: protected internal::pimpl<QtVigoMenu_>
  {
    NO_GEN(Menu)

  public:
    Menu(pcstr title);
   ~Menu();

    void Add   (MenuAction&);
    void Remove(MenuAction&);

    void AddSeparator();
    pvoid Widget();
  };

  
  class MenuAction: protected internal::pimpl<QtVigoMenuAction_>
  {
    NO_GEN(MenuAction)
    friend class Menu;
    
  public:
    enum kind      { TRIGGER, TOGGLE, SMART, SEPARATOR};
    enum modifiers { NONE = 0x00, CTRL = 0x01, ALT = 0x02, SHIFT = 0x04};
    // uppercase shortcut implies CTRL
    
    MenuAction(pcstr text, ToggleAction::pfun, ToggleAction::param,
                           ToggleAction::pdata,
                           kind, char shortcut, uint modifiers);
    MenuAction(pcstr text, ToggleAction::pfun, ToggleAction::param=NULL,
                           ToggleAction::pdata /*toggled if not NULL*/=NULL,
                           char shortcut='\0', uint modifiers=NONE);
   ~MenuAction();

    bool* PBool();
    void  Triggered(bool);
    
    void  Polish();
    pvoid Widget();

  private:
    AbstractAction *m_action;
  };


/**@} (defgroup)*/
NAMESPACE_END_() //============================================================

#endif

//** eof **********************************************************************
