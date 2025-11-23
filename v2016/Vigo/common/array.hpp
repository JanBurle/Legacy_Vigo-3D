// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Templated safe 1- 2- and 3-D array classes
 *
 *  These classes encapsulate 1- 2- or 3-dimensional C-type arrays.
 *  In the debug build the arrays are safe (i.e. perform boundary checking.)
 *  In the release build they are almost as efficient as C-type arrays.
 */

#ifndef VIGO_COMMON_ARRAY_HPP_INCLUDED
#define VIGO_COMMON_ARRAY_HPP_INCLUDED

  #include "./types.hpp"
  #include "./tuple.hpp"
  #include <iostream>

NAMESPACE_VIGO(util) //========================================================
/** \defgroup grp_array_hpp Safe arrays
@{*/

/// One-dimensional safe array
  template<typename T>
  class array1d
  {
  protected:
    T    *m_data;
    uint  m_size;

  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS OUT_OF_BOUNDS
    DECLARE_EXC_ROOT
    #undef EXC_IDS
#endif

  public:
    /// Construct an uninitialized array of a specified size.
    /** @param size The number of array items.
     */
    array1d(uint size): m_data(NULL), m_size(size)
    {
      m_data = new T[m_size];
    }

    /// Construct an array of a specified size, with each item set
    /** to the \c init value.
     *  @param size The number of array items.
     *  @param init The initial value of array items.
     */
    array1d(uint size, T const& init): m_data(NULL), m_size(size)
    {
      m_data = new T[m_size];
      set(init);
    }

    /// Construct an array of a specified size and specified contents.
    /** @param size The number of array items.
     *  @param data The initial contents of the array, must point to a memory
     *     area of \c size values of type \c T.
     *  @param copy If true, make a copy of the data. If false, the object will
     *     assume ownership of the \c data memory, and will eventually delete
     *     it in its destructor.
     */
    array1d(uint size, T* data, bool copy=true): m_data(NULL), m_size(size)
    {
      assert(data);
      if(copy)
      {
        m_data = new T[m_size];
        ::memcpy((pvoid)m_data, (pcvoid)data, sizeof(T) * m_size);
      }
      else
      {
        m_data = data;
      }
    }

    /// \overload
    array1d(uint size, T const* data): m_data(NULL), m_size(size)
    {
      assert(data);
      m_data = new T[m_size];
      ::memcpy((pvoid)m_data, (pcvoid)data, sizeof(T) * m_size);
    }

    /// A copy constructor. A copy of the data will be allocated.
    array1d(array1d const& r): m_data(NULL), m_size(0)
    {
      *this = r;
    }

    virtual~ array1d()
    {
      delete [] m_data;
    }

    /// An assignment, binary copy.
    /** The target object allocates its own memory and the
     *  data from the source is copied into it.
     */
    array1d& operator= (array1d const& r)
    {
      delete [] m_data;

      m_size = r.m_size;
      m_data = new T[m_size];
      ::memcpy((pvoid)m_data, (pcvoid)r.m_data, sizeof(T) * m_size);

      return *this;
    }

    /// An assignment, item by item copy.
    /** The target object allocates its own memory and the
     *  data from the source is copied into it.
     */
    array1d& memcpy(array1d const& r)
    {
      delete [] m_data;

      m_size = r.m_size;
      m_data = new T[m_size];
      for(uint i=0; i<m_size; ++i)
	m_data[i] = r.m_data[i];

      return *this;
    }

    /// Sets every item to the \c init value.
    array1d& set(T const& init)
    {
      for(uint i = 0; i<m_size; ++i)
        m_data[i] = init;

      return *this;
    }

    /// binary compare
    bool memcmp(array1d const& r) const
    {
      return (m_size==r.m_size)
          && (0==memcmp((pcvoid)m_data,(pcvoid)r.m_data,sizeof(T) * m_size));
    }

    /// item by item compare
    bool operator== (array1d const& r) const
    {
      if(m_size!=r.m_size)
	return false;

      for(uint i=0; i<m_size; ++i)
	if(m_data[i] != r.m_data[i])
	  return false;
    }

    /// item by item compare
    bool operator!= (array1d const& r) const
    {
      return !operator==(r);
    }

    /// Indexing into the array, debug-time boundary checks.
    inline T& operator() (uint i)
    {
      ASSERT_EXC2(i<m_size,OUT_OF_BOUNDS,"array1d: index out of bounds")
      return m_data[i];
    }

    /// \overload
    inline T const& operator() (uint i) const
    {
      return const_cast<array1d*>(this)->operator()(i);
    }

    /// \overload
    /** @param u The index, in a form of a 1-D tuple.
     */
    inline T&       operator() (uint1d const& u)       { return operator()(u.x); }
    /// \overload
    inline T const& operator() (uint1d const& u) const { return operator()(u.x); }

    /// Indexing into the array, debug-time boundary checks.
    inline T& operator[] (uint i)
    {
      return operator()(i);
    }

    /// \overload
    inline T const& operator[] (uint i) const
    {
      return const_cast<array1d*>(this)->operator[](i);
    }

    /// Direct access to the data.
    /** @return A pointer to the array memory. */
    inline T* ptr()
    {
      return m_data;
    }

    /// \overload
    inline T const* ptr() const
    {
      return const_cast<array1d*>(this)->ptr();
    }

    /// Direct access to the data, with an offset of \c i items.
    /** @param i The index of the first item to access.
     *  @return  A pointer to the array memory, starting at the i-th item.
     */
    inline T* ptr(uint i)
    {
      return m_data + i;
    }

    /// \overload
    inline T const* ptr(uint i) const
    {
      return const_cast<array1d*>(this)->ptr(i);
    }

    /// Detaches and returns the array data.
    /** The caller must assume ownership of the data mamory!
     */
    T* detach()
    {
      T* data = m_data;
      m_data = NULL;
      return data;
    }

    /// The array size.
    /** @return The number of array items.
     */
    uint   size()   const { return m_size;         }
    /// The array size.
    /** @return The number of array items, as a 1-D tuple.
     */
    uint1d size1d() const { return uint1d(m_size); }

    /// Apply a function \c fn to each item
    void foreach(void (*fn)(T&))
    {
      for(T *t=m_data, *tEnd=m_data+m_size; t<tEnd; ++t)
	fn(*t);
    }
  };

  /// Insert an array into a stream, as a comma-separated list of items.
  template<class T>
  std::ostream& operator<<(std::ostream& os, array1d<T> const& a)
  {
    for(uint i=0; i<a.size(); ++i)
    {
      if(i>0) os << ',';
      os << a[i];
    }
    return os;
  }

