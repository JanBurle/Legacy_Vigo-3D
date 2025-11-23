// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief The "pointer to implementation" pattern.
 *
 */

#ifndef VIGO_COMMON_PIMPL_HPP_INCLUDED
#define VIGO_COMMON_PIMPL_HPP_INCLUDED

  #include "./types.hpp"

NAMESPACE_VIGO(internal) //====================================================
// These classes are for internal use.

  // The pImpl pattern.
  template <class Impl>
  class pimpl
  {
  public:
    pimpl(Impl* p=NULL): m_pImpl(p) { }
    virtual ~pimpl()                { delete m_pImpl; }

    void pImpl(Impl* impl)  { m_pImpl = impl;                    }
    void pImpl(Impl& impl)  { m_pImpl = &impl;                   }
    Impl& pImpl()           { assert(m_pImpl); return *m_pImpl;  }

  protected:
    Impl* m_pImpl;
  };

  // The backlink to the interface object.
  template <class Interface>
  class pimpl_iface
  {
  public:
    pimpl_iface(Interface& iface): m_iface(iface) { }

  protected:
    Interface &m_iface;
  };


NAMESPACE_END() //=============================================================

#endif

// eof
