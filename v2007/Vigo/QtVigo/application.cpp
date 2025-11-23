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
* $Id: application.cpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "./application.hpp"
  #include <QtGui/QMessageBox>
  #include <QtGui/QCloseEvent>
  #include <QtOpenGL/QGLFormat>

//-----------------------------------------------------------------------------

  static QtVigoApplication_ *g_application_ = NULL;

  QtVigoApplication_::QtVigoApplication_(vigo::Application& iface,
                                         pcstr name,int argc,pstr argv[])
    : vigo::internal::pimpl_iface<vigo::Application>(iface),
      m_argc(argc), m_argv(argv),
      m_windowLayout(), m_switches(),
      m_name(name?name:""), m_cfgFileName(m_name+".cfg")/*>>>directory, if not specified, as ./*/,
      m_qApp(m_argc,m_argv)
  {
    if(g_application_)
      vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                "You can have only one application object"));
    if(!QGLFormat::hasOpenGL())
      vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                "This system does not support OpenGL"));
    //if(!QGLFormat::hasOpenGLOverlays())
    //  vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
    //                            "This system has no OpenGL overlays"));
    g_application_ = this;

    AddSwitch(sSwitch("--fullscreen",&m_windowLayout.fullscreen,
                      "make window fullscreen"));
    AddSwitch(sSwitch("--dock",      &m_windowLayout.showdock,
                     "show dock"));

    if(!m_qApp.connect(&m_qApp,SIGNAL(lastWindowClosed()),&m_qApp,SLOT(quit())))
      vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                "connect SLOT(quit()) failed"));
  }


  QtVigoApplication_::~QtVigoApplication_()
  {
    g_application_ = NULL;
  }


  QtVigoApplication_& QtVigoApplication_::Get()
  {
    if(!g_application_)
      vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                "An application object does not exists"));
    return *g_application_;
  }


  int QtVigoApplication_::Exec()
  {
    return m_qApp.exec();
  }


  void QtVigoApplication_::Exit(int code)
  {
  /* Something fishy going on. Must send a close event first, without it my
     Windows/Canvasses crash. */
    QCloseEvent ev;
    QApplication::sendEvent(&m_qApp,&ev);
  // `exit` is needed to set the exit code
    m_qApp.exit(code);
  }


  void QtVigoApplication_::InfoPopup(pcstr msg)
  {
    QMessageBox::information(NULL, "Information", msg, QMessageBox::Ok);
  }


  bool QtVigoApplication_::QuestionPopup(pcstr msg)
  {
    return QMessageBox::Ok ==
      QMessageBox::question(NULL, "Question", msg, QMessageBox::Cancel,
                                                   QMessageBox::Ok);
  }


  void QtVigoApplication_::ErrorPopup(pcstr msg)
  {
    QMessageBox::warning(NULL, "Error", msg, QMessageBox::Ok,
                                             QMessageBox::NoButton);
  }

  std::string QtVigoApplication_::ArgStr(int& i, uint offset)
  {
    std::string value = std::string(m_argv[i]).substr(offset);
    if(value.empty())
    {
      // do expect the value as a separate token
      if(++i>=m_argc)
        vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                  "Expecting another command-line argument"));
      value = m_argv[i];
    }

    return value;
  }


  void QtVigoApplication_::ProcessArgs()
  {
    enum {coordX = 0x01, coordY = 0x02, coordW = 0x04, coordH = 0x08,
          coordNONE = 0, coordALL = coordX|coordY|coordW|coordH};
    // the low 4 bits are flags, set when a window coordinate is specified
    // at the end, must be either 0 (none), or 0xF (all)
    uint8 specifiedCoord = 0;

    // all arguments
    for(int i=1; i<m_argc; ++i)
    {
      std::string argvi = m_argv[i];

      // values
      if("-f"==argvi.substr(0,2))
      {
        m_cfgFileName = ArgStr(i,2);
      }
      else if("-X"==argvi.substr(0,2))
      {
        m_windowLayout.x = ArgInt(i,2);
        specifiedCoord |= coordX;
      }
      else if("-Y"==argvi.substr(0,2))
      {
        m_windowLayout.y = ArgInt(i,2);
        specifiedCoord |= coordY;
      }
      else if("-W"==argvi.substr(0,2))
      {
        m_windowLayout.w = ArgInt(i,2);
        specifiedCoord |= coordW;
      }
      else if("-H"==argvi.substr(0,2))
      {
        m_windowLayout.h = ArgInt(i,2);
        specifiedCoord |= coordH;
      }
      else  // must be a switch
      {
        bool enable   = true; // the default is '+'
        uint lgt      = argvi.length();
        char lastChar = argvi[lgt-1];

        if('-'==lastChar)
        {
          enable = false;
          argvi  = argvi.substr(0,lgt-1);
        }
        else if('+'==lastChar)
        {
          enable = true;
          argvi  = argvi.substr(0,lgt-1);
        }

        for(switches_t::const_iterator i=m_switches.begin(); ; ++i)
        {
          sSwitch const &s = *i;
          if(i==m_switches.end())  // reached end, unknown switch
            vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                      "unknown command-line argument"));
          if(argvi==s.tag)
          {
            *s.pValue = enable;
            break;
          }
        }
      }
    }

    // check if window coordinates are valid
    if(coordNONE!=specifiedCoord && coordALL!=specifiedCoord)
      vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                "incomplete window coordinates"));
    if(coordALL==specifiedCoord) // check validity
    {
      if(m_windowLayout.w<=0 || m_windowLayout.h<=0)
      vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                "Negative window size"));
    }
  }


  void QtVigoApplication_::AddCmdlineSwitch(
      pcstr tag, bool& value, pcstr helpText)
  {
    AddSwitch(sSwitch(tag,&value,helpText));
  }

  std::string QtVigoApplication_::Help()
  {
    std::string help =
      vigo::sformat(
        "usage: %s\n"
        "  [-f <file>]                 // configuration file\n"
        "  [-X## -Y## -W## -H##]       // window position and size\n",
        m_argv[0]);

    for(switches_t::const_iterator i=m_switches.begin();
        i!=m_switches.end(); ++i)
    {
      sSwitch const &sw = *i;

      std::string s = vigo::sformat("  [%s[+|-]]",sw.tag.c_str());
      s = vigo::sformat("%-30s// %s\n",s.c_str(),sw.helpText.c_str());

      help += s;
    }

    return help;
  }


/*** eof ********************************************************************/
