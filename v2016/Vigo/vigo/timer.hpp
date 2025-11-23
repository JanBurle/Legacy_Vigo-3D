// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 *  \brief A timer class
 */

#ifndef VIGO_VIGO_TIMER_HPP_INCLUDED
#define VIGO_VIGO_TIMER_HPP_INCLUDED

  #include "../common/pimpl.hpp"
  #include "./actions.hpp"

  class QtVigoTimer_;

NAMESPACE_VIGO_() //===========================================================
/** \defgroup grp_timer_hpp Timer
@{*/

  class Timer: public internal::pimpl<QtVigoTimer_>
  {
    NO_GEN(Timer)

  public:
    Timer(uint timeout, AbstractAction&);
   ~Timer();

    void Start();
    void Stop();
    void SingleShot();
    void Reset();

    GETTER(TotalTime, uint64, m_totalTime)
    GETSET(Timeout, SetTimeout, uint, m_timeout)

  private:
    friend class ::QtVigoTimer_;
    void Triggered();

  private:
    uint            m_timeout;   // msec
    uint64          m_totalTime; // msec
    AbstractAction &m_action;
  };

/**@} (defgroup)*/
NAMESPACE_END_() //============================================================

#endif

// eof
