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
* $Id: timer.cpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

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


/*** eof ********************************************************************/
