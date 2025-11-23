// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Unit tests for array.hpp.
 *
 */

#include "../vigotest.hpp"
#include "../../common/array.hpp"

  NAMESPACE_VIGO(vt_array)

//----------------------------------------------------------------------------

VT_BEGIN(arrayXd)

CPPUNIT_TEST_SUITE(VIGOTEST);
CPPUNIT_TEST(a1d);
CPPUNIT_TEST(Construct);
CPPUNIT_TEST_SUITE_END();

private:
  typedef util::array1d<int> ia;

  template<class T>
  class Value
  {
  public:
    Value(T value=0): m_value(value)
    {
    }

    operator T() const
    {
      return m_value;
    }

    private:
      T m_value;
    };

public:
  void setUp()
  {
  }

  void tearDown()
  {
  }

  void a1d()
  {
    uint const SIZE = 43;
    ia a1(SIZE);

    CPPUNIT_ASSERT(SIZE == a1.size());
  }

  void Construct()
  {
    uint const SIZE1 = 13, SIZE2 = 17, SIZE3 = 4,
               VALUE = 23489,
               SIZE  = SIZE1*SIZE2*SIZE3;
    uint i,j,k;
    int  v;

    util::array1d<Value<int> > a(SIZE,VALUE);
    CPPUNIT_ASSERT_EQUAL(SIZE,a.size());

    for(i=0; i<SIZE; ++i)
    {
      CPPUNIT_ASSERT(VALUE == a[i]);
    }

    util::array3d<Value<int> > b(SIZE1,SIZE2,SIZE3,VALUE);
    CPPUNIT_ASSERT_EQUAL(SIZE,b.size());

    for(i=0; i<SIZE; ++i)
    {
      CPPUNIT_ASSERT(b[i] == a[i]);
    }

    v = 0;
    for(i=0; i<SIZE1; ++i)
    for(j=0; j<SIZE2; ++j)
    for(k=0; k<SIZE3; ++k)
    {
      b(i,j,k) = v++;
    }

    v = 0;
    for(i=0; i<SIZE; i++)
    {
      CPPUNIT_ASSERT(v++ == b[i]);
    }
  }

VT_END()

//----------------------------------------------------------------------------

#if RUN_ALL_TESTS || RUN_COMMON_TESTS
  VT_REGISTER(arrayXd)
#endif

NAMESPACE_END() // eof

