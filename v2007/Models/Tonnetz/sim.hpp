/*****************************************************************************\
*    Copyright (c) 2006 by Ian G Burleigh           burleigh@cpsc.ucalgary.ca *
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
* $Id: sim.hpp 47 2006-06-21 10:34:49Z ian_b $
\*****************************************************************************/

/** \file
 */

#ifndef TONNETZ_SIM_HPP_INCLUDED
#define TONNETZ_SIM_HPP_INCLUDED

  #include <vigosim>

//-----------------------------------------------------------------------------

  /// Common simulation base class
  class Simulation: public vigo::Simulation
  {
    SUPER(vigo::Simulation)
    typedef Simulation *THIS;
    
  public:
    Simulation();
    
  private:
    virtual void SetFreq(float freq,bool just) = 0;
    static pvoid cb_set_eq(pvoid This) { ((THIS)This)->SetFreq(440.,false); return NULL; }
    static pvoid cb_set_jt(pvoid This) { ((THIS)This)->SetFreq(440.,true);  return NULL; }
    vigo::MenuAction m_setEq, m_setJt;
  };


//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
