// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

NAMESPACE_END() // eof