//----------------------------------------------------------------------------
/** Two-dimensional safe array. \see array1d. */

  template<typename T>
  class array2d: public array1d<T>
  {
    SUPER(array1d<T>)

  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS
    DECLARE_EXC
    #undef EXC_IDS
#endif

  protected:
    uint m_size2;

  public:
    /// Construct an uninitialized array of a specified size.
    /** @param size1 First dimension.
     *  @param size2 Second dimension.
     */
    array2d(uint size1, uint size2): super(size1*size2), m_size2(size2)
    {
    }

    /// Construct an array, with each item set to the \c init value.
    array2d(uint size1, uint size2, T const& init)
      : super(size1*size2, init), m_size2(size2)
    {
    }

    /// A 2-D array of specified sizes and specified contents.
    array2d(uint size1, uint size2, T* data, bool copy=true)
      : super(size1*size2, data, copy), m_size2(size2)
    {
    }

    /// \overload
    array2d(uint size1, uint size2, T const* data)
      : super(size1*size2, data), m_size2(size2)
    {
    }

    /// A deep copy.
    array2d(array2d const& r): super(r), m_size2(r.m_size2)
    {
    }

    /// A deep assignment.
    array2d& operator= (array2d const& r)
    {
      m_size2 = r.m_size2;
      return static_cast<array2d&>(super::operator=(r));
    }

    /// A deep assignment.
    array2d& memcpy(array2d const& r)
    {
      m_size2 = r.m_size2;
      return static_cast<array2d&>(super::memcpy(r));
    }

    /// Sets every item to the \c init value.
    array2d& set(T const& init)
    {
      super::set(init);
      return *this;
    }

    /// binary compare
    bool memcmp(array2d const& r) const
    {
      return (m_size2==r.m_size2) && super::memcmp(r);
    }

    /// item by item compare
    bool operator== (array2d const& r) const
    {
      return (m_size2==r.m_size2) && super::operator==(r);
    }

    /// item by item compare
    bool operator!= (array2d const& r) const
    {
      return !operator==(r);
    }

    /// Indexing into the array as if it were a 1-D array.
    inline T& operator() (uint i)
    {
      return super::operator()(i);
    }

    /// \overload
    inline T const& operator() (uint i) const
    {
      return super::operator()(i);
    }

    /// \overload
    inline T&       operator() (uint1d const& u)       { return operator()(u.x); }
    /// \overload
    inline T const& operator() (uint1d const& u) const { return operator()(u.x); }

    /// Indexing into the array, two-dimensional.
    inline T& operator() (uint i,uint j)
    {
      ASSERT_EXC2(j<m_size2,OUT_OF_BOUNDS,"array2d: index out of bounds")
      return super::operator()(i*m_size2+j);
    }

    /// \overload
    inline T const& operator() (uint i,uint j) const
    {
      return const_cast<array2d*>(this)->operator()(i,j);
    }

    /// \overload
    inline T&       operator() (uint2d const& u)       { return operator()(u.u,u.v); }
    /// \overload
    inline T const& operator() (uint2d const& u) const { return operator()(u.u,u.v); }

    /// Direct access to the data.
    inline T* ptr()
    {
      return super::ptr();
    }

    /// \overload
    inline T const* ptr() const
    {
      return super::ptr();
    }

    /// Direct access to the data, with a 1-D offset.
    inline T* ptr(uint i)
    {
      return super::ptr(i);
    }

    /// \overload
    inline T const* ptr(uint i) const
    {
      return super::ptr(i);
    }

    /// Direct access to the data, with a 2-D offset.
    inline T* ptr(uint i, uint j)
    {
      return super::ptr(i*m_size2+j);
    }

    /// \overload
    inline T const* ptr(uint i, uint j) const
    {
      return const_cast<array2d*>(this)->ptr(i,j);
    }

    /// The array size (first dimension).
    uint   size1()  const { return super::size()/m_size2;   }
    /// The array size (second dimension).
    uint   size2()  const { return m_size2;                 }
    /// The array size as a 2-D tuple.
    uint2d size2d() const { return uint2d(size1(),size2()); }
  };

  /// Insert into a stream, items separated by commas and newlines.
  template<class T>
  std::ostream& operator<<(std::ostream& os, array2d<T> const& a)
  {
    for(uint i=0; i<a.size1(); ++i)
    {
      os << std::endl;
      for(uint j=0; j<a.size2(); ++j)
      {
        if(j>0)
          os << ',';
        os << a(i,j);
      }
    }
    return os;
  }

