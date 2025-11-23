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
* $Id: refcnt.cpp 30 2006-04-06 19:13:28Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "./refcnt.hpp"

NAMESPACE_VIGO(util) //========================================================

  thread::tls& storage_base_HeapPending()
  {
    // must be done this way: ensure m_heapPending is constructed before it is
    // used while constructing global static :refcnt objects
    static thread::tls m_heapPending;
    return m_heapPending;
  }

//----------------------------------------------------------------------------

  refptr_base::refptr_base() : m_ptr(NULL)
  {
  }

  refptr_base::refptr_base(refptr_base const& r): m_ptr(r.m_ptr)
  {
    if(m_ptr)
      m_ptr->inc_refcnt();
  }

  refptr_base::refptr_base(refcnt* ptr): m_ptr(ptr)
  {
    if(m_ptr)
      m_ptr->inc_refcnt();
  }

  refptr_base& refptr_base::operator=(refptr_base const& r)
  {
    if(m_ptr!=r.m_ptr)
    {
      if(m_ptr)
        m_ptr->dec_refcnt();
      m_ptr = r.m_ptr;
      if(m_ptr)
        m_ptr->inc_refcnt();
    }

    return *this;
  }

  refptr_base& refptr_base::operator=(refcnt* ptr)
  {
    release();

    m_ptr = ptr;
    if(m_ptr)
      m_ptr->inc_refcnt();

    return *this;
  }

  void refptr_base::release()
  {
    if(m_ptr)
      m_ptr->dec_refcnt();

    m_ptr = NULL;
  }

  void refptr_base::swap(refptr_base& r)
  {
    vigo::swap(m_ptr,r.m_ptr);
  }

NAMESPACE_END() //** eof ******************************************************
