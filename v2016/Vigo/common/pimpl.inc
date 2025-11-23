// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief The "pointer to implementation" pattern.
 *
 */

/** \def PIMPL_IMPL
 * Links a method to its implementation.
 */
#define PIMPL_IMPL(name,type,argsin,argspass,constQ) \
  type PIMPL_FACE_CLASS::name argsin constQ \
  { return reinterpret_cast<PIMPL_IMPL_CLASS*>(m_pImpl)->name argspass; }

/** \def PIMPL_SIMPL
 * Links a static method to its implementation.
 */
#define PIMPL_SIMPL(name,type,argsin,argspass) \
  type PIMPL_FACE_CLASS::name argsin \
  { return PIMPL_IMPL_CLASS::name argspass; }

// eof
