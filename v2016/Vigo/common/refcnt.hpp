// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Reference counted objects and pointers and related issues.
 *
 *  Implements reference counted objects and pointers. Objects could be
 *  restricted to the heap, to the stack, or able to detect their location.
 *  Automatic destruction of heap-based objects is possible.
 */

#ifndef VIGO_COMMON_REFCNT_HPP_INCLUDED
#define VIGO_COMMON_REFCNT_HPP_INCLUDED

  #include "./types.hpp"
  #include "./async.hpp"

NAMESPACE_VIGO(util) //========================================================
/** \defgroup grp_refctn_hpp Reference counting
@{*/

/** \name Detecting or enforcing storage types
@{*/
  /// The base class for storage types.
  template<class T>
  class storage_base: public T
  {
    SUPER(T)

  protected:
    enum {flNONE=0x00, flON_HEAP=0x01};
    /// Detects the value of the flags.
    uint8 QFlags()
    {
      extern thread::tls& storage_base_HeapPending();
      uint *hp = (uint*)storage_base_HeapPending().get();
      if(NULL==hp || 0==*hp) return flNONE;
      --(*hp); return flON_HEAP;
    }

    virtual ~storage_base() = 0;

  public:
    /// overloaded \c new
    static pvoid operator new(size_t sz)
    {
      extern thread::tls& storage_base_HeapPending();
      uint *hp = (uint*)storage_base_HeapPending().get();
      if(hp==NULL)
      {
        hp = new uint(0);
        storage_base_HeapPending().set(hp);
      }

      ++(*hp);
      return ::operator new(sz);
    }

  public:
    AGETTER(IsOnHeap,bool)
  };

  template<class T>
  storage_base<T>::~storage_base()
  {
  }


  /// An object that (almost certainly) detects if it is on the heap or not.
  template<class T>
  class storage_auto: public storage_base<T>
  {
    SUPER(storage_base<T>)

  private:
    uint8 m_flags;

  public:
    GETTER(IsOnHeap, bool, !!(super::flON_HEAP & m_flags))

    /// Detects if the object is on the heap and sets a flag accordingly.
    storage_auto()
      : super(), m_flags(super::QFlags())
    {
    }

    storage_auto(storage_auto<T> const& r)
      : super(r), m_flags(super::QFlags())
    {
    }

    storage_auto(storage_base<T> const& r)
      : super(r), m_flags(super::QFlags())
    {
    }

    storage_auto& operator=(storage_auto<T> const& r)
    {
      super::operator=(r);
      return *this;
      // m_isOnHeap stays as it was
    }

    storage_auto& operator=(storage_base<T> const& r)
    {
      super::operator=(r);
      return *this;
      // m_isOnHeap stays as it was
    }

    virtual ~storage_auto()
    {
    }

  private:
    static pvoid operator new[](size_t);    ///< prevents use of \c new[]
    static  void operator delete[](pvoid);  ///< prevents use of \c delete []
  };


 /** \def STORAGE_HEAP(Class)
  * Declares and defines a protected destructor.
  * Protected destructor prevents creation on the stack. Subclasses should
  * make their destructors also protected (or private).
  */
  #define STORAGE_HEAP(Class)   \
    protected: ~Class() {}

  /// An object that must be created on the heap.
  template<class T>
  class storage_heap: public storage_base<T>
  {
    SUPER(storage_base<T>)
    STORAGE_HEAP(storage_heap)

  private:
    void CheckHeap()
    {
      RUNTIME_CHECK_EXC2((super::QFlags() & super::flON_HEAP),
	                 "This object must be constructed on the heap.")
    }
  public:
    GETTER(IsOnHeap, bool, true)

    /// Detects if the object is on the heap: throws an exception otherwise.
    storage_heap(): super()
    {
      CheckHeap();
    }

    storage_heap(storage_heap<T> const& r): super(r)
    {
      CheckHeap();
    }

    storage_heap(storage_base<T> const& r): super(r)
    {
      CheckHeap();
    }

    storage_heap& operator=(storage_heap<T> const& r)
    {
      super::operator=(r);
      return *this;
    }

    storage_heap& operator=(storage_base<T> const& r)
    {
      super::operator=(r);
      return *this;
    }

    /// Since the destructor is not public,
    /// call <tt>p->Delete()</tt> instead of <tt>delete p</tt>.
    void Delete()
    {
      delete this;
    }

  private:
    static pvoid operator new[](size_t);    ///< prevents use of \c new[]
    static  void operator delete[](pvoid);  ///< prevents use of \c delete []
  };


  /// An object that must be created on the stack.
  template<class T>
  class storage_stack: public storage_base<T>
  {
    SUPER(storage_base<T>)

  private:
    void CheckStack()
    {
      RUNTIME_CHECK_EXC2(!(super::QFlags() & super::flON_HEAP),
	                 "This object must be constructed on the stack.")
    }

  public:
    GETTER(IsOnHeap, bool, false)

    /// Detects if the object is on the stack: throws an exception otherwise.
    storage_stack(): super()
    {
      CheckStack();
    }

    storage_stack(storage_stack<T> const& r): super(r)
    {
      CheckStack();
    }

    storage_stack(storage_base<T> const& r): super(r)
    {
      CheckStack();
    }

    storage_stack& operator=(storage_stack<T> const& r)
    {
      super::operator=(r);
      return *this;
    }

    storage_stack& operator=(storage_base<T> const& r)
    {
      super::operator=(r);
      return *this;
    }

    virtual ~storage_stack()
    {
    }

  private:
    static pvoid operator new(size_t);      ///< prevents use of \c new
    static pvoid operator new[](size_t);    ///< prevents use of \c new[]
    static  void operator delete[](pvoid);  ///< prevents use of \c delete []
  };
