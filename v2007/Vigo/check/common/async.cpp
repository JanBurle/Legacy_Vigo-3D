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
* $Id: async.cpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Unit tests for async.hpp.
 *
 */
 
#include "../vigotest.hpp"
#include "../../common/async.hpp"

  NAMESPACE_VIGO(vt_async)

//----------------------------------------------------------------------------

VT_BEGIN(Async)

CPPUNIT_TEST_SUITE(VIGOTEST);
CPPUNIT_TEST_SUITE_END();

private:

public:
  void setUp()
  {
  }

  void tearDown()
  {
  }
    
VT_END()

//----------------------------------------------------------------------------

#if RUN_ALL_TESTS || RUN_COMMON_TESTS
  VT_REGISTER(Async)
#endif

NAMESPACE_END() //** eof ******************************************************
