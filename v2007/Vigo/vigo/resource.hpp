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
* $Id: resource.hpp 196 2007-01-14 20:34:29Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Resources
 *
 */

#ifndef VIGO_VIGO_RESOURCE_HPP_INCLUDED
#define VIGO_VIGO_RESOURCE_HPP_INCLUDED

  #include "../common/types.hpp"
  #include <string>

NAMESPACE_VIGO(res) //=========================================================
/** \defgroup grp_resource_hpp Resources
@{*/
  
  extern std::string const name;
  extern uint        const version;
  extern std::string const version_string;
  extern std::string const name_version;
  extern std::string const url;
  extern std::string const copyright;
  extern std::string const statusCopyright;
  extern std::string const licence;
  extern std::string const about;
  
  extern std::string const GPLnotice;

  std::string const& VigoText();

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

//** eof **********************************************************************
