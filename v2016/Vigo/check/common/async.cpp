// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

NAMESPACE_END() // eof
