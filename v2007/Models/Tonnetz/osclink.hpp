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
* $Id: osclink.hpp 47 2006-06-21 10:34:49Z ian_b $
\*****************************************************************************/

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

/*** eof ********************************************************************/

