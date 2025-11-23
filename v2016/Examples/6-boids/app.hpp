// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *
 */

#ifndef BOIDS_APP_HPP_INCLUDED
#define BOIDS_APP_HPP_INCLUDED

  #include <vigoapp>
  #include "params.hpp"

//----------------------------------------------------------------------------

  class Application: public vigo::Application
  {
    SUPER(vigo::Application)

  public:
    Application(pcstr name, int argc, pstr argv[])
      : super(name,argc,argv)
    {
    }

   ~Application()
    {
    }

    static Application& Get()  { return (Application&)super::Get(); }
  };

//-----------------------------------------------------------------------------

#endif

// eof
