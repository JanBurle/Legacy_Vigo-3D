// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Unit tests for socket.hpp.
 *
 */

#include "../vigotest.hpp"
#include "../../common/socket.hpp"

  NAMESPACE_VIGO(vt_socket)

//----------------------------------------------------------------------------

VT_BEGIN(Socket)

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
  VT_REGISTER(Socket)
#endif

NAMESPACE_END() // eof
