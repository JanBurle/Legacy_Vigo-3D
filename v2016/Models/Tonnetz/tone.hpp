// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

// eof
