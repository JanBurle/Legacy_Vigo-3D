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
* $Id:vigotest.hpp 30 2006-04-06 19:13:28Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief CppUnit tests macros
 *
 */

#ifndef UNITTESTS_VIGOTEST_HPP_INCLUDED
#define UNITTESTS_VIGOTEST_HPP_INCLUDED

  #include "../common/types.hpp"
  #include <cppunit/TestCase.h>
  #include <cppunit/extensions/HelperMacros.h>

  #define RUN_ALL_TESTS         1
  #define RUN_COMMON_TESTS      1
  #define RUN_OSC_TESTS         1

NAMESPACE_VIGO(vigotest) //====================================================

#define VT_NAME(name) name##Test
#define VT_BEGIN(name)                              \
  class VT_NAME(name): public CppUnit::TestFixture  \
  {                                                 \
  public:                                           \
    typedef VT_NAME(name) VIGOTEST;

#define VT_END()                                    \
  };

#define VT_REGISTER(name)                           \
  NAMESPACE_VIGO(vigotest) CPPUNIT_TEST_SUITE_REGISTRATION(VT_NAME(name)); NAMESPACE_END()

NAMESPACE_END() //=============================================================

#endif

//** eof **********************************************************************
