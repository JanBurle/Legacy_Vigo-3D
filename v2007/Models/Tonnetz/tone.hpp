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
* $Id: tone.hpp 47 2006-06-21 10:34:49Z ian_b $
\*****************************************************************************/

/** \file
 */

#ifndef TONNETZ_TONE_HPP_INCLUDED
#define TONNETZ_TONE_HPP_INCLUDED

  #include <vigobase>
  #include "osclink.hpp"

//-----------------------------------------------------------------------------

  class Tone
  {
  private:
    static float m_mean;
  public:
    static float Mean() { return m_mean; }
    static SETTER(SetMean,float,m_mean)

    static float EqSemi(float freq,int steps);
    static float JtFfth(float freq,int steps);
      
  public:
    typedef uint tno;

    Tone(float freq)
      : m_freq(freq), m_sustain(false), m_nodeNo(0)
    {
    }

    GETSET(Freq,SetFreq,float,m_freq)

    void Toggle();
    void Start ();
    void Stop  ();
    GETTER(IsSound,bool,0!=m_nodeNo)
    
  private:
    float m_freq;
    bool  m_sustain;
    SCLink::no_t m_nodeNo;
  };

//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
