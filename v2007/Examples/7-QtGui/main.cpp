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
* $Id: main.cpp 192 2007-01-13 03:54:25Z ian_b $
\*****************************************************************************/

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

//** eof **********************************************************************
