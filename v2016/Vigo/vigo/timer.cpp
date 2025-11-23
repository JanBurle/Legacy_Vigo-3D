// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

  #include "./timer.hpp"
  #include "../QtVigo/timer.hpp"

NAMESPACE_VIGO_() //***********************************************************

  Timer::Timer(uint timeout, AbstractAction& action)
    : internal::pimpl<QtVigoTimer_>(new QtVigoTimer_(*this)),
      m_timeout(timeout), m_totalTime(0), m_action(action)
  {
  }


  Timer::~Timer()
  {
  }


  void Timer::Reset()
  {
    Stop(); m_totalTime = 0;
  }


  void Timer::Triggered()
  {
    m_totalTime += m_timeout;
    m_action();
  }

#define PIMPL_FACE_CLASS Timer
#define PIMPL_IMPL_CLASS QtVigoTimer_
#include "../common/pimpl.inc"

  PIMPL_IMPL(Start,     void, (),(m_timeout),)
  PIMPL_IMPL(Stop,      void, (),(),)
  PIMPL_IMPL(SingleShot,void, (),(),)

NAMESPACE_END_() // eof
