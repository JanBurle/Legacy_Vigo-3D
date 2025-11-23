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
* $Id: tagent.hpp 47 2006-06-21 10:34:49Z ian_b $
\*****************************************************************************/

/** \file
 */

#ifndef TONNETZ_TAGENT_HPP_INCLUDED
#define TONNETZ_TAGENT_HPP_INCLUDED

  #include <vigosim>
  #include "scene.hpp"
  #include "tone.hpp"

//-----------------------------------------------------------------------------

  typedef vigo::space::Node<float,vigo::parts::Parts<
                                  vigo::parts::P2<float>,
                                  vigo::parts::R0,
                                  vigo::parts::T0<float>,
                                  vigo::parts::SL,
                                  vigo::parts::A0
  > > TNode;

//-----------------------------------------------------------------------------

  class TAgent: public TNode
  {
    SUPER(TNode)
    
  public:
    TAgent(Scene&,float freq=440.,pcstr label="");
   ~TAgent();
   
    GETTER(Freq,float,m_tone.Freq())
    void SetFreq(float freq) { m_tone.SetFreq(freq); }

  protected:
    vigo::shape::Shape*      GetShape()      const;
    vigo::shape::Appearance* GetAppearance() const;

    bool Select(int kind, uint mod);
    bool Receive(ANode const*,vigo::space::Message&);
    
  private:
    Tone m_tone;
  };


//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
