// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Unit tests for tuple.hpp.
 *
 */

#include "../vigotest.hpp"
#include "../../common/tuple.hpp"

NAMESPACE_VIGO(vt_tuple)

using util::int1d;
using util::int2d;
using util::int3d;
using util::int4d;

//----------------------------------------------------------------------------

VT_BEGIN(tupleXd)

CPPUNIT_TEST_SUITE(VIGOTEST);
CPPUNIT_TEST(v1d);
CPPUNIT_TEST(i1d);
CPPUNIT_TEST(i4d);
CPPUNIT_TEST_SUITE_END();

private:

public:
  void setUp()
  {
  }

  void tearDown()
  {
  }

  void v1d()
  {
  #ifdef _DEBUG
    int1d v1(37), v2(v1);
    CPPUNIT_ASSERT(37 == v1.x);
    CPPUNIT_ASSERT(v2.x == v1.x);
    CPPUNIT_ASSERT(v1[0] == v1.x);

    int1d v100(100);
    CPPUNIT_ASSERT(v100>v2);

    bool pass = false;
    try
    {
      int v = v1[1];
      v++;
    }
    catch(int1d::exc& e)
    {
      if(int1d::exc::OUT_OF_BOUNDS == e.Id())
        pass = true;
    }

    CPPUNIT_ASSERT(pass);
  #endif
  }

  void i1d() // indexing
  {
    int1d v1(12345);
    CPPUNIT_ASSERT_EQUAL(12345,v1[0]);
  }

  void i4d() // indexing
  {
    int1d a; int2d b; int3d c;
    int4d v4(1,2,3,4);
    CPPUNIT_ASSERT_EQUAL(1,v4[0]);
    CPPUNIT_ASSERT_EQUAL(2,v4[1]);
    CPPUNIT_ASSERT_EQUAL(3,v4[2]);
    CPPUNIT_ASSERT_EQUAL(4,v4[3]);
  }

VT_END()

//>>> more tests for 2d, 3d, 4d

//----------------------------------------------------------------------------

#if RUN_ALL_TESTS || RUN_COMMON_TESTS
  VT_REGISTER(tupleXd)
#endif

NAMESPACE_END() // eof
