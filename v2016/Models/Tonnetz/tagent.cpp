// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

// eof
