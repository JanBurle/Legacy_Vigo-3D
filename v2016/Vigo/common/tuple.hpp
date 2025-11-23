// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Templated 1-, 2-, 3- and 4-component tuple classes
 *
 *  The debug version performs boundary checking.
 *  The release version has only a little overhead.
 */

#ifndef VIGO_COMMON_TUPLE_HPP_INCLUDED
#define VIGO_COMMON_TUPLE_HPP_INCLUDED

  #include "./types.hpp"
  #include <iostream>
  #include <sstream>
  #include <limits>

NAMESPACE_VIGO(util) //========================================================
/** \defgroup grp_tuple_hpp Tuples
@{*/

  /// A single value tuple, provided for consistency with tuple2|3d|4d
  template<typename T>
  struct tuple1
  {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS OUT_OF_BOUNDS
    DECLARE_EXC_ROOT
    #undef EXC_IDS
#endif

    T x;

    /// an undefined value
    tuple1()
    {
    }

    /// a specified initial value
    tuple1(T x_)
     : x(x_)
    {
    }

    /// a copy of another tuple
    tuple1(tuple1 const& r)
     : x(r.x)
    {
    }

    /// read from a string
    tuple1(pcstr p)
    {
      std::string s(p?p:""); std::istringstream is(s);
      is >> *this;
    }

    tuple1(std::string const& s)
    {
      std::istringstream is(s);
      is >> *this;
    }

    /// templated assignment with conversion from a tuple of another type
    template<typename O>
    tuple1& Assign(tuple1<O> const& o)
    {
      x = (T)o.x;
      return *this;
    }

    /// Indexing, provided for consistency with tuple2 etc.; the index must be 0.
    inline T& operator[] (uint const i)
    {
      ASSERT_EXC2(i<1,OUT_OF_BOUNDS,"tuple1: index out of bounds")
      return *((&x) + i);
    }

    /// \overload
    inline T const& operator[] (uint const i) const
    {
      return const_cast<tuple1*>(this)->operator[](i);
    }

    COMPARABLE(tuple1)
    /// Comparison operators.
    int Compare(tuple1 const& r) const
    {
      if(x<r.x) return -1;
      if(x>r.x) return +1;
      return 0;
    }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  // arithmetic operators
  #define VIGO_UTIL_TUPLE1_OPERATOR(op) \
    template<typename O>                                        \
    inline tuple1 operator op (tuple1<O> const& r) const        \
    {                                                           \
      return tuple1(*this) op##= r;                             \
    }                                                           \
                                                                \
    template<typename O>                                        \
    tuple1& operator op##= (tuple1<O> const& r)                 \
    {                                                           \
      this->x op##= r.x;                                        \
      return *this;                                             \
    }                                                           \
                                                                \
    template<typename O>                                        \
    inline tuple1 operator op (O r) const                       \
    {                                                           \
      return tuple1(*this) op##= r;                             \
    }                                                           \
                                                                \
    template<typename O>                                        \
    tuple1& operator op##= (O r)                                \
    {                                                           \
      this->x op##= r;                                          \
      return *this;                                             \
    }
#endif

    VIGO_UTIL_TUPLE1_OPERATOR(+)
    VIGO_UTIL_TUPLE1_OPERATOR(-)
    VIGO_UTIL_TUPLE1_OPERATOR(*)
    VIGO_UTIL_TUPLE1_OPERATOR(/)

  #undef VIGO_UTIL_TUPLE1_OPERATOR
  };

