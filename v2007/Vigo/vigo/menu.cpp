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
* $Id: menu.cpp 45 2006-06-21 10:20:33Z ian_b $
\*****************************************************************************/

  #include "./menu.hpp"
  #include "../QtVigo/menu.hpp"

NAMESPACE_VIGO_() //***********************************************************

  class MenuSeparator: public MenuAction
  {
    SUPER(MenuAction)
    NO_GEN(MenuSeparator)

  public:
    MenuSeparator(): super(NULL,NULL,NULL,NULL,super::SEPARATOR,'\0',super::NONE)
    {
    }

    static MenuSeparator separator;
  };


//-----------------------------------------------------------------------------

  Menu::Menu(pcstr title)
    : internal::pimpl<QtVigoMenu_>(new QtVigoMenu_(*this,title))
  {
  }


  Menu::~Menu()
  {
  }


  void Menu::Add(MenuAction& act)
  {
    pImpl().Add(act);
  }
  
  
  void Menu::Remove(MenuAction& act)
  {
    pImpl().Remove(act);
  }


  void Menu::AddSeparator()
  {
    Add(*new MenuSeparator/*>>> leaks ::separator*/);
  }

#define PIMPL_FACE_CLASS Menu
#define PIMPL_IMPL_CLASS QtVigoMenu_
#include "../common/pimpl.inc"

  //>>>out PIMPL_IMPL(Add,   void, (MenuAction& a),(a),)
  //>>>out PIMPL_IMPL(Remove,void, (MenuAction& a),(a),)
  PIMPL_IMPL(Widget,pvoid,(),(),)
#undef PIMPL_FACE_CLASS
#undef PIMPL_IMPL_CLASS

//-----------------------------------------------------------------------------

 MenuAction::MenuAction(pcstr text, ToggleAction::pfun  pf,
                                    ToggleAction::param par,
                                    ToggleAction::pdata pd,
                                    kind k, char shortcut, uint mods)
  : internal::pimpl<QtVigoMenuAction_>(),
    m_action(new ToggleAction(pf,par,pd))
 {
    if(isupper(shortcut)) mods |= CTRL;
    // must construct m_action before pImpl, in case that pImpl trigger()s
    // in its constructor
    pImpl(new QtVigoMenuAction_(*this,text,k,((uint)mods<<8)|shortcut));
 }


 MenuAction::MenuAction(pcstr text, ToggleAction::pfun  pf,
                                    ToggleAction::param par,
                                    ToggleAction::pdata pd,
                                    char shortcut, uint mods)
  : internal::pimpl<QtVigoMenuAction_>(),
    m_action(new ToggleAction(pf,par,pd))
 {
    if(isupper(shortcut)) mods |= CTRL;
    // must construct m_action before pImpl, in case that pImpl trigger()s
    // in its constructor
    pImpl(new QtVigoMenuAction_(*this,text,SMART,((uint)mods<<8)|shortcut));
 }


 MenuAction::~MenuAction()
 {
   delete m_action;
 }


 bool* MenuAction::PBool()
 {
   ToggleAction* ta = dynamic_cast<ToggleAction*>(m_action);
   if(!ta) return NULL;
   return ta->PData();
 }

 
 void MenuAction::Triggered(bool b)
 { 
   ToggleAction* ta = dynamic_cast<ToggleAction*>(m_action);
   if(ta) (*ta)(b);
   else   (*m_action)();
 }

#define PIMPL_FACE_CLASS MenuAction
#define PIMPL_IMPL_CLASS QtVigoMenuAction_
#include "../common/pimpl.inc"

  PIMPL_IMPL(Polish,void, (),(),)
  PIMPL_IMPL(Widget,pvoid,(),(),)

//-----------------------------------------------------------------------------

 MenuSeparator MenuSeparator::separator;


NAMESPACE_END_() //** eof *****************************************************
