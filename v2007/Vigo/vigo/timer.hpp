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
* $Id: timer.hpp 45 2006-06-21 10:20:33Z ian_b $
\*****************************************************************************/

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

//** eof **********************************************************************
