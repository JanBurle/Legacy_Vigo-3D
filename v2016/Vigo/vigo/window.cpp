// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

  #include "./window.hpp"
  #include "./canvas.hpp"
  #include "../QtVigo/window.hpp"

NAMESPACE_VIGO_() //***********************************************************

  Window::Window(pcstr title, bool show)
    : internal::pimpl<QtVigoWindow_>(new QtVigoWindow_(*this,title,show)),
      m_canvas(NULL),
      m_windowMenu("&Window"),
      m_fullscreen(false),
      m_actFullscreen("&Full screen",cb_fullscreen,this,&m_fullscreen,'F'),
      m_showDock(false),
      m_actShowDock  ("&Dock",       cb_showdock,  this,&m_showDock,  'D')
  {
    AddMenu(m_windowMenu);

    m_fullscreen = pImpl().IsFullScreen();
    m_windowMenu.Add(m_actFullscreen);
    m_showDock   = pImpl().IsDockShown();
    m_windowMenu.Add(m_actShowDock);
  }


  Window::~Window()
  {
    RemoveMenu(m_windowMenu);
  }


  void Window::FullScreen(bool b)
  {
    m_fullscreen = b;
    pImpl().FullScreen(b);
  }


  void Window::ShowDock(bool b)
  {
    m_showDock = b;
    pImpl().ShowDock(b);
  }


  void Window::Attach(Canvas& cvs,bool setWidget)
  {
    RUNTIME_CHECK_EXC3(NULL==m_canvas,TOO_MANY_CANVASSES,
                      "A window can have only one canvas")
    m_canvas = &cvs;
    pImpl().Attach(cvs,setWidget);
    Polish();
  }


  pvoid Window::cb_fullscreen(pvoid This_)
  {
    Window *This = static_cast<Window*>(This_);
    This->FullScreen(This->m_fullscreen);
    return NULL;
  }


  pvoid Window::cb_showdock(pvoid This_)
  {
    Window *This = static_cast<Window*>(This_);
    This->ShowDock(This->m_showDock);
    return NULL;
  }


#define PIMPL_FACE_CLASS Window
#define PIMPL_IMPL_CLASS QtVigoWindow_
#include "../common/pimpl.inc"

  PIMPL_IMPL(AddMenu,     void, (Menu& m), (m),)
  PIMPL_IMPL(RemoveMenu,  void, (Menu& m), (m),)
  PIMPL_IMPL(Show,        void, (bool b), (b),)
  PIMPL_IMPL(Polish,      void, (),(),)
  PIMPL_IMPL(Widget,      pvoid,(),(),)

  PIMPL_IMPL(Move,        void, (int x,int y),(x,y),)
  PIMPL_IMPL(Resize,      void, (int w,int h),(w,h),)
  PIMPL_IMPL(X,           int,  (),(),)
  PIMPL_IMPL(Y,           int,  (),(),)
  PIMPL_IMPL(Width,       int,  (),(),)
  PIMPL_IMPL(Height,      int,  (),(),)

NAMESPACE_END_() // eof