//----------------------------------------------------------------------------
/** Three-dimensional safe array. \see array2d. */

  template<typename T>
  class array3d: public array2d<T>
  {
    SUPER(array2d<T>)

  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS
    DECLARE_EXC
    #undef EXC_IDS
#endif

  protected:
    uint m_size3;

  public:
    /// Construct an uninitialized array of a specified size.
    /** @param size1 First dimension.
     *  @param size2 Second dimension.
     *  @param size3 Third dimension.
     */
    array3d(uint size1, uint size2, uint size3)
      : super(size1,size2*size3), m_size3(size3)
    {
    }

    /// Construct an array, with each item set to the \c init value.
    array3d(uint size1, uint size2, uint size3, T const& init)
      : super(size1,size2*size3, init), m_size3(size3)
    {
    }

    /// A 3-D array of specified sizes and specified contents.
    array3d(uint size1, uint size2, uint size3, T* data, bool copy=true)
      : super(size1,size2*size3, data, copy), m_size3(size3)
    {
    }

    /// \overload
    array3d(uint size1, uint size2, uint size3, T const* data)
      : super(size1,size2*size3, data), m_size3(size3)
    {
    }

    /// A deep copy.
    array3d(array3d const& r): super(r), m_size3(r.m_size3)
    {
    }

    /// A deep assignment.
    array3d& operator= (array3d const& r)
    {
      m_size3 = r.m_size3;
      super::operator=(r);
    }

    /// A deep assignment.
    array3d& memcpy(array3d const& r)
    {
      m_size3 = r.m_size3;
      super::memcpy(r);
    }

    /// Sets every item to the \c init value.
    array3d& set(T const& init)
    {
      super::set(init);
      return *this;
    }

    /// item by item compare
    bool operator== (array3d const& r) const
    {
      return (m_size3==r.m_size3) && super::operator==(r);
    }

    /// item by item compare
    bool operator!= (array3d const& r) const
    {
      return !operator==(r);
    }

    /// binary compare
    bool memcmp(array3d const& r) const
    {
      return (m_size3==r.m_size3) && super::memcmp(r);
    }

    /// Indexing into the array as if it were a 1-D array.
    inline T& operator() (uint i)
    {
      return super::operator()(i);
    }

    /// \overload
    inline T const& operator() (uint i) const
    {
      return super::operator()(i);
    }

    /// \overload
    inline T&       operator() (uint1d const& u)       { return operator()(u.x); }
    /// \overload
    inline T const& operator() (uint1d const& u) const { return operator()(u.x); }

    /// debug-time safe indexing, 2-D
    inline T& operator() (uint i, uint j)
    {
      return super::operator()(i,j);
    }

    /// Indexing into the array as if it were a 2-D array.
    inline T const& operator() (uint i, uint j) const
    {
      return super::operator()(i,j);
    }

    /// \overload
    inline T&       operator() (uint2d const& u)       { return operator()(u.u,u.v); }
    /// \overload
    inline T const& operator() (uint2d const& u) const { return operator()(u.u,u.v); }

    /// debug-time safe indexing, 3-D
    inline T& operator() (uint i,uint j,uint k)
    {
      ASSERT_EXC2(k<m_size3,OUT_OF_BOUNDS,"array3d: index out of bounds")
      return super::operator()(i,j*m_size3+k);
    }

    /// Indexing into the array, three-dimensional.
    inline T const& operator() (uint i,uint j,uint k) const
    {
      return const_cast<array3d*>(this)->operator()(i,j,k);
    }

    /// \overload
    inline T&       operator() (uint3d const& u)       { return operator()(u.x,u.y,u.z); }
    /// \overload
    inline T const& operator() (uint3d const& u) const { return operator()(u.x,u.y,u.z); }

    /// Direct access to the data.
    inline T* ptr()
    {
      return super::ptr();
    }

    /// \overload
    inline T const* ptr() const
    {
      return super::ptr();
    }

    /// Direct access to the data, with a 1-D offset.
    inline T* ptr(uint i)
    {
      return super::ptr(i);
    }

    /// \overload
    inline T const* ptr(uint i) const
    {
      return super::ptr(i);
    }

    /// Direct access to the data, with a 2-D offset.
    inline T* ptr(uint i, uint j)
    {
      return super::ptr(i,j);
    }

    /// \overload
    inline T const* ptr(uint i, uint j) const
    {
      return super::ptr(i,j);
    }

    /// Direct access to the data, with a 3-D offset.
    inline T* ptr(uint i, uint j, uint k)
    {
      return super::ptr(i,j*m_size3+k);
    }

    /// \overload
    inline T const* ptr(uint i, uint j, uint k) const
    {
      return const_cast<array3d*>(this)->ptr(i,j,k);
    }

    /// The array size (second dimension).
    uint   size2()  const { return super::size2()/m_size3; }
    /// The array size (third dimension).
    uint   size3()  const { return m_size3;                }
    /// The array size as a 3-D tuple.
    uint3d size3d() const { return uint3d(this->size1(),size2(),size3()); }
  };

  /// Insert into a stream, items separated by commas and newlines and indented.
  template<class T>
  std::ostream& operator<<(std::ostream& os, array3d<T> const& a)
  {
    for(uint i=0; i<a.size1(); ++i)
    {
      for(uint j=0; j<a.size2(); ++j)
      {
        os << std::endl;
        if(j>0)
          os << "  "; // indent
        for(uint k=0; k<a.size3(); ++k)
        {
          if(k>0)
            os << ',';
          os << a(i,j,k);
        }
      }
    }
    return os;
  }

