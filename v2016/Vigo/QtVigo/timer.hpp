// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

#ifndef VIGO_QTVIGO_TIMER_HPP_INCLUDED
#define VIGO_QTVIGO_TIMER_HPP_INCLUDED

  #include "../vigo/timer.hpp"
  #include "../common/pimpl.hpp"
  #include <QtCore/QTimer>

//-----------------------------------------------------------------------------

  class QtVigoTimer_: public QTimer,
                      protected vigo::internal::pimpl_iface<vigo::Timer>
  {
    SUPER(QTimer)
    NO_GEN(QtVigoTimer_)
    Q_OBJECT

  public:
    QtVigoTimer_(vigo::Timer& iface);
   ~QtVigoTimer_();

    void Start(uint msec);
    void Stop();
    void SingleShot();

  private slots:
    void Timeout();
  };

//-----------------------------------------------------------------------------

#endif

// eof