//----------------------------------------------------------------------------

  /// A pair (two) value tuple: u, v.
  template<typename T>
  struct tuple2
  {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS OUT_OF_BOUNDS
    DECLARE_EXC_ROOT
    #undef EXC_IDS
#endif

    T u, v;

    /// an undefined value
    tuple2()
    {
    }

    /// a single specified initial value for both components
    tuple2(T init)
     : u(init), v(init)
    {
    }

    /// specified values of both components
    tuple2(T u_, T v_)
     : u(u_), v(v_)
    {
    }

    /// a copy
    tuple2(tuple2 const& r)
     : u(r.u), v(r.v)
    {
    }

    /// read from a string
    tuple2(pcstr p)
    {
      std::string s(p?p:""); std::istringstream is(s);
      is >> *this;
    }

    tuple2(std::string const& s)
    {
      std::istringstream is(s);
      is >> *this;
    }

    /// templated assignment with conversion from another \c tuple2 type
    template<typename O>
    tuple2& Assign(tuple2<O> const& o)
    {
      u = (T)o.u; v = (T)o.v;
      return *this;
    }

    /// Indexing; the index must be 0 or 1.
    inline T& operator[] (uint const i)
    {
      ASSERT_EXC2(i<2,OUT_OF_BOUNDS,"tuple2: index out of bounds")
      return *((&u) + i);
    }

    /// \overload
    inline T const& operator[] (uint const i) const
    {
      return const_cast<tuple2*>(this)->operator[](i);
    }

    COMPARABLE(tuple2)
    /// Comparison operators. Sorted by \c u first, then by \c v.
    int Compare(tuple2 const& r) const
    {
      if(u<r.u) return -1;
      if(u>r.u) return +1;
      if(v<r.v) return -1;
      if(v>r.v) return +1;
      return 0;
    }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  // arithmetic operators
  #define VIGO_UTIL_TUPLE2_OPERATOR(op) \
    template<typename O>                                        \
    inline tuple2 operator op (tuple2<O> const& r) const        \
    {                                                           \
      return tuple2(*this) op##= r;                             \
    }                                                           \
                                                                \
    template<typename O>                                        \
    tuple2& operator op##= (tuple2<O> const& r)                 \
    {                                                           \
      this->u op##= r.u;                                        \
      this->v op##= r.v;                                        \
      return *this;                                             \
    }                                                           \
                                                                \
    template<typename O>                                        \
    inline tuple2 operator op (O r) const                       \
    {                                                           \
      return tuple2(*this) op##= r;                             \
    }                                                           \
                                                                \
    template<typename O>                                        \
    tuple2& operator op##= (O r)                                \
    {                                                           \
      this->u op##= r;                                          \
      this->v op##= r;                                          \
      return *this;                                             \
    }
#endif

    VIGO_UTIL_TUPLE2_OPERATOR(+)
    VIGO_UTIL_TUPLE2_OPERATOR(-)
    VIGO_UTIL_TUPLE2_OPERATOR(*)
    VIGO_UTIL_TUPLE2_OPERATOR(/)

  #undef VIGO_UTIL_TUPLE2_OPERATOR
  };

