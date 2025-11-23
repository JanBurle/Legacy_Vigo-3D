// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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


NAMESPACE_END_() // eof
