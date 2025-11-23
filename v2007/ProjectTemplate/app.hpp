/*****************************************************************************\
*    Copyright (c) 2006 by ...........                 .....@cpsc.ucalgary.ca *                                                                       *
*-----------------------------------------------------------------------------*
* $Id: $
\*****************************************************************************/

/** \file  
 *
 */

#ifndef VIGOPROJECT_APP_HPP_INCLUDED
#define VIGOPROJECT_APP_HPP_INCLUDED

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

//** eof **********************************************************************
