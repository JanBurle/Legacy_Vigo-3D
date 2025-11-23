// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

// eof
