// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include <vigoapp>
  #include <iostream>

  #include "qtwindow.hpp"
  #include "sim.hpp"

//-----------------------------------------------------------------------------

  int main(int argc, pstr argv[])
  {
    try
    {
      vigo::Application app("QtGui",argc,argv);

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

      QtGuiWindow wnd("My Window");
      vigo::Canvas cvs(wnd,false); // do not set the central widget ...
      // ...we'll do it here:
      wnd.CreateLayout();

      Simulation sim;
      Scene scene;
      sim.SetScene(scene);

      vigo::Camera cam;
      cam.Attach(cvs); cam.AttachTo(sim);

      wnd.Connect(cvs,cam,sim);

      sim.Start();
      return app.Exec();
    }
    catch(vigo::exc& e)
    {
      std::cerr << e.What() << std::endl;
      return -1;
    }
  }

// eof
