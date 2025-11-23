// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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


NAMESPACE_END_() // eof
