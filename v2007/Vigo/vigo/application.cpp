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
* $Id: application.cpp 45 2006-06-21 10:20:33Z ian_b $
\*****************************************************************************/

  #include "./application.hpp"
  #include "./resource.hpp"
  #include "../QtVigo/application.hpp"
  #include <iostream>

NAMESPACE_VIGO_() //***********************************************************

  static Application *g_application = NULL;

  Application::Application(pcstr name,int argc,pstr argv[])
    : internal::pimpl<QtVigoApplication_>(new QtVigoApplication_(*this,name,argc,argv))
  {
    RUNTIME_CHECK_EXC2(!g_application,"You can have only one application object")
    g_application = this;

    std::cout << res::GPLnotice;
  }


  Application::~Application()
  {
    g_application = NULL;
  }


  Application& Application::Get()
  {
    RUNTIME_CHECK_EXC2(g_application,"An application object does not exists")
    return *g_application;
  }


  int Application::Exec()
  {
    return pImpl().Exec();
  }


#define PIMPL_FACE_CLASS Application
#define PIMPL_IMPL_CLASS QtVigoApplication_
#include "../common/pimpl.inc"

  PIMPL_IMPL(AddCmdlineSwitch,void, (pcstr t,bool& v,pcstr h), (t,v,h),)
  PIMPL_IMPL(ProcessArgs,   void, (), (),)
  PIMPL_IMPL(Exit,          void, (int code), (code),)
  PIMPL_IMPL(Help,          std::string, (), (),)

  PIMPL_IMPL(CfgFileName,   std::string const&, (), (), const)

  PIMPL_SIMPL(InfoPopup,    void, (pcstr msg), (msg))
  PIMPL_SIMPL(QuestionPopup,bool, (pcstr msg), (msg))
  PIMPL_SIMPL(ErrorPopup,   void, (pcstr msg), (msg))


NAMESPACE_END_() //** eof *****************************************************
