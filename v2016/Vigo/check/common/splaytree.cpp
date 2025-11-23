// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Unit tests for splaytree.hpp.
 *
 */

#include "../vigotest.hpp"
#include "../../common/splaytree.hpp"
#include <iostream>

  NAMESPACE_VIGO(vt_splaytree)

//----------------------------------------------------------------------------

  struct Int
  {
    Int(int i): m_i(i) {}
    operator int() const { return m_i;}

  private:
    int m_i;
  };

  class int_tree: public util::splaytree<Int,Int>
  {
  public:
    int Compare(Int const&l, Int const& r) const
    {
      if((int)l < (int)r) return -1;
      if((int)l > (int)r) return +1;
      return 0;
    }
  };

  template<class T>
  uint size(util::splaytree<T,T> const& tree)
  {
    uint s = 0;
    for(T const *t = tree.min(); t; ++s, t=tree.next())
      ;
    return s;
  }

  template<class T>
  bool ordered(util::splaytree<T,T> const& tree)
  {
    T const *last = tree.min(), *next = tree.next();
    while(next)
    {
      if(tree.Compare(*last,*next) >=0)
        return false;
      last = next; next = tree.next();
    }
    return true;
  }

VT_BEGIN(splay)

CPPUNIT_TEST_SUITE(VIGOTEST);
CPPUNIT_TEST(Construct);
CPPUNIT_TEST_SUITE_END();

public:
  void setUp()
  {
  }

  void tearDown()
  {
  }

  void Construct()
  {
    int_tree t;
    CPPUNIT_ASSERT(0==size(t));
    CPPUNIT_ASSERT(t.empty());
    t.insert(3);
    CPPUNIT_ASSERT(ordered(t));
    CPPUNIT_ASSERT(1==size(t));
    CPPUNIT_ASSERT(!t.empty());
    CPPUNIT_ASSERT(t.insert(-3));
    CPPUNIT_ASSERT(t.insert(32));
    CPPUNIT_ASSERT(t.insert(13));
    CPPUNIT_ASSERT(t.insert(333));
    CPPUNIT_ASSERT(ordered(t));
    CPPUNIT_ASSERT(5==size(t));
    CPPUNIT_ASSERT(!t.insert(13));
    CPPUNIT_ASSERT(t.insert(-13));
    CPPUNIT_ASSERT(t.insert(132));
    CPPUNIT_ASSERT(t.insert(113));
    CPPUNIT_ASSERT(t.insert(3133));
    CPPUNIT_ASSERT(ordered(t));
    CPPUNIT_ASSERT(9==size(t));
    CPPUNIT_ASSERT(t.find(13) && 13==*t.find(13));
    CPPUNIT_ASSERT(t.remove(13));
    CPPUNIT_ASSERT(!t.remove(13));
    CPPUNIT_ASSERT(!t.find(13));
    CPPUNIT_ASSERT(t.findGE(13) && 32==*t.findGE(13));
    t.erase();
    CPPUNIT_ASSERT(0==size(t));
    CPPUNIT_ASSERT(t.empty());
  }

VT_END()

//----------------------------------------------------------------------------

#if RUN_ALL_TESTS || RUN_COMMON_TESTS
  VT_REGISTER(splay)
#endif

NAMESPACE_END() // eof