/**@}*/

//----------------------------------------------------------------------------

/** \name Reference counting and ref pointers
@{*/
  /** Reference counted object. If the reference count drops to zero, the
   * object destroys itself, if it thinks that it is on the heap.
   */
  class refcnt
  {
  private:
    /// reference counter
    mutable int m_cnt; //>>> uint?

  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS
    DECLARE_EXC_ROOT
    #undef EXC_IDS
#endif

    AGETTER(IsOnHeap,bool)

    /// default constructor
    refcnt()
      : m_cnt(0)
    {
    }

    /// copy constructor
    refcnt(refcnt const&)
      : m_cnt(0)
    {
    }

    // assignment
    refcnt& operator=(refcnt const&)
    {
      // do not copy m_cnt
      return *this;
    }

    virtual ~refcnt()
    {
    }

    uint inc_refcnt()  ///< increments the counter
    {
      return atomic_increment(m_cnt);
    }

    uint dec_refcnt()  ///< decrements the counter, possibly destructs self
    {
      uint cnt = atomic_decrement(m_cnt);
      if((0==cnt) && IsOnHeap())
        delete this;
      return cnt;
    }

    uint qry_refcnt()  ///< returns the counter value
    {
      return m_cnt;
    }
  };

  //>>> not sure if ..auto works
  typedef storage_auto<refcnt> refcnt_auto;
  typedef storage_heap<refcnt> refcnt_heap;

//----------------------------------------------------------------------------

  /// The base class of pointers to reference counted objects
  class refptr_base
  {
  protected:
    /// the pointed-to object
    refcnt *m_ptr;

  protected:
    /// 'NULL' pointer constructor
    refptr_base();
    /// copy constructor
    refptr_base(refptr_base const&);
    /// The constructor taking a pointed-to object
    refptr_base(refcnt*);

  public:
    /// may trigger destruction of the pointed-to object.
    virtual ~refptr_base()
    {
      release();
    }

  protected:
    refptr_base& operator=(refptr_base const&);
    refptr_base& operator=(refcnt*);

  public:
    /// Releases the pointed-to object; similar to NULL assignment for ordinary
    /// pointers.
    /// May trigger destruction of the pointed-to object.
    void release();
    /// Is the pointer null?
    bool isNull() const { return NULL==m_ptr; }

  private:
    /// increments the reference counter
    uint inc()
    {
      assert(m_ptr);
      return m_ptr->inc_refcnt();
    }

    /// decrements the reference counter
    uint dec()
    {
      assert(m_ptr);
      return m_ptr->dec_refcnt();
    }

  protected:
    /// compare (identity)
    inline bool operator==(refptr_base const& r)
    {
      return m_ptr == r.m_ptr;
    }

    /// \overload
    inline bool operator!=(refptr_base const& r)
    {
      return m_ptr != r.m_ptr;
    }

    /// \overload
    inline bool operator==(refcnt* ptr)
    {
      return m_ptr == ptr;
    }

    /// \overload
    inline bool operator!=(refcnt* ptr)
    {
      return m_ptr != ptr;
    }

  public:
    /// swap
    void swap(refptr_base& r);
  };

//----------------------------------------------------------------------------

  /// A typesafe pointer to reference counted objects.
  template <typename T>
  class refptr: public refptr_base
  {
    SUPER(refptr_base)

  public:
    /// no object pointed to (NULL)
    refptr(): super()
    {
    }

    /// a copy
    refptr(refptr<T> const& r): super(r)
    {
    }

    /// takes the adress of a pointed-to object, or NULL
    explicit refptr(refcnt* ptr): super(ptr)
    {
    }

    /// assignment
    refptr& operator=(refptr<T> const& r)
    {
      return static_cast<refptr&>(super::operator=(r));
    }

    /// assignment
    refptr& operator=(refcnt* ptr)
    {
      return static_cast<refptr&>(super::operator=(ptr));
    }

    /// a plain pointer
    T* ptr() const
    {
      return static_cast<T*>(m_ptr);
    }

    /// overloaded operator
    T& operator*() const
    {
      assert(m_ptr);
      return *static_cast<T*>(m_ptr);
    }

    /// overloaded operator
    T* operator->() const
    {
      assert(m_ptr);
      return static_cast<T*>(m_ptr);
    }

    /// compare pointed-to objects for identity
    inline bool operator==(refptr<T> const& r)
    {
      return super::operator==(r);
    }

    /// compare pointed-to objects for identity
    inline bool operator!=(refptr<T> const& r)
    {
      return super::operator!=(r);
    }

    /// compare pointed-to objects for identity
    inline bool operator==(refcnt* ptr)
    {
      return super::operator==(ptr);
    }

    /// compare pointed-to objects for identity
    inline bool operator!=(refcnt* ptr)
    {
      return super::operator!=(ptr);
    }
  };

  /// compare pointed-to objects for identity
  template <typename T>
  inline bool operator==(refcnt* ptr, refptr<T> const& r)
  {
    return r.operator==(ptr);
  }

  /// compare pointed-to objects for identity
  template <typename T>
  inline bool operator!=(refcnt* ptr, refptr<T> const& r)
  {
    return r.operator!=(ptr);
  }
/**@}*/

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

// eof
