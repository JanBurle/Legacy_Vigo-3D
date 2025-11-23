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
* $Id: osclink.cpp 47 2006-06-21 10:34:49Z ian_b $
\*****************************************************************************/

  #include "osclink.hpp"

  using namespace vigo;
  using osc::TimeTag;

//-----------------------------------------------------------------------------

  SCLink::SCLink(uint appPort, pcstr scHost, uint scPort)
    : super(appPort,scHost,scPort),
      m_groups(), m_nodes()
  {
    InitSC(true,1);
  }

  SCLink::~SCLink()
  {
    DoneSC();
  }

  void SCLink::InitSC(bool notify,int dump)
  {
    SendNotify(notify);
    Send("/dumpOSC",dump);
    ReceiveReply();
  }

  void SCLink::DoneSC()
  {
    ReceiveReply();
  }


  SCLink::no_t SCLink::NewGroup()
  {
    no_t gno = NewGroupNo();
    *this << osc::BeginMessage("/g_new") << (int32)gno << 1/*add to tail*/ << 0/* of root*/
          << osc::EndMessage << osc::SendMessage;
    return gno;
  }

  void SCLink::FreeGroup(no_t no)
  {
    *this << osc::BeginMessage("/n_free") << (int32)no
          << osc::EndMessage << osc::SendMessage;
    FreeGroupNo(no);
  }


  void SCLink::Send(pcstr s)
  {
    *this << osc::BeginMessage(s)
          << osc::EndMessage << osc::SendMessage;
  }


  void SCLink::Send(pcstr s, int i)
  {
    *this << osc::BeginMessage(s) << i
          << osc::EndMessage << osc::SendMessage;
  }


  void SCLink::Send(pcstr s, pcstr s2)
  {
    *this << osc::BeginMessage(s) << s2
          << osc::EndMessage << osc::SendMessage;
  }


  void SCLink::ReceiveReply(timeout_t tout)
  {
    for(;;)
    {
      osc::OSCPacket p = Receive(tout);
      if(0==p.Size())
        break;
      //TRACE("recv",p)
    }
  }
  

  SCLink::no_t SCLink::NewTone(no_t groupNo,pcstr synth,
                               float freq,float mean,float amp,
			       osc::TimeTag const& onset)
  {
    no_t no = NewNodeNo();
    *this << osc::BeginBundle(onset)
            << osc::BeginMessage("/s_new") << synth << (int)no << 1/*tail*/ << (int)groupNo
              << "freq" << freq << "mean" << mean << "amp" << amp
            << osc::EndMessage
          << osc::EndBundle << osc::SendMessage;
    return no;
  }


  void SCLink::FreeTone(no_t nodeNo)
  {
    *this << osc::BeginMessage("/n_free") << (int)nodeNo
          << osc::EndMessage << osc::SendMessage;
    FreeNodeNo(nodeNo);
  }


  void SCLink::Set(no_t no,float freq,float mean,float amp)
  {
    *this << osc::BeginMessage("/n_set") << (int)no 
          << "freq" << freq
          << "mean" << mean
          << "amp"  << amp
          << osc::EndMessage << osc::SendMessage;
  }

  
  SCLink::no_t SCLink::NewGroupNo()
  {
    no_t no = math::rand.UInt32(1,NUM_GROUPS);
    while(m_groups.find(no))
    {
      ++no; if(no>=NUM_GROUPS) no = 1; //>>> stalls if no group nos left
    }
    
    m_groups.insert(no);
    return no;
  }
  
  
  void SCLink::FreeGroupNo(no_t no)
  {
    m_groups.remove(no);
  }


  SCLink::no_t SCLink::NewNodeNo()
  {
    uint const MAX_NODE_NO = 0x80000000;
    no_t no = math::rand.UInt32(NUM_GROUPS,MAX_NODE_NO);
    while(m_nodes.find(no))
    {
      ++no; if(no>=MAX_NODE_NO) no = NUM_GROUPS;
    }

    m_nodes.insert(no);
    return no;
  }


  void SCLink::FreeNodeNo(no_t no)
  {
    m_nodes.remove(no);
  }
  

/*** eof ********************************************************************/
