// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

// eof
