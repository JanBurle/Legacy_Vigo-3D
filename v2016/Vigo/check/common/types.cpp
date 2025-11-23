// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Unit tests for types.h|hpp|cpp.
 *
 */

#include "../vigotest.hpp"
#include "../../common/types.h"
#include "../../common/types.hpp"
#include "../../common/ogl.hpp"

NAMESPACE_VIGO(vt_types)

//----------------------------------------------------------------------------

VT_BEGIN(PrimitiveTypes)

CPPUNIT_TEST_SUITE(VIGOTEST);
CPPUNIT_TEST(AssumedSizes);
CPPUNIT_TEST(AssumedLimits);
CPPUNIT_TEST(Other);
CPPUNIT_TEST_SUITE_END();

private:
  // data here

public:
  void setUp()
  {
  }

  void tearDown()
  {
  }

  void AssumedSizes() // assumptions about machine types
  {
    CPPUNIT_ASSERT(8 == CHAR_BIT);
    CPPUNIT_ASSERT(1 == sizeof(int8));
    CPPUNIT_ASSERT(1 == sizeof(uint8));
    CPPUNIT_ASSERT(2 == sizeof(int16));
    CPPUNIT_ASSERT(2 == sizeof(uint16));
    CPPUNIT_ASSERT(4 == sizeof(int32));
    CPPUNIT_ASSERT(4 == sizeof(uint32));
    CPPUNIT_ASSERT(8 == sizeof(int64));
    CPPUNIT_ASSERT(8 == sizeof(uint64));
    CPPUNIT_ASSERT(sizeof(uint) == sizeof(GLuint)); // used in vigo::shape::Texture
  }

  void AssumedLimits() // assumptions about machine types
  {
    int8 int8min = MIN_INT8, int8max = MAX_INT8;
    int8 i8min = int8min; --i8min;
    int8 i8max = int8max; ++i8max;
    CPPUNIT_ASSERT(int8min < int8max);
    CPPUNIT_ASSERT(i8min == int8max); // underflow
    CPPUNIT_ASSERT(i8max == int8min); // overflow

    int16 int16min = MIN_INT16, int16max = MAX_INT16;
    int16 i16min = int16min; --i16min;
    int16 i16max = int16max; ++i16max;
    CPPUNIT_ASSERT(int16min < int16max);
    CPPUNIT_ASSERT(i16min == int16max); // underflow
    CPPUNIT_ASSERT(i16max == int16min); // overflow

    int32 int32min = MIN_INT32, int32max = MAX_INT32;
    int32 i32min = int32min; --i32min;
    int32 i32max = int32max; ++i32max;
    CPPUNIT_ASSERT(int32min < int32max);
    CPPUNIT_ASSERT(i32min == int32max); // underflow
    CPPUNIT_ASSERT(i32max == int32min); // overflow

    int64 int64min = MIN_INT64, int64max = MAX_INT64;
    int64 i64min = int64min; --i64min;
    int64 i64max = int64max; ++i64max;
    CPPUNIT_ASSERT(int64min < int64max);
    CPPUNIT_ASSERT(i64min == int64max); // underflow
    CPPUNIT_ASSERT(i64max == int64min); // overflow

    uint8 uint8min = MIN_UINT8, uint8max = MAX_UINT8;
    uint8 ui8min = uint8min; --ui8min;
    uint8 ui8max = uint8max; ++ui8max;
    CPPUNIT_ASSERT(uint8min < uint8max);
    CPPUNIT_ASSERT(ui8min == uint8max); // underflow
    CPPUNIT_ASSERT(ui8max == uint8min); // overflow

    uint16 uint16min = MIN_UINT16, uint16max = MAX_UINT16;
    uint16 ui16min = uint16min; --ui16min;
    uint16 ui16max = uint16max; ++ui16max;
    CPPUNIT_ASSERT(uint16min < uint16max);
    CPPUNIT_ASSERT(ui16min == uint16max); // underflow
    CPPUNIT_ASSERT(ui16max == uint16min); // overflow

    uint32 uint32min = MIN_UINT32, uint32max = MAX_UINT32;
    uint32 ui32min = uint32min; --ui32min;
    uint32 ui32max = uint32max; ++ui32max;
    CPPUNIT_ASSERT(uint32min < uint32max);
    CPPUNIT_ASSERT(ui32min == uint32max); // underflow
    CPPUNIT_ASSERT(ui32max == uint32min); // overflow

    uint64 uint64min = MIN_UINT64, uint64max = MAX_UINT64;
    uint64 ui64min = uint64min; --ui64min;
    uint64 ui64max = uint64max; ++ui64max;
    CPPUNIT_ASSERT(uint64min < uint64max);
    CPPUNIT_ASSERT(ui64min == uint64max); // underflow
    CPPUNIT_ASSERT(ui64max == uint64min); // overflow
  }

  void Other()
  {
    CPPUNIT_ASSERT_EQUAL(empty(NULL), true);
    CPPUNIT_ASSERT_EQUAL(empty(""),   true);
    CPPUNIT_ASSERT_EQUAL(empty(" "),  false);
  }

