/*****************************************************************************\
* This file is a part of Vigo::3D, a multi-agent simulation and visualization *
* system.                                                                     *
*                                                                             *
* Copyright (c) 2002--2006 by Ian G. Burleigh       burleigh@cpsc.ucalgary.ca *
* burleigh@cpsc.ucalgary.ca                         http://ard-galen.org/vigo *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for   *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the                                     *
*   Free Software Foundation, Inc.,                                           *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
*-----------------------------------------------------------------------------*
* $Id: timer.cpp 45 2006-06-21 10:20:33Z ian_b $
\*****************************************************************************/

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

NAMESPACE_END_() //** eof *****************************************************
