// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

// eof
