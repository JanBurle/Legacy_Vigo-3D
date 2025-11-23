// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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


// eof
