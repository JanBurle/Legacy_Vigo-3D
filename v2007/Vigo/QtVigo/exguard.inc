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
* $Id: exguard.inc 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 *  Guards exceptions from propagating to Qt slots
 */

// named .inc rather than .hpp to escape moc compilation
#ifndef VIGO_QT_EXGUARD_INC_INCLUDED
#define VIGO_QT_EXGUARD_INC_INCLUDED

  #include "../common/types.hpp"
  #include "../vigo/application.hpp"

  #define EXGUARD_BEGIN() \
    try {

  #define EXGUARD_END() \
    } catch(vigo::exc& e) { \
      vigo::Application &app = vigo::Application::Get(); \
      app.ErrorPopup(e.What()); \
      throw; \
    }

#endif

//** eof **********************************************************************
