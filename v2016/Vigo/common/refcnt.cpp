// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "./refcnt.hpp"

NAMESPACE_VIGO(util) //========================================================

  thread::tls& storage_base_HeapPending()
  {
    // must be done this way: ensure m_heapPending is constructed before it is
    // used while constructing global static :refcnt objects
    static thread::tls m_heapPending;
    return m_heapPending;
  }

//----------------------------------------------------------------------------

  refptr_base::refptr_base() : m_ptr(NULL)
  {
  }

  refptr_base::refptr_base(refptr_base const& r): m_ptr(r.m_ptr)
  {
    if(m_ptr)
      m_ptr->inc_refcnt();
  }

  refptr_base::refptr_base(refcnt* ptr): m_ptr(ptr)
  {
    if(m_ptr)
      m_ptr->inc_refcnt();
  }

  refptr_base& refptr_base::operator=(refptr_base const& r)
  {
    if(m_ptr!=r.m_ptr)
    {
      if(m_ptr)
        m_ptr->dec_refcnt();
      m_ptr = r.m_ptr;
      if(m_ptr)
        m_ptr->inc_refcnt();
    }

    return *this;
  }

  refptr_base& refptr_base::operator=(refcnt* ptr)
  {
    release();

    m_ptr = ptr;
    if(m_ptr)
      m_ptr->inc_refcnt();

    return *this;
  }

  void refptr_base::release()
  {
    if(m_ptr)
      m_ptr->dec_refcnt();

    m_ptr = NULL;
  }

  void refptr_base::swap(refptr_base& r)
  {
    vigo::swap(m_ptr,r.m_ptr);
  }

NAMESPACE_END() // eof
