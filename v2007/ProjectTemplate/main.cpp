/*****************************************************************************\
*    Copyright (c) 2006 by ...........                 .....@cpsc.ucalgary.ca *                                                                       *
*-----------------------------------------------------------------------------*
* $Id: $
\*****************************************************************************/

/** \file  
 *
 */

  #include <vigoapp>
  #include <iostream>

  #include "params.hpp"

  #include "app.hpp"
  #include "sim.hpp"
  #include "scene.hpp"

//----------------------------------------------------------------------------

  #define APPNAME "Template"
  
  int main(int argc, pstr argv[])
  {
    try
    {
      Application app(APPNAME,argc,argv);      
      
      try
      { 
        app.ProcessArgs(); 
      }
      catch(vigo::exc& e)
      {
        std::cerr << e.What()   << std::endl;
        std::cout << app.Help() << std::endl;
        return -1;
      }

      params::Load(app.CfgFileName().c_str());
      params::Print(std::cout);

      Sim   sim;
      Scene scene;
      sim.SetScene(scene);

      vigo::Window wnd(APPNAME);
      vigo::Canvas cvs(wnd);
      
      vigo::Camera cam(cvs,sim);
      
      cam.AddLight(vigo::Camera::light(vigo::shape::ColorAlpha(254/15,254/15,166/15,255),
                                       vigo::shape::ColorAlpha(254,254,166,255),
                                       Point(10.0,0.0001,0.0),false));
      sim.Start();
      return app.Exec();
    }
    catch(vigo::exc& e)
    {
      std::cerr << e.What() << std::endl;
      return -1;
    }
  }

/*** eof ********************************************************************/
