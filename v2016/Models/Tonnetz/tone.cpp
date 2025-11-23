// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "tone.hpp"
  #include "app.hpp"

//-----------------------------------------------------------------------------

  float Tone::m_mean = 880.;

  float Tone::EqSemi(float freq,int steps)
  {
    static float root12 = pow(2,1/12.);
    return freq * pow(root12,steps);
  }

  float Tone::JtFfth(float freq,int steps)
  {
    float f = freq;
    while(steps>0)   { f*=1.5; --steps; }
    while(steps<0)   { f/=1.5; ++steps; }
    while(f<freq )   { f*=2.; }
    while(f>freq*2.) { f/=2.; }
    return f;
  }

  void Tone::Toggle()
  {
    if(!m_sustain)
    {
      Start(); m_sustain = true;
    }
    else
    {
      Stop();  m_sustain = false;
    }
  }


  void Tone::Start()
  {
    SCLink& link = Application::SC();
    if(!m_sustain && !m_nodeNo)
      m_nodeNo = link.NewTone(0,"Tonnetz",Freq(),Mean(),.1);
  }


  void Tone::Stop()
  {
    SCLink& link = Application::SC();
    if(!m_sustain && m_nodeNo)
    {
      link.FreeTone(m_nodeNo); m_nodeNo = 0;
    }
  }


