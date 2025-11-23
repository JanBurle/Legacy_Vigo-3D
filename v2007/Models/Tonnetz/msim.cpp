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
* $Id: msim.cpp 47 2006-06-21 10:34:49Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "msim.hpp"
  #include "mscene.hpp"

//-----------------------------------------------------------------------------

  MoebiusSim::MoebiusSim()
    : super(),
      m_actR(cb_rhn, this, &m_slideR),
      m_actH(cb_rhn, this, &m_slideH),
      m_actN(cb_rhn, this, &m_slideN),
      m_slideR(1), m_slideH(1),
      m_actPlayUpSemi("up 1/2",cb_ups,this,NULL,'q'),
      m_actPlayDnSemi("dn 1/2",cb_dns,this,NULL,'a'),
      m_actPlayUp5   ("up 5",  cb_up5,this,NULL,'w'),
      m_actPlayDn5   ("dn 5",  cb_dn5,this,NULL,'s')
  {
    GetDock()->AddSlider("R", m_actR,1,30,10,true);
    GetDock()->AddSlider("H", m_actH,1,10, 1,true);
    GetDock()->AddSlider("N", m_actN,1, 6, 2,true);
    
    GetMenu()->Add(m_actPlayUpSemi);
    GetMenu()->Add(m_actPlayDnSemi);
    GetMenu()->Add(m_actPlayUp5);
    GetMenu()->Add(m_actPlayDn5);
  }


  void MoebiusSim::SetFreq(float freq,bool just)
  {
    MoebiusScene* ms = dynamic_cast<MoebiusScene*>(Scene());
    if(ms) 
    {
      ms->SetFreq(freq,just);
      UpdateGL();
    }
  }
  
  
  void MoebiusSim::RHN()
  {
    MoebiusScene* ms = dynamic_cast<MoebiusScene*>(Scene());
    if(ms) 
    {
      ms->SetRHN(m_slideR,m_slideH,m_slideN);
      UpdateGL();
    }
  }
  
  
  void MoebiusSim::Play(int what)
  {
    MoebiusScene* ms = dynamic_cast<MoebiusScene*>(Scene());
    if(ms) 
    {
      ms->Play(what);
    }
  }
  

//** eof **********************************************************************