VT_END()

//----------------------------------------------------------------------------

VT_BEGIN(ClassHelpers)

CPPUNIT_TEST_SUITE(VIGOTEST);
CPPUNIT_TEST(ClassMacros);
CPPUNIT_TEST(ThrowStuff);
CPPUNIT_TEST_SUITE_END();

private:
  struct Foo
  {
    int m_a;
    GETSET(A, SetA, int, m_a)

    COMPARABLE(Foo)
    int Compare(Foo const& r) const
    {
      if(m_a<r.m_a) return -1;
      if(m_a>r.m_a) return +1;
      return 0;
    }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS FOO_X1, FOO_X2
    DECLARE_EXC_ROOT
    #undef EXC_IDS
#endif

    void ThrowFoo()
    {
      THROW_EXC2(FOO_X1,"X1X")
    }
  };

  struct Bar: Foo
  {
    SUPER(Foo)
    GETTER(A, int, 1 + super::A())

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS BAR_XA, BAR_X2
    DECLARE_EXC
    #undef EXC_IDS
#endif

    void ThrowBar()
    {
      THROW_EXC2(BAR_X2,"X2X")
    }
  };

  struct SubBar: Bar
  {
    SUPER(Bar)

    void ThrowAlsoBar()
    {
      THROW_EXC2(BAR_X2,"X2X")
    }
  };

public:
  void setUp()
  {
  }

  void tearDown()
  {
  }

  void ClassMacros()
  {
    Bar bar; bar.SetA(2);
    CPPUNIT_ASSERT_EQUAL(3, bar.A());   // super works

    Foo f1; f1.SetA(1);
    CPPUNIT_ASSERT(f1.A() == f1.m_a);
    CPPUNIT_ASSERT(f1.A() == 1);        // get/set works

    Foo f2;

    f2.SetA(1); CPPUNIT_ASSERT(f1==f2); // Compare() works
                CPPUNIT_ASSERT(f1<=f2);
                CPPUNIT_ASSERT(f1>=f2);
    f2.SetA(2); CPPUNIT_ASSERT(f1!=f2);
                CPPUNIT_ASSERT(f1<f2);
                CPPUNIT_ASSERT(f1<=f2);
    f2.SetA(0); CPPUNIT_ASSERT(f1!=f2);
                CPPUNIT_ASSERT(f1>f2);
                CPPUNIT_ASSERT(f1>=f2);
  }

  void ThrowStuff()
  {
    Bar bar;

    try // throwing Foo
    {
      bar.ThrowFoo();
    }
    catch(Foo::exc& e)
    {
      CPPUNIT_ASSERT(e.what() == std::string("X1X"));
      CPPUNIT_ASSERT(e.What() == std::string("Exception [user 1001]: X1X"));
      CPPUNIT_ASSERT(e.Msg()  == std::string("X1X"));

      CPPUNIT_ASSERT(typeid(e) == typeid(Foo::exc));
    }

    try // throwing Bar::exc, catching by a superclass (Foo::exc) reference
    {
      bar.ThrowBar();
    }
    catch(Foo::exc& e)
    {
      CPPUNIT_ASSERT(e.what() == std::string("X2X"));
      CPPUNIT_ASSERT(e.What() == std::string("Exception [user 1002]: X2X"));
      CPPUNIT_ASSERT(e.Msg()  == std::string("X2X"));

      // caught by reference, e is Bar::exc
      CPPUNIT_ASSERT(typeid(e) == typeid(Bar::exc));
      // shallow copy
      CPPUNIT_ASSERT(typeid(Foo::exc(e)) == typeid(Foo::exc));
      // deep copy
      CPPUNIT_ASSERT(typeid(e.Copy()) == typeid(Bar::exc));
    }

    try // throwing Bar::exc, catching by a superclass (Foo::exc) value
    {
      bar.ThrowBar();
    }
    catch(Foo::exc e)
    {
      CPPUNIT_ASSERT(e.what() == std::string("X2X"));
      CPPUNIT_ASSERT(e.What() == std::string("Exception [user 1002]: X2X"));
      CPPUNIT_ASSERT(e.Msg()  == std::string("X2X"));

      // caught by value, e is Foo::exc
      CPPUNIT_ASSERT(typeid(e) == typeid(Foo::exc));
      // shallow copy
      CPPUNIT_ASSERT(typeid(Foo::exc(e)) == typeid(Foo::exc));
      // a deep copy is still Foo::exc
      CPPUNIT_ASSERT(typeid(e.Copy()) == typeid(Foo::exc));
    }

    SubBar subBar;

    try // SubBar also throws Bar::exc
    {
      subBar.ThrowAlsoBar();
    }
    catch(Foo::exc& e)
    {
      CPPUNIT_ASSERT(typeid(e) == typeid(Bar::exc));
    }
  }

