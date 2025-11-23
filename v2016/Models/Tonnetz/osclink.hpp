// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

#ifndef TONNETZ_OSCLINK_HPP_INCLUDED
#define TONNETZ_OSCLINK_HPP_INCLUDED

  #include <vigocmn>
  #include <vigoosc>

//----------------------------------------------------------------------------

  /// SuperCollider link
  class SCLink: public vigo::osc::OSCSocket
  {
    SUPER(vigo::osc::OSCSocket)

  public:
    typedef uint32 no_t;
    enum {NUM_GROUPS = 1000};

    SCLink(uint appPort, pcstr scHost, uint scPort);
   ~SCLink();

    void InitSC(bool notify,int dump);
    void DoneSC();

  public:
    no_t NewGroup();
    void FreeGroup(no_t);

    void Send(pcstr);
    void Send(pcstr,int);
    void Send(pcstr,pcstr);

    void SendStatus()       	{ Send("/status");          	}
    void SendNotify(bool n) 	{ Send("/notify", n?1:0);   	}

    void SendQuit()          	{ Send("/quit");       		}

    void ReceiveReply(timeout_t=0/*msec*/);

  public:
    no_t NewTone(no_t groupNo,pcstr synth,float freq,float mean,float amp,
                 vigo::osc::TimeTag const& onset=vigo::osc::TimeTag::Immediate());
    void FreeTone(no_t nodeNo);

    void Set(no_t,float freq,float mean,float amp);

  private:
    no_t NewGroupNo();
    void FreeGroupNo(no_t);

    no_t NewNodeNo();
    void FreeNodeNo(no_t);

  private:
    vigo::util::splaytree_simple<no_t> m_groups, m_nodes;
  };


//----------------------------------------------------------------------------

#endif

// eof

