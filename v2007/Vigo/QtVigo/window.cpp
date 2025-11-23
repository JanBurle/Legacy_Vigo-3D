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
* $Id: window.cpp 197 2007-01-15 19:32:55Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "./window.hpp"
  #include "./dock.hpp"
  #include "./application.hpp"
  #include "../vigo/application.hpp"
  #include "../vigo/window.hpp"
  #include "../vigo/canvas.hpp"
  #include "../vigo/menu.hpp"
  #include "../vigo/dock.hpp"
  #include "../vigo/resource.hpp"
  #include <QtGui/QMenuBar>
  #include <QtGui/QCloseEvent>
  #include <QtGui/QMessageBox>
#undef foreach // >>>
  #include "../common/socket.hpp"

//-----------------------------------------------------------------------------

  static bool g_closing_ = false; // when Qt is shutting down, down do certain cleanup

  QtVigoWindow_::QtVigoWindow_(vigo::Window& iface, pcstr title, bool show)
    : super(NULL/*parent*/), vigo::internal::pimpl_iface<vigo::Window>(iface),
      m_progress(NULL),
      m_cameraMenu(NULL), m_simulationMenu(NULL), m_helpMenu(NULL),
      m_dock(NULL), m_showDock(false)
  {
    setWindowTitle(title);

    QtVigoApplication_::layout_t const &l =
      QtVigoApplication_::Get().WindowLayout();

    if(l.IsSpecified())
    {
      move(l.x,l.y);
      resize(l.w,l.h);
    }

    m_helpMenu   = menuBar()->addMenu("&Help");
    m_helpMenu->addAction("&About",this,SLOT(about()));
    m_helpMenu->addAction("&Update...",this,SLOT(checkUpdate()));
  #ifndef __MACX__
    m_helpMenu->addSeparator();
    //>>> this should quit the main app, really, but that would bomb
    m_helpMenu->addAction("&Quit",this,SLOT(close()),QKeySequence("Ctrl+Q"));
  #endif
    FullScreen(l.fullscreen);
    //>>> dock does not exist yet ShowDock  (l.showdock);
    Show(show);
  }


  QtVigoWindow_::~QtVigoWindow_()
  {
  }


  void QtVigoWindow_::closeEvent(QCloseEvent* ev)
  {
    g_closing_ = true; return; //>>>the dialog pops up repeatedly after "Cancel"
    if(vigo::Application::QuestionPopup("Do you really want to quit?"))
    {
      ev->accept();
      g_closing_ = true;
    }
    else
    {
      ev->ignore();
    }
  }


  void QtVigoWindow_::about()
  {
    QMessageBox::about(this,
      vigo::res::name.c_str(),vigo::res::about.c_str());
  }


  void QtVigoWindow_::checkUpdate()
  { //>>> move to vigo::Application
    cancelUpdate();
    m_progress = new QProgressDialog("Looking up version information...","Cancel",0,2);
    connect(m_progress,SIGNAL(canceled()),this,SLOT(cancelUpdate()));

    static pcstr  host     = "ard-galen.org";
    static ushort port     = 80;
    static pcstr  url      = "/software/get_version.php";

    static uint   thisVer  = vigo::res::version;
    static pcstr  sw       = "Vigo::3D";
    static vigo::util::HTTPSocket::timeout_t tout = 6000;

    std::string reply;
    bool isReply = true;
    try
    {
      m_progress->setValue(1);
      qApp->processEvents();
      std::string query = vigo::util::HTTPSocket::MakeQuery("sw",sw,NULL);
      reply = vigo::util::HTTPSocket::SendQuery(host,port,url,query.c_str(),tout);
    }
    catch(vigo::util::HTTPSocket::exc& e)
    {
      isReply = false;
    }

    cancelUpdate();

    std::string answer;

    if(!isReply)
    {
      answer = vigo::sformat("Cannot reach <%s>.\n Check your Internet connection.",host);
    }
    else
    {
      uint ver; char at[65];
      if(2==sscanf(reply.c_str(),vigo::sformat("%s: ver=%%lu at %%64s",sw).c_str(),&ver,at))
      {
        if(ver>thisVer)
          answer = vigo::sformat("There is a newer version available at <%s>",at);
        else
          answer = vigo::sformat("You have the latest version.\nHome page: <%s>",at);
      }
      else
      {
        answer = "Cannot retrieve information.\nTry again later.";
      }
    }

    QMessageBox::information(this,"Update",answer.c_str());
  }


  void QtVigoWindow_::cancelUpdate()
  {
    delete m_progress;
    m_progress = NULL;
  }


  void QtVigoWindow_::fullScreen(bool full)
  {
    if(full)
      showFullScreen();
    else
      showNormal();
  }


  void QtVigoWindow_::showDock(bool view)
  {
    m_showDock = view;
    if(m_dock) m_dock->Show(view);
  }


  void QtVigoWindow_::AddMenu(vigo::Menu* menu)
  {
    if(menu) menuBar()->addMenu(static_cast<QMenu*>(menu->Widget()));
  }


  void QtVigoWindow_::RemoveMenu(vigo::Menu* menu)
  {
    if(menu && !g_closing_ && menu)
      menuBar()->removeAction(static_cast<QMenu*>(menu->Widget())->menuAction());
  }


  void QtVigoWindow_::Polish() //>>> one-canvas solution
  {
    vigo::Canvas *cvs = m_iface.GetCanvas();

    vigo::Menu   *newCameraMenu     = cvs ? cvs->GetCameraMenu()     : NULL;
    vigo::Menu   *newSimulationMenu = cvs ? cvs->GetSimulationMenu() : NULL;

    if(newCameraMenu!=m_cameraMenu)
      RemoveMenu(m_cameraMenu);

    if(newSimulationMenu!=m_simulationMenu)
      RemoveMenu(m_simulationMenu);

  //>>>use insertMenu(before) to keep order?
    if(newCameraMenu!=m_cameraMenu)
    {
      m_cameraMenu = newCameraMenu;
      AddMenu(m_cameraMenu);
    }

    if(newSimulationMenu!=m_simulationMenu)
    {
      m_simulationMenu = newSimulationMenu;
      AddMenu(m_simulationMenu);
    }

    // dock
    vigo::control::Dock *newDock = cvs ? cvs->GetSimulationDock() : NULL;
    if(m_dock!=newDock)
    {
      if(m_dock)
      {
        m_dock->Show(false);
        ((QtVigoDock_*)(m_dock->Widget()))->setParent(NULL);
      }

      m_dock = newDock;

      if(m_dock)
      {
        m_dock->Show(false);
        ((QtVigoDock_*)(m_dock->Widget()))->setParent(this);
      }
    }
  }


  void QtVigoWindow_::Attach(vigo::Canvas& cvs,bool setWidget)
  {
    if(setWidget) setCentralWidget(static_cast<QWidget*>(cvs.Widget()));
  }


  void QtVigoWindow_::Show(bool b)
  {
    if(b) show(); else hide();
  }


  void QtVigoWindow_::FullScreen(bool b)
  {
    fullScreen(b);
  }


  bool QtVigoWindow_::IsFullScreen() const
  {
    return isFullScreen();
  }


  void QtVigoWindow_::ShowDock(bool b)
  {
    showDock(b);
  }


  bool QtVigoWindow_::IsDockShown() const
  {
    return m_showDock;
  }


  void QtVigoWindow_::Move(int x, int y)
  {
    move(x, y);
  }


  void QtVigoWindow_::Resize(int w, int h)
  {
    resize(w, h);
  }


  int QtVigoWindow_::X() const
  {
    return x();
  }


  int QtVigoWindow_::Y() const
  {
    return y();
  }


  int QtVigoWindow_::Width() const
  {
    return width();
  }


  int QtVigoWindow_::Height() const
  {
    return height();
  }


/*** eof ********************************************************************/
