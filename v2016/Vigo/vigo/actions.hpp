// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief Vigo actions and controls
 */

#ifndef VIGO_VIGO_ACTIONS_HPP_INCLUDED
#define VIGO_VIGO_ACTIONS_HPP_INCLUDED

  #include "../common/types.hpp"

NAMESPACE_VIGO_() //===========================================================
/** \defgroup grp_actions_hpp Actions
@{*/

  class AbstractAction
  {
  public:
    AbstractAction()          {}
    virtual ~AbstractAction() {}

    /// Execute the action.
    virtual pvoid operator()() = 0;
  };


  class CallbackAction: public AbstractAction
  {
    SUPER(AbstractAction)
  public:
    typedef pvoid param;
    typedef pvoid (*pfun)(param);

  public:
    CallbackAction(pfun pf, param par): super(), m_pfun(pf), m_param(par) {}
    virtual pvoid operator()()
    {
      return m_pfun ? m_pfun(m_param) : NULL;
    }

  protected:
    pfun m_pfun; param m_param;
  };


  template<typename T>
  class DataAction: public CallbackAction
  {
    SUPER(CallbackAction)
  public:
    typedef T *pdata;

  public:
    DataAction(pfun pf, param par, pdata pd): super(pf,par), m_pdata(pd) {}
    GETTER(PData,   T*,   m_pdata)

    /// Modify the data and call back.
    virtual pvoid operator()(T data)
    {
      if(m_pdata) *m_pdata = data;
      return super::operator()();
    }

  protected:
    pdata m_pdata;
  };


  typedef DataAction<bool> ToggleAction;

/**@} (defgroup)*/
NAMESPACE_END_() //============================================================

#endif

// eof
