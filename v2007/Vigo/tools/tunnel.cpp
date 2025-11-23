/*****************************************************************************\
* This file is a part of Vigo::3D, a multi-agent simulation and visualization *
* system.                                                                     *
*                                                                             *
* Copyright (c) 2002--2006 by Ian G. Burleigh       burleigh@cpsc.ucalgary.ca *
* burleigh@cpsc.ucalgary.ca                         http://ard-galen.org/vigo *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for   *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the                                     *
*   Free Software Foundation, Inc.,                                           *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
*-----------------------------------------------------------------------------*
* $Id: socket.hpp 41 2006-06-07 22:44:11Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief
 */

  #include "../common/socket.hpp"
  #include "../common/async.hpp"

  using vigo::util::TCPSocket;
  using vigo::util::TCPChannel;

#ifdef WIN32
  #include <winsock2.h>
#else
  // #include <netdb.h>
  // #include <sys/socket.h>
  #include <sys/select.h>
  // #include <netinet/in.h>
  // #include <arpa/inet.h>
  #include <errno.h>
#endif

//-----------------------------------------------------------------------------

  class Tunnel
  {
    NO_GEN(Tunnel)

  public:
    Tunnel(TCPSocket accepted,pcstr name,ushort port);
    virtual ~Tunnel();

  protected:
    TCPSocket  m_acceptedSocket;
    TCPSocket  m_connectedSocket;
  };


//-----------------------------------------------------------------------------

  Tunnel::Tunnel(TCPSocket accepted,pcstr name,ushort port)
    : m_acceptedSocket(accepted), m_connectedSocket(0)
  {
	m_connectedSocket.Connect(name,port);
	std::cout << "\n-->-->-->-->-->-->-->-->-->-->\n";
	while(true)
	{
		byte b; TCPSocket::timeout_t tout = 10;
		while(TCPSocket::SOCK_RECEIVED==m_acceptedSocket.Receive(b,tout))
		{
			std::cout << b;
			m_connectedSocket.Send(b);
		}
		while(TCPSocket::SOCK_RECEIVED==m_connectedSocket.Receive(b,tout))
		{
			std::cout << b;
			m_acceptedSocket.Send(b);
		}
	}
	std::cout << "\n--<--<--<--<--<--<--<--<--<--<\n";
  }


  Tunnel::~Tunnel()
  {
  }


  void help()
  {
    printf("Usage:\n"
           "  tunnel <port in> <ip out> <port out>\n"
           "e.g.\n"
           "  tunnel 81 127.0.0.1 80\n");
  }

  int main(int argc,pcstr argv[])
  {
    if(4!=argc)
    {
      help(); return -1;
    }

    ushort portIn  = (ushort)atoi(argv[1]);
    pcstr  ipOut   = argv[2];
    ushort portOut = (ushort)atoi(argv[3]);

    if(0==portIn || 0==portOut || !(ipOut && *ipOut))
    {
      help(); return -1;
    }

    printf("Tunneling port %i --> %s:%i\n",portIn,ipOut,portOut);
    printf("-----------------------------------------\n");

    TCPChannel channel;

    channel.Open(portIn);
    channel.Listen();
	Tunnel(channel.Accept(),ipOut,portOut);

    return 0;
  }


//* EOF ***********************************************************************
