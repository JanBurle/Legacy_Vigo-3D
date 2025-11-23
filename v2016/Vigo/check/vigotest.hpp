// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

// eof
