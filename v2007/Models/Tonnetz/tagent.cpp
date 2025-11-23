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
* $Id: tagent.cpp 47 2006-06-21 10:34:49Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "tagent.hpp"
  using namespace vigo;

//-----------------------------------------------------------------------------

  TAgent::TAgent(Scene& scene,float freq,pcstr label)
    : super(scene), m_tone(freq)
  {
    SetLabel(label);
    Register(regMSG);
  }


  TAgent::~TAgent()
  {
  }

  
  shape::Shape* TAgent::GetShape() const
  {
    static shape::Sphere sphere(.3);
    return &sphere;
  }
  
  
  shape::Appearance* TAgent::GetAppearance() const
  {
    enum eState {stOFF, stSEL=0x1, stPLY=0x2, stPLS=stSEL+stPLY};
    int st = stOFF;

    if(IsSelected())
      st |= stSEL;
    if(m_tone.IsSound())
      st |= stPLY;

    static util::refptr<shape::Appearance> appOff(new shape::Appearance(0.,.7,.7));
    static util::refptr<shape::Appearance> appSel(new shape::Appearance(.2,1.,1.));
    static util::refptr<shape::Appearance> appPly(new shape::Appearance(.7,.7,0.));
    static util::refptr<shape::Appearance> appPls(new shape::Appearance(1.,1.,0.));

    switch(st)
    {
    default:    return appOff.ptr();
    case stSEL: return appSel.ptr();
    case stPLY: return appPly.ptr();
    case stPLS: return appPls.ptr();
    }
  }
  
  
  bool TAgent::Select(int kind, uint /*mod*/)
  {
    enum {selMOVEOUT=-2,selUNCLICK=-1,selMOVE=0,selCLICK=1,selDBLCLICK=2};
    switch(kind)
    {
    case selCLICK:
      m_tone.Toggle();
      break;
    case selMOVE:
      m_tone.Start();
      break;
    case selMOVEOUT:
      m_tone.Stop();
      break;
    default:
      return false;
    }

    UpdateFrame();
    return true;
  }

  bool TAgent::Receive(ANode const*,vigo::space::Message& msg)
  {
    switch(msg.Id())
    {
    case 1:
      m_tone.Start();
      break;
    case 0:
      m_tone.Stop();
      break;
    default:
      return false;
    }
    
    UpdateFrame();
    return false;
  }
  
//** eof **********************************************************************
