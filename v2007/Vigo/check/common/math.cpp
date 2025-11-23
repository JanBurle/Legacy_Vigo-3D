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
* $Id: math.cpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Unit tests for math.hpp.
 *
 */
 
#include "../vigotest.hpp"
#include "../../common/math.hpp"
#include <iostream>

  NAMESPACE_VIGO(vt_math)

//----------------------------------------------------------------------------

VT_BEGIN(Math)

CPPUNIT_TEST_SUITE(VIGOTEST);
CPPUNIT_TEST(Rand);
CPPUNIT_TEST_SUITE_END();

private:

public:
  void setUp()
  {
  }

  void tearDown()
  {
  }
  
  void Rand()
  {
    uint32 init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
    math::Rand r(init,length);
    for(uint i=0; i<80; ++i)
    {
      std::cout << r.UInt32(0,2);
    }
  }
    
VT_END()

//----------------------------------------------------------------------------

#if RUN_ALL_TESTS || RUN_COMMON_TESTS
  VT_REGISTER(Math)
#endif

NAMESPACE_END() //** eof ******************************************************
