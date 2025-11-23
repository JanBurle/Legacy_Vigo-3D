/*****************************************************************************\
* This file is a part of Vigo::3D, a multi-agent simulation and visualization *
* system.                                                                     *
*                                                                             *
* Copyright (c) 2002--2006 by Ian G. Burleigh       burleigh@cpsc.ucalgary.ca *
* burleigh@cpsc.ucalgary.ca                         http://ard-galen.org/vigo *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for   *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the                                     *
*   Free Software Foundation, Inc.,                                           *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
*-----------------------------------------------------------------------------*
* $Id: resource.cpp 228 2007-05-21 17:47:02Z ian_b $
\*****************************************************************************/

  #include "./resource.hpp"

NAMESPACE_VIGO(res) //=========================================================

  std::string const source_host    = "vigo3d.net";

  #define MAJOR 1
  #define MINOR 1

  std::string const name           = "Vigo::3D";
  uint        const version        = MAJOR*100 + MINOR;
  std::string const version_string = sformat("%u.%uGPL",MAJOR,MINOR);
  std::string const name_version   = name + " ver." + version_string;
  std::string const url            = "http://vigo3d.net";
  std::string const copyright      = "Copyright (c) 2004-2007 Ian G Burleigh";
  std::string const statusCopyright= sformat("%s v%s",
                                       name.c_str(),version_string.c_str());
  std::string const licence        = "GNU General Public License (GPL)";
  std::string const about          = sformat("| %s\n| %s\n| %s\n| http://%s",
                                             name_version.c_str(),
                                             copyright.c_str(),
                                             licence.c_str(),source_host.c_str());

  std::string const GPLnotice      = name_version
                                   + " http://"+source_host+"\n"
                                   + copyright +
    "\nVigo is free software, covered by the GNU General Public Licence.\n"
    "There is absolutely no warranty for Vigo.\n\n";

  std::string const& VigoText()
  {
	const time_t period = 12, urlTime = 3;

	time_t t = (time(NULL) % period);
	return (t < urlTime) ? url : statusCopyright;
  }

NAMESPACE_END() //** eof ******************************************************
