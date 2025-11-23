/*****************************************************************************\
*    Copyright (c) 2006 by Ian G Burleigh           burleigh@cpsc.ucalgary.ca *
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
* $Id: main.cpp 47 2006-06-21 10:34:49Z ian_b $
\*****************************************************************************/

/** \file  
 *
 *  Tonnetz
 */

  #include <vigoapp>

  #include "params.hpp"
  #include <iostream>

  #include "app.hpp"
  #include "tsim.hpp"
  #include "msim.hpp"
  #include "tscene.hpp"
  #include "mscene.hpp"
  #include "osclink.hpp"

//----------------------------------------------------------------------------

  #define APPNAME "Tonnetz"
  
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
      app.InitSCLink();

      MoebiusSim   sim;
      MoebiusScene scene;
      sim.SetScene(scene);

      vigo::Window wnd1(APPNAME " (Moebius 1)"),
                   wnd2(APPNAME " (Moebius 2)");
      vigo::Canvas cvs1(wnd1), cvs2(wnd2);
      
      vigo::Camera cam1(cvs1,sim), cam2(cvs2,sim);
      
      cam1.AddLight(vigo::Camera::light(vigo::shape::ColorAlpha(254/15,254/15,166/15,255),
                                        vigo::shape::ColorAlpha(254,254,166,255),
                                        Pointd(10.0,0.0001,0.0),false));
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
