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
* $Id: refcnt.cpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Unit tests for refcnt.hpp.
 *
 */
 
#include "../vigotest.hpp"
#include "../../common/refcnt.hpp"

  NAMESPACE_VIGO(vt_refcnt)

//----------------------------------------------------------------------------

VT_BEGIN(Refcnt)

CPPUNIT_TEST_SUITE(VIGOTEST);
CPPUNIT_TEST(StorageAuto);
CPPUNIT_TEST(StorageStack);
CPPUNIT_TEST(StorageHeap);
CPPUNIT_TEST_SUITE_END();

private:

public:
  void setUp()
  {
  }

  void tearDown()
  {
  }
    
  class S {};

  void StorageAuto()
  {
    class SType: public vigo::util::storage_auto<S>
    {
    };

    SType a1, a2,  *h1 = new SType, *h2 = new SType;
    SType a3(*h1), *h3 = new SType(a2);
  
    CPPUNIT_ASSERT(!a1.IsOnHeap());
    CPPUNIT_ASSERT(!a2.IsOnHeap());
    CPPUNIT_ASSERT(!a3.IsOnHeap());
    CPPUNIT_ASSERT(h1->IsOnHeap());
    CPPUNIT_ASSERT(h2->IsOnHeap());
    CPPUNIT_ASSERT(h3->IsOnHeap());
 
    a2 = *h1; *h2 = a1;
  
    CPPUNIT_ASSERT(!a2.IsOnHeap());
    CPPUNIT_ASSERT(h2->IsOnHeap());
  }
  
  void StorageStack()
  {
    class SType: public vigo::util::storage_stack<S>
    {
    };

    SType s;
    CPPUNIT_ASSERT(!s.IsOnHeap());
  }
  
  void StorageHeap()
  {
    class SType: public vigo::util::storage_heap<S>
    {
      STORAGE_HEAP(SType)
    };

    SType *h = new SType;
  
    CPPUNIT_ASSERT(h->IsOnHeap());
  }
VT_END()

//----------------------------------------------------------------------------

#if RUN_ALL_TESTS || RUN_COMMON_TESTS
  VT_REGISTER(Refcnt)
#endif

NAMESPACE_END() //** eof ******************************************************
