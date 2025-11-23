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
* $Id: pimpl.inc 42 2006-06-20 08:06:06Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief The "pointer to implementation" pattern.
 *
 */

/** \def PIMPL_IMPL
 * Links a method to its implementation.
 */
#define PIMPL_IMPL(name,type,argsin,argspass,constQ) \
  type PIMPL_FACE_CLASS::name argsin constQ \
  { return reinterpret_cast<PIMPL_IMPL_CLASS*>(m_pImpl)->name argspass; }

/** \def PIMPL_SIMPL
 * Links a static method to its implementation.
 */
#define PIMPL_SIMPL(name,type,argsin,argspass) \
  type PIMPL_FACE_CLASS::name argsin \
  { return PIMPL_IMPL_CLASS::name argspass; }

//** eof **********************************************************************
