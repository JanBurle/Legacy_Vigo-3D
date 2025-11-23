/*****************************************************************************\
*    Copyright (c) 2004--2007 by Ian G Burleigh                               *
*    https://github.com/phi53/Vigo-3D                                         *
*                                                                             *
*    This program is free software; you can redistribute it and/or modify     *
*    it under the terms of the GNU General Public License as published by     *
*    the Free Software Foundation; either version 2 of the License, or        *
*    (at your option) any later version.                                      *
*                                                                             *
*    This program is distributed in the hope that it will be useful,          *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*    GNU General Public License for more details.                             *
*                                                                             *
*    You should have received a copy of the GNU General Public License        *
*    along with this program; if not, write to the                            *
*    Free Software Foundation, Inc.,                                          *
*    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                *
*-----------------------------------------------------------------------------*
* $Id: timer.hpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

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

//** eof **********************************************************************
