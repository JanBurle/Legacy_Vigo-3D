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
* $Id: pimpl.hpp 30 2006-04-06 19:13:28Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief The "pointer to implementation" pattern.
 *
 */

#ifndef VIGO_COMMON_PIMPL_HPP_INCLUDED
#define VIGO_COMMON_PIMPL_HPP_INCLUDED

  #include "./types.hpp"

NAMESPACE_VIGO(internal) //====================================================
// These classes are for internal use.

  // The pImpl pattern.
  template <class Impl>
  class pimpl
  {
  public:
    pimpl(Impl* p=NULL): m_pImpl(p) { }
    virtual ~pimpl()                { delete m_pImpl; }

    void pImpl(Impl* impl)  { m_pImpl = impl;                    }
    void pImpl(Impl& impl)  { m_pImpl = &impl;                   }
    Impl& pImpl()           { assert(m_pImpl); return *m_pImpl;  }

  protected:
    Impl* m_pImpl;
  };

  // The backlink to the interface object.
  template <class Interface>
  class pimpl_iface
  {
  public:
    pimpl_iface(Interface& iface): m_iface(iface) { }

  protected:
    Interface &m_iface;
  };


NAMESPACE_END() //=============================================================

#endif

//** eof **********************************************************************
