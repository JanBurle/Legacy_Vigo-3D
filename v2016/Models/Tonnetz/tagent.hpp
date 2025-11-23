// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

// eof