//-----------------------------------------------------------------------------
/// Useful array operations.

  /// Set array items to a sequence of incrementing values.
  template<class N>
  void iota(array1d<N>& a, N const step)
  {
    N *p = a.ptr();
    for(uint i=0, iSize=a.size(); i<iSize; ++i)
      *(p++) = i*step;
  }

  /// Set array items to a sequence of incrementing values.
  template<class N>
  inline void iota(array2d<N>& a, N const step)
  {
    iota((array1d<N>&)a,step);
  }

  /// Set array items to a sequence of incrementing values.
  template<class N>
  inline void iota(array3d<N>& a, N const step)
  {
    iota((array1d<N>&)a,step);
  }

  /// Set array items to a sequence of values incremented by 1.
  template<class N>
  void iota(array1d<N>& a)
  {
    iota(a, N(1));
  }

  /// Set array items to a sequence of values incremented by 1.
  template<class N>
  void iota(array2d<N>& a)
  {
    iota(a, N(1));
  }

  /// Set array items to a sequence of values incremented by 1.
  template<class N>
  void iota(array3d<N>& a)
  {
    iota(a, N(1));
  }

  /// Approximate comparison (\c eps=tolerance).
  template<class N>
  bool almostEqual(array1d<N> const& a, array1d<N> const& b, N const eps)
  {
    if(a.size() != b.size())
      return false;

    N const *pa = a.ptr(), *pb = b.ptr();

    for(uint i=0, iSize=a.size(); i<iSize; ++i)
    {
      if(abs((*(pa++)) - (*(pb++))) > eps)
        return false;
    }

    return true;
  }

  /// Approximate comparison (\c eps=tolerance).
  template<class N>
  bool almostEqual(array2d<N> const& a, array2d<N> const& b, N const eps)
  {
    return almostEqual((array1d<N> const&)a, (array1d<N> const&)b,eps);
  }

  /// Approximate comparison (\c eps=tolerance).
  template<class N>
  bool almostEqual(array3d<N> const& a, array3d<N> const& b, N const eps)
  {
    return almostEqual((array1d<N> const&)a, (array1d<N> const&)b,eps);
  }

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
