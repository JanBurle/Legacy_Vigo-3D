// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "./timer.hpp"

//-----------------------------------------------------------------------------

  QtVigoTimer_::QtVigoTimer_(vigo::Timer& iface)
    : vigo::internal::pimpl_iface<vigo::Timer>(iface)
  {
    if(!connect(this,SIGNAL(timeout()),this,SLOT(Timeout())))
      vigo::throw_exc(vigo::exc(vigo::exc::EXC_RUNTIME,
                                "connect SLOT(timeout()) failed"));
  }


  QtVigoTimer_::~QtVigoTimer_()
  {
  }


  void QtVigoTimer_::Start(uint msec)
  {
    super::start(msec);
  }


  void QtVigoTimer_::Stop()
  {
    super::stop();
  }


  void QtVigoTimer_::SingleShot()
  {
    super::singleShot(0,this,SLOT(Timeout()));
  }


  void QtVigoTimer_::Timeout()
  {
    m_iface.Triggered();
  }


// eof