//-----------------------------------------------------------------------------
/*

  class TriadAgent;

  class ToneAgent: public TAgent
  {
    SUPER(TAgent)

  protected:
    static shape::Shape& Shape();

    enum eState {stOFF, stSEL=0x1, stPLY=0x2, stPLS=stSEL+stPLY};
    static shape::Appearance& Appearance(eState);

  protected:
    ToneAgent(TonnetzScene&,Note const&);
  public:
   ~ToneAgent();

    static ToneAgent& New(TonnetzScene&,Note const&);
    // with cutoff frequencies
    static ToneAgent* NewWithCutoff(TonnetzScene&,Note const&);

    bool Act();
    bool Select(int kind, uint mod);

    void UpdateAppearance();

    GETTER(GetNote,Note const&,m_note)

    void StartPlay();
    void StopPlay();

    void Play(double duration);

    GETTER(IsPlaying, bool, m_isPlaying)

  public:
    bool Receive(ANode const* sender,space::Message&);
    bool ReceiveBroadcast(ANode const* sender, space::Message const&);

    void MakeNeighbours();

    void ConnectTone(uint i,ToneAgent&);
    void DisconnectTone(uint i);
    void DisconnectTone(ToneAgent*);

  private:
    void DrawSelf(Camera const& cam,Pointd const& pos);

  private:
    union
    {
      ToneAgent *ptone[6];
      struct {
        ToneAgent *_5up, *M3up, *m3dn, *_5dn, *M3dn, *m3up;
      } pt;
    }
    m_nbr;

    TriadAgent *m_tr[6];

    Note m_note;
    uint m_scNode;

    bool m_isPlaying; double m_offTime;

    bool et12;
  };

  shape::Shape& ToneAgent::Shape()
  {
    static shape::Shape *sh = new shape::Disk(TONE_AGENT_RADIUS);
    return *sh;
  }


  shape::Appearance& ToneAgent::Appearance(eState state)
  {
    static util::refptr<shape::Appearance> appOff(new shape::Appearance(0.,.7,.7));
    static util::refptr<shape::Appearance> appSel(new shape::Appearance(.2,1.,1.));
    static util::refptr<shape::Appearance> appPly(new shape::Appearance(.7,.7,0.));
    static util::refptr<shape::Appearance> appPls(new shape::Appearance(1.,1.,0.));
    switch(state)
    {
    default:    return *appOff;
    case stSEL: return *appSel;
    case stPLY: return *appPly;
    case stPLS: return *appPls;
    }
  }


  ToneAgent::ToneAgent(TonnetzScene& scene,Note const& note)
    : super(scene), m_nbr(), m_tr(), m_note(note),
      //>>>m_scNode(scene.SC().NextNode()),
      m_isPlaying(false), m_offTime(0.), et12(false)
  {
    bzero(&m_nbr, sizeof(m_nbr));
    bzero(&m_tr,  sizeof(m_tr));
  }


  ToneAgent::~ToneAgent()
  {
    for(uint i=0; i<6; ++i)
      DisconnectTone(i);
  }


  ToneAgent& ToneAgent::New(TonnetzScene& scene,Note const& note)
  {
    ToneAgent* ta = new ToneAgent(scene,note);
    //ta->SetShape(Shape());
    ta->UpdateAppearance();
    return *ta;
  }


  ToneAgent* ToneAgent::NewWithCutoff(TonnetzScene& scene,Note const& note)
  {
    if(note.freq>=params::Tones::lowCutoff
    && note.freq<=params::Tones::highCutoff && abs(note.numCommas)<5/ *>>>* /)
      return &New(scene,note);
    return NULL;
  }


  bool ToneAgent::Act()
  {return false;//!!!
    bool res = false;
    switch(ActPhase())
    {
    case 0: // stop timed play
      if(m_offTime>0. && m_offTime<=SimTime())
      {
        StopPlay(); res=true;
      }
      break;
    }

    return super::Act() || res;
  }


  bool ToneAgent::Select(int kind, uint / *mod* /)
  {return false;//!!!
    enum {selMOVEOUT=-2,selUNCLICK=-1,selMOVE=0,selCLICK=1, selDBLCLICK=2};
    switch(kind)
    {
    case selCLICK:
      if(IsPlaying()) StopPlay();
      else StartPlay();
    case selMOVEOUT:
    case selMOVE:
      UpdateAppearance();
      return true;
    }

    return false;
  }


  void ToneAgent::UpdateAppearance()
  {return;//!!!
    int st = stOFF;

    if(IsSelected())
      st |= stSEL;
    if(IsPlaying())
      st |= stPLY;

    //SetAppearance(Appearance((eState)st));
    //UpdateFrame();
  }


  void ToneAgent::StartPlay()
  {
    m_isPlaying = true;
    m_offTime   = 0.;
    ((TonnetzScene&)GetScene()).StartPlay(m_scNode,et12?m_note.freqET12:m_note.freq);
    UpdateAppearance();
  }

  void ToneAgent::StopPlay()
  {
    m_isPlaying = false;
    m_offTime   = 0.;
    ((TonnetzScene&)GetScene()).StopPlay(m_scNode);
    UpdateAppearance();
  }


  void ToneAgent::Play(double duration)
  {
    double t = SimTime()+duration;
    if(!m_isPlaying)
    {
      m_isPlaying = true;
      m_offTime   = t;
      ((TonnetzScene&)GetScene()).StartPlay(m_scNode,et12?m_note.freqET12:m_note.freq);
      UpdateAppearance();
    }
    else
    {
      // update timed play
      if(m_offTime>0.) m_offTime = max(m_offTime,t);
    }
  }


  bool ToneAgent::Receive(ANode const* / *sender* /,space::Message& msg)
  {
    switch(msg.id)
    {
    case MSG_MAKENBR:
      MakeNeighbours();
      return true;
    }
    return false;
  }


  bool ToneAgent::ReceiveBroadcast(ANode const*,space::Message const& msg)
  {
    switch(msg.id)
    {
    case MSG_JUST:
      //SetLabel(m_note.ToString(false).c_str()); et12=false;
      return true;
    case MSG_ET12:
      //SetLabel(m_note.ToString(true).c_str()); et12=true;
      return true;
    }
    return false;
  }


  void ToneAgent::MakeNeighbours()
  {
    // notes
    for(uint i=0; i<6; ++i)
    {
      ToneAgent *& tr = m_nbr.ptone[i];
      if(!tr)
      {
        Note n;
        switch(i)
        {
        case 0:
          n = m_note._5up();
          break;
        case 1:
          n = m_note.M3up();
          break;
        case 2:
          n = m_note.m3dn();
          break;
        case 3:
          n = m_note._5dn();
          break;
        case 4:
          n = m_note.M3dn();
          break;
        case 5:
          n = m_note.m3up();
          break;
        }

        ToneAgent *ta = NewWithCutoff((TonnetzScene&)GetScene(),n);
        if(!ta) continue;

        GetScene().Root().Add(*ta);
        ConnectTone(i,*ta);
#ifndef TORUS
        Vectorf v = Vectorf::X * TONE_AGENT_DISTANCE;
        v = v.Rotate(Rotation(i*algebra3d::deg2rad(60),Vectord::Z));
        ta->SetPos(Pos()+v);
#else
#endif
        SendAsync(*ta,space::Message(MSG_MAKENBR)); // propagate
      }
    }

    for(uint n=0; n<6; ++n)
    {
      ToneAgent *ta = m_nbr.ptone[n];
      if(ta) ta->ConnectTone((n+2)%6,*m_nbr.ptone[(n+1)%6]);
    }

    // triads
    for(uint i=0; i<6; ++i)
    {
      TriadAgent *&tr = m_tr[i]; ToneAgent *&t1=m_nbr.ptone[i], *&t2=m_nbr.ptone[(i+1)%6];
      if(!tr && t1 && t2)
      {
        TonnetzScene &sc = (TonnetzScene&)GetScene();
        tr = &TriadAgent::New(sc,*this,*t1,*t2);
        sc.Root().Add(*tr);

#ifndef TORUS
        Vector<spct> v1 = t1->Pos() - Pos(),
                                v2 = t2->Pos() - Pos();
        tr->SetPos(Pos()+v1/5.+v2/5.);
#else
        //tr->SetSurface(*(t1->GetSurface()));
        //spct iu = (2*U() + t1->U() + t2->U())/4;
        //spct iv = (2*V() + t1->V() + t2->V())/4;
        //tr->Set(iu,iv);
#endif
      }
    }
  }


  void ToneAgent::ConnectTone(uint i,ToneAgent& ta)
  {
    assert(i<6);
    ToneAgent *& tr = m_nbr.ptone[i];
    if(tr!=&ta)
    {
      DisconnectTone(i);
      tr = &ta;
      ta.ConnectTone((i+3)%6,*this);
    }
  }


  void ToneAgent::DisconnectTone(uint i)
  {
    assert(i<6);
    ToneAgent *& tr = m_nbr.ptone[i];
    ToneAgent *ta = tr; tr = NULL;
    if(ta) ta->DisconnectTone(this);
  }


  void ToneAgent::DisconnectTone(ToneAgent* ta)
  {
    if(!ta) return;

    for(uint i=0; i<6; ++i)
    {
      ToneAgent *& tr = m_nbr.ptone[i];
      if(tr==ta)
      {
        DisconnectTone(i);
        return;
      }
    }
  }


  void ToneAgent::DrawSelf(Camera const& cam,Pointd const& pos)
  {
    super::DrawSelf(cam,pos);
    for(uint i=0; i<6; ++i)
    {
      ToneAgent *ta = m_nbr.ptone[i];
      if(ta)
      {
        glColor3d(0.,.8,.8);

        Vectord v(ta->Pos()-Pos()),
                           vr(v.Unit()*TONE_AGENT_RADIUS*1.1);
        Pointd  a = Pointd::_0 + vr,
                           b = Pointd::_0 + v - vr;

        glBegin(GL_LINES);
        glVertex3d(a.x,a.y,a.z);
        glVertex3d(b.x,b.y,b.z);
        glEnd();
      }
    }
  }

*/
// eof
