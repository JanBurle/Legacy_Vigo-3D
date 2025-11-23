// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *
 *  Tonnetz
 */

#ifndef TONNETZ_APP_HPP_INCLUDED
#define TONNETZ_APP_HPP_INCLUDED

  #include <vigoapp>
  #include "osclink.hpp"
  #include "params.hpp"

//----------------------------------------------------------------------------

  class Application: public vigo::Application
  {
    SUPER(vigo::Application)

  public:
    Application(pcstr name, int argc, pstr argv[])
      : super(name,argc,argv), m_scLink(NULL)
    {
    }

   ~Application()
    {
      delete m_scLink;
    }

    static Application& Get()  { return (Application&)super::Get(); }
    static SCLink&      SC()   { return *(Get().m_scLink);          }

    void InitSCLink()
    {
      m_scLink =
        new SCLink(params::APP::port, params::SC::host.c_str(), params::SC::port);
    }

  private:
    SCLink *m_scLink;
  };

//-----------------------------------------------------------------------------

#endif

// eof
