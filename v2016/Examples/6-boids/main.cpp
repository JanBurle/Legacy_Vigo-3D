// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *
 * This example implements C.Reynolds' boids. It simulates the physics of
 * heavier-than-air flight, flocking behaviour, and obstacle avooidance.
 *
 * The scene is intentionally constructed in single (float) precision, to
 * test Vigo code. Most of simulations use double precision.
 */

  #include <vigoapp>
  #include <iostream>

  #include "params.hpp"

  #include "app.hpp"
  #include "sim.hpp"
  #include "scene.hpp"

//----------------------------------------------------------------------------

  #define APPNAME "boids"

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

      cam.AddLight(
        vigo::Camera::light(scene.par_camera.lightAmbColor,
                            scene.par_camera.lightDiffColor,
                            scene.par_camera.lightPosition,true));

      wnd.ShowDock(true);
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
