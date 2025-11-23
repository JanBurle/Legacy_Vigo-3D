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
* $Id: msim.hpp 47 2006-06-21 10:34:49Z ian_b $
\*****************************************************************************/

/** \file
 */

#ifndef TONNETZ_MSIM_HPP_INCLUDED
#define TONNETZ_MSIM_HPP_INCLUDED

  #include "sim.hpp"

//-----------------------------------------------------------------------------

  class MoebiusSim: public Simulation
  {
    SUPER(Simulation)
    typedef MoebiusSim *THIS;
    
  public:
    MoebiusSim();

  private:
    void SetFreq(float freq,bool just);
    void RHN();
    static pvoid cb_rhn(pvoid This) { ((THIS)This)->RHN();   return NULL; }
    vigo::DataAction<int> m_actR, m_actH, m_actN;
    int m_slideR, m_slideH, m_slideN;

    void Play(int);
    static pvoid cb_ups(pvoid This) { ((THIS)This)->Play(0); return NULL; }    
    static pvoid cb_dns(pvoid This) { ((THIS)This)->Play(1); return NULL; }
    static pvoid cb_up5(pvoid This) { ((THIS)This)->Play(2); return NULL; }
    static pvoid cb_dn5(pvoid This) { ((THIS)This)->Play(3); return NULL; }
    vigo::MenuAction m_actPlayUpSemi, m_actPlayDnSemi,
                     m_actPlayUp5, m_actPlayDn5;
  };


//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
