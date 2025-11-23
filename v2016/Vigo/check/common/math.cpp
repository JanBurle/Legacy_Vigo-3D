// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

NAMESPACE_END() // eof
