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
* $Id: main.cpp 46 2006-06-21 10:33:26Z ian_b $
\*****************************************************************************/

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

/*** eof ********************************************************************/