//----------------------------------------------------------------------------

  /// A triple (three) value tuple: x-y-z.
  template<typename T>
  struct tuple3
  {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS OUT_OF_BOUNDS
    DECLARE_EXC_ROOT
    #undef EXC_IDS
#endif

    T x, y, z;

    /// an undefined value
    tuple3()
    {
    }

    /// a single specified initial value
    tuple3(T init)
     : x(init), y(init), z(init)
    {
    }

    /// specified values of all components
    tuple3(T x_, T y_, T z_)
     : x(x_), y(y_), z(z_)
    {
    }

    /// a copy
    tuple3(tuple3 const& v)
     : x(v.x), y(v.y), z(v.z)
    {
    }

    /// read from a string
    tuple3(pcstr p)
    {
      std::string s(p?p:""); std::istringstream is(s);
      is >> *this;
    }

    tuple3(std::string const& s)
    {
      std::istringstream is(s);
      is >> *this;
    }

    /// templated assignment with conversion from another \c tuple3 type
    template<typename O>
    tuple3& Assign(tuple3<O> const& o)
    {
      x = (T)o.x; y = (T)o.y; z = (T)o.z;
      return *this;
    }

    /// Indexing; the index must be 0, 1 or 2.
    inline T& operator[] (uint const i)
    {
      ASSERT_EXC2(i<3,OUT_OF_BOUNDS,"tuple3: index out of bounds")
      return *((&x) + i);
    }

    /// \overload
    inline T const& operator[] (uint const i) const
    {
      return const_cast<tuple3*>(this)->operator[](i);
    }

    COMPARABLE(tuple3)
    /// Comparison operators. Sorted by \c x first, then by \c y, then \c z.
    int Compare(tuple3 const& r) const
    {
      if(x<r.x) return -1;
      if(x>r.x) return +1;
      if(y<r.y) return -1;
      if(y>r.y) return +1;
      if(z<r.z) return -1;
      if(z>r.z) return +1;
      return 0;
    }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  // arithmetic operators
  #define VIGO_UTIL_TUPLE3_OPERATOR(op) \
    template<typename O>                                        \
    inline tuple3 operator op (tuple3<O> const& r) const        \
    {                                                           \
      return tuple3(*this) op##= r;                             \
    }                                                           \
                                                                \
    template<typename O>                                        \
    tuple3& operator op##= (tuple3<O> const& r)                 \
    {                                                           \
      this->x op##= r.x;                                        \
      this->y op##= r.y;                                        \
      this->z op##= r.z;                                        \
      return *this;                                             \
    }                                                           \
                                                                \
    template<typename O>                                        \
    inline tuple3 operator op (O r) const                       \
    {                                                           \
      return tuple3(*this) op##= r;                             \
    }                                                           \
                                                                \
    template<typename O>                                        \
    tuple3& operator op##= (O r)                                \
    {                                                           \
      this->x op##= r;                                          \
      this->y op##= r;                                          \
      this->z op##= r;                                          \
      return *this;                                             \
    }
#endif

    VIGO_UTIL_TUPLE3_OPERATOR(+)
    VIGO_UTIL_TUPLE3_OPERATOR(-)
    VIGO_UTIL_TUPLE3_OPERATOR(*)
    VIGO_UTIL_TUPLE3_OPERATOR(/)

  #undef VIGO_UTIL_TUPLE3_OPERATOR
  };

//----------------------------------------------------------------------------

  /// A quadruple (four) value tuple: x-y-z-w.
  template<typename T>
  struct tuple4
  {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS OUT_OF_BOUNDS
    DECLARE_EXC_ROOT
    #undef EXC_IDS
#endif

    T x, y, z, w;

    /// an undefined value
    tuple4()
    {
    }

    /// a single specified initial value
    tuple4(T init)
     : x(init), y(init), z(init), w(init)
    {
    }

    /// specified values of all components
    tuple4(T x_, T y_, T z_, T w_)
     : x(x_), y(y_), z(z_), w(w_)
    {
    }

    /// a copy
    tuple4(tuple4 const& v)
     : x(v.x), y(v.y), z(v.z), w(v.w)
    {
    }

    /// read from a string
    tuple4(pcstr p)
    {
      std::string s(p?p:""); std::istringstream is(s);
      is >> *this;
    }

    tuple4(std::string const& s)
    {
      std::istringstream is(s);
      is >> *this;
    }

    /// templated assignment with conversion from another \c tuple4 type
    template<typename O>
    tuple4& Assign(tuple4<O> const& o)
    {
      x = (T)o.x; y = (T)o.y; z = (T)o.z; w = (T)o.w;
      return *this;
    }

    /// Indexing; the index must be 0 to 3.
    inline T& operator[] (uint const i)
    {
      ASSERT_EXC2(i<4,OUT_OF_BOUNDS,"tuple4: index out of bounds")
      return *((&x) + i);
    }

    /// \overload
    inline T const& operator[] (uint const i) const
    {
      return const_cast<tuple4*>(this)->operator[](i);
    }

    COMPARABLE(tuple4)
    /// Comparison operators. Sorted by \c x, \c y, \c z, \c w.
    int Compare(tuple4 const& r) const
    {
      if(x<r.x) return -1;
      if(x>r.x) return +1;
      if(y<r.y) return -1;
      if(y>r.y) return +1;
      if(z<r.z) return -1;
      if(z>r.z) return +1;
      if(w<r.w) return -1;
      if(w>r.w) return +1;
      return 0;
    }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  // arithmetic operators
  #define VIGO_UTIL_TUPLE4_OPERATOR(op) \
    template<typename O>                                        \
    inline tuple4 operator op (tuple4<O> const& r) const        \
    {                                                           \
      return tuple4(*this) op##= r;                             \
    }                                                           \
                                                                \
    template<typename O>                                        \
    tuple4& operator op##= (tuple4<O> const& r)                 \
    {                                                           \
      this->x op##= r.x;                                        \
      this->y op##= r.y;                                        \
      this->z op##= r.z;                                        \
      this->w op##= r.w;                                        \
      return *this;                                             \
    }                                                           \
                                                                \
    template<typename O>                                        \
    inline tuple4 operator op (O r) const                       \
    {                                                           \
      return tuple4(*this) op##= r;                             \
    }                                                           \
                                                                \
    template<typename O>                                        \
    tuple4& operator op##= (O r)                                \
    {                                                           \
      this->x op##= r;                                          \
      this->y op##= r;                                          \
      this->z op##= r;                                          \
      this->w op##= r;                                          \
      return *this;                                             \
    }
#endif

    VIGO_UTIL_TUPLE4_OPERATOR(+)
    VIGO_UTIL_TUPLE4_OPERATOR(-)
    VIGO_UTIL_TUPLE4_OPERATOR(*)
    VIGO_UTIL_TUPLE4_OPERATOR(/)

  #undef VIGO_UTIL_TUPLE4_OPERATOR
  };

