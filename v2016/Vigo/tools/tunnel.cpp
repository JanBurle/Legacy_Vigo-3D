// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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


// eof