VT_END()

//----------------------------------------------------------------------------

VT_BEGIN(Functions)

CPPUNIT_TEST_SUITE(VIGOTEST);
CPPUNIT_TEST(Func);
CPPUNIT_TEST_SUITE_END();

private:

public:
  void setUp()
  {
  }

  void tearDown()
  {
  }

  void Func()
  {
    CPPUNIT_ASSERT(2==min(2,3));
    CPPUNIT_ASSERT(3==max(3,2));
    CPPUNIT_ASSERT(3==abs(-3));
    CPPUNIT_ASSERT(-1==sgn(-3));
    byte a=1, b=2; swap(a,b); CPPUNIT_ASSERT(1==b && 2==a);
    CPPUNIT_ASSERT(odd(3));
    CPPUNIT_ASSERT(even(56));
    CPPUNIT_ASSERT(9==sqr(3));
    CPPUNIT_ASSERT(odd(3));
  }

VT_END()

//----------------------------------------------------------------------------

VT_BEGIN(Strings)

CPPUNIT_TEST_SUITE(VIGOTEST);
CPPUNIT_TEST(Trimming);
CPPUNIT_TEST_SUITE_END();

private:

public:
  void setUp()
  {
  }

  void tearDown()
  {
  }

  void Trimming()
  {
    std::string s("\f\n\r\t\v A\f\n\r\t\v ");
    CPPUNIT_ASSERT(std::string("A\f\n\r\t\v ") == trimleft(s));
    CPPUNIT_ASSERT(std::string("\f\n\r\t\v A") == trimright(s));
    CPPUNIT_ASSERT(std::string("A") == trim(s));
    CPPUNIT_ASSERT(std::string("\n\r\t\v A\f\n\r\t\v") == trim(s,"\f "));
  }

VT_END()

//----------------------------------------------------------------------------

VT_BEGIN(TypeIds)

CPPUNIT_TEST_SUITE(VIGOTEST);
CPPUNIT_TEST(Assumptions);
CPPUNIT_TEST_SUITE_END();

private:

public:
  void setUp()
  {
  }

  void tearDown()
  {
  }

  void Assumptions()
  {
    // assumptions about typeids
    class A{};
    class B{};

    A a1, a2;
    pcvoid id = &typeid(A);

    CPPUNIT_ASSERT(id != &typeid(B));
    CPPUNIT_ASSERT(id != &typeid(A*));
    CPPUNIT_ASSERT(id == &typeid(a1));
    CPPUNIT_ASSERT(id == &typeid(a2));
    CPPUNIT_ASSERT(id != &typeid(&a2));
  }

VT_END()

//----------------------------------------------------------------------------

#if RUN_ALL_TESTS || RUN_COMMON_TESTS
  VT_REGISTER(PrimitiveTypes)
  VT_REGISTER(ClassHelpers)
  VT_REGISTER(Functions)
  VT_REGISTER(Strings)
  VT_REGISTER(TypeIds)
#endif

NAMESPACE_END() // eof