//-----------------------------------------------------------------------------

  /// insert into a stream
  template<class T>
  std::ostream& operator<<(std::ostream& os, tuple1<T> const& v)
  {
    return os << v.x;
  }

  /// insert into a stream, comma separated values
  template<class T>
  std::ostream& operator<<(std::ostream& os, tuple2<T> const& v)
  {
    return os << v.u << ',' << v.v;
  }

  /// insert into a stream, comma separated values
  template<class T>
  std::ostream& operator<<(std::ostream& os, tuple3<T> const& v)
  {
    return os << v.x << ',' << v.y << ',' << v.z;
  }

  /// insert into a stream, comma separated values
  template<class T>
  std::ostream& operator<<(std::ostream& os, tuple4<T> const& v)
  {
    return os << v.x << ',' << v.y << ',' << v.z << ',' << v.w;
  }

//-----------------------------------------------------------------------------

  /// extract from a stream
  template<class T>
  std::istream& operator>>(std::istream& is, tuple1<T>& v)
  {
    is >> v.x;
    return is;
  }

  /// extract from a stream, comma separated values
  template<class T>
  std::istream& operator>>(std::istream& is, tuple2<T>& v)
  {
    is >> v.u;
    is.ignore(std::numeric_limits<int>::max(),',');
    is >> v.v;
    return is;
  }

  /// extract from a stream, comma separated values
  template<class T>
  std::istream& operator>>(std::istream& is, tuple3<T>& v)
  {
    is >> v.x;
    is.ignore(std::numeric_limits<int>::max(),',');
    is >> v.y;
    is.ignore(std::numeric_limits<int>::max(),',');
    is >> v.z;
    return is;
  }

  /// extract from a stream, comma separated values
  template<class T>
  std::istream& operator>>(std::istream& is, tuple4<T>& v)
  {
    is >> v.x;
    is.ignore(std::numeric_limits<int>::max(),',');
    is >> v.y;
    is.ignore(std::numeric_limits<int>::max(),',');
    is >> v.z;
    is.ignore(std::numeric_limits<int>::max(),',');
    is >> v.w;
    return is;
  }

//-----------------------------------------------------------------------------

/// \name Instantiated frequently used templates.
  typedef tuple1<float>  float1d;
  typedef tuple2<float>  float2d;
  typedef tuple3<float>  float3d;
  typedef tuple4<float>  float4d;

  typedef tuple1<double> double1d;
  typedef tuple2<double> double2d;
  typedef tuple3<double> double3d;
  typedef tuple4<double> double4d;

  typedef tuple1<int>    int1d;
  typedef tuple2<int>    int2d;
  typedef tuple3<int>    int3d;
  typedef tuple4<int>    int4d;

  typedef tuple1<uint>   uint1d;
  typedef tuple2<uint>   uint2d;
  typedef tuple3<uint>   uint3d;
  typedef tuple4<uint>   uint4d;

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
