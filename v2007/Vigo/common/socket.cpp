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
* $Id: socket.cpp 209 2007-03-20 14:50:16Z ian_b $
\*****************************************************************************/

/** \file
 */

  #include "./socket.hpp"
  #include <stdarg.h>

#ifdef WIN32
  #include <winsock2.h>
#else
  #include <netdb.h>
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <errno.h>
#endif

#ifdef __SUNOS__
  #include <strings.h>
  #include <unistd.h>
#endif

#ifdef WIN32
  #define THROW_ERROR(err,msg) THROW_EXC2(err,std::string(msg)+"|"+WSA::LastError())
#else
  #define THROW_ERROR(err,msg) THROW_EXC2(err,std::string(msg)+"|"+strerror(errno))
#endif

#ifndef INADDR_NONE
  #define INADDR_NONE 0xffffffff
#endif

NAMESPACE_VIGO(util) //========================================================

#ifdef WIN32
  NAMESPACE_VIGO(WSA)

  static bool    isStarted = false;
  static WSADATA wsaData;

  // API error codes
  struct sErrType
  {
    uint  errCode;
    pcstr errString;
  }
  errType[] =
  {
    { 10013, "Permission denied"                                },
    { 10048, "Address already in use"                           },
    { 10049, "Cannot assign requested address"                  },
    { 10047, "Address family not supported by protocol family"  },
    { 10037, "Operation already in progress"                    },
    { 10053, "Software caused connection abort"                 },
    { 10061, "Connection refused"                               },
    { 10054, "Connection reset by peer"                         },
    { 10039, "Destination address required"                     },
    { 10014, "Bad address"                                      },
    { 10064, "Host is down"                                     },
    { 10065, "No route to host"                                 },
    { 10036, "Operation now in progress"                        },
    { 10004, "Interrupted function call"                        },
    { 10022, "Invalid argument"                                 },
    { 10056, "Socket is already connected"                      },
    { 10024, "Too many open files"                              },
    { 10040, "Message too long"                                 },
    { 10050, "Network is down"                                  },
    { 10052, "Network dropped connection on reset"              },
    { 10051, "Network is unreachable"                           },
    { 10055, "No buffer space available"                        },
    { 10042, "Bad protocol option"                              },
    { 10057, "Socket is not connected"                          },
    { 10038, "Socket operation on non-socket"                   },
    { 10045, "Operation not supported"                          },
    { 10046, "Protocol family not supported"                    },
    { 10067, "Too many processes"                               },
    { 10043, "Protocol not supported"                           },
    { 10041, "Protocol wrong type for socket"                   },
    { 10058, "Cannot send after socket shutdown"                },
    { 10044, "Socket type not supported"                        },
    { 10060, "Connection timed out"                             },
    { 10109, "Class type not found"                             },
    { 10035, "Resource temporarily unavailable"                 },
    { 11001, "Host not found"                                   },
    { 10093, "Successful WSAStartup not yet performed"          },
    { 11004, "Valid name, no data record of requested type"     },
    { 11003, "This is a non-recoverable error"                  },
    { 10091, "Network subsystem is unavailable"                 },
    { 11002, "Non-authoritative host not found"                 },
    { 10092, "WINSOCK.DLL version out of range"                 },
    { 10094, "Graceful shutdown in progress"                    },
    { 0, NULL}
  };

  class WSAStarter
  {
  public:
    WSAStarter();
   ~WSAStarter();
  };

  WSAStarter::WSAStarter()
  {
    if(0 == ::WSAStartup(MAKEWORD(1,1),&wsaData))
    {
      isStarted = true;
    }
  }

  WSAStarter::~WSAStarter()
  {
    if(isStarted)
    {
      ::WSACleanup();
      isStarted = false;
    }
  }

  bool Start()
  {
    static WSAStarter wsaStarter;
    return isStarted;
  }

  bool IsStarted()
  {
    return isStarted;
  }

  pcstr LastError()
  {
    static int lastError = 0;
    sErrType *p   = errType;

    int err = WSAGetLastError();
    if(0!=err)
      lastError = err;

    for(;;)
    {
      int errCode = p->errCode;

      if(0==errCode)
        break;

      if(lastError==errCode)
        return p->errString;

      ++p;
    }

    return NULL;
  }

  NAMESPACE_END()

#else // not WIN32

  static int const INVALID_SOCKET = -1;
  static int const SOCKET_ERROR   = -1;
#endif

//-----------------------------------------------------------------------------

  SocketBase::SocketBase(sock_t sock, uint sendBufferSize)
    : m_sock(sock), m_port(0),
      m_sendBufferSize((sendBufferSize>0) ? sendBufferSize : 1)
  {
  #ifdef WIN32
    RUNTIME_CHECK_EXC3(WSA::Start(),SOCKETS_NOT_STARTED,"sockets not started")
  #endif
  }


  SocketBase::~SocketBase()
  {
  }


  bool SocketBase::IsValid() const
  {
    return INVALID_SOCKET!=m_sock;
  }


  bool SocketBase::WaitForData(timeout_t msec)
  {
    RUNTIME_CHECK_EXC3(IsValid(),SOCKET_NOT_VALID,"invalid socket")
  #ifdef WIN32
    FD_SET readfds;
  #else
    fd_set readfds;
  #endif
    FD_ZERO(&readfds);
    FD_SET(m_sock,&readfds);

    timeval timeout = {msec/1000,               // sec
                       1000*(msec%1000)};    // usec

    if(SOCKET_ERROR==select(m_sock+1,&readfds,NULL,NULL,&timeout))
    {
      THROW_ERROR(RECV_FAILED,"select failed")
    }

    return (0!=FD_ISSET(m_sock,&readfds));
  }


  void SocketBase::Close_()
  {
    m_sock = INVALID_SOCKET;
    m_port = 0;
  }


  SocketBase::IPAddress SocketBase::Addr(pcstr host)
  {
	if(!empty(host))
	{
      IPAddress addr = inet_addr(host);
      if(INADDR_NONE==addr)
      {
        hostent *hent = gethostbyname(host);
        RUNTIME_CHECK_EXC3(hent,CANNOT_RESOLVE_HOST,"cannot resolve host")
        addr = ((in_addr*)hent->h_addr)->s_addr;
      }

      return addr;
    }

    return htonl(INADDR_ANY);
  }

//-----------------------------------------------------------------------------

  TCPSocket::TCPSocket(uint sendBufferSize)
    : super(INVALID_SOCKET,sendBufferSize),
      m_buffer(m_sendBufferSize), m_buffered(0),
      m_receiveClosed(false)
  {
  }


  TCPSocket::TCPSocket(sock_t sock,uint sendBufferSize)
    : super(sock,sendBufferSize),
      m_buffer(m_sendBufferSize), m_buffered(0),
      m_receiveClosed(false)
  {
  }


  TCPSocket::TCPSocket(TCPSocket const& socket)
    : super(socket.m_sock,socket.m_sendBufferSize),
      m_buffer(m_sendBufferSize), m_buffered(0),
      m_receiveClosed(false)
  {
  }


  TCPSocket::~TCPSocket()
  {
    Close(true);
  }


  void TCPSocket::Connect(pcstr name,ushort port)
  {
    Close();

    RUNTIME_CHECK_EXC3(!empty(name),EMPTY_NAME,"empty host name")

    typedef unsigned long IPAddress;

    sockaddr_in saddr; bzero((pvoid)&saddr,sizeof(saddr));
    saddr.sin_family      = AF_INET;
    saddr.sin_port        = htons(port);
    saddr.sin_addr.s_addr = Addr(name);

    m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(!IsValid()) THROW_ERROR(CANNOT_CREATE_SOCKET,"cannot create socket")

    if(SOCKET_ERROR==connect(m_sock,(sockaddr*)&saddr,sizeof(saddr)))
    {
      Close();
      THROW_ERROR(CANNOT_CONNECT,"cannot connect")
    }

    m_port = port;
  }


  void TCPSocket::Close(bool dontThrow)
  {
    if(IsValid())
    {
      try{ Flush(); }
      catch (...)
      {
      	if(!dontThrow) throw;
      }

    #ifdef WIN32
      closesocket(m_sock);
    #else
      close(m_sock);
    #endif
    }

    super::Close_();
  }


  void TCPSocket::Send(byte b)
  {
    RUNTIME_CHECK_EXC3(IsValid(),SOCKET_NOT_VALID,"invalid socket")
    if(m_buffered>=m_buffer.size())
      Flush();
    m_buffer[m_buffered++] = b;
  }


  void TCPSocket::Send(pcvoid data,uint size)
  {
    assert(NULL!=data);

    pcbyte p = (pcbyte)data;
    while(size>0)
    {
      Send(*(p++));
      size--;
    }
  }


  void TCPSocket::Send(pcstr buf)
  {
    assert(NULL!=buf);

    byte b;
    while(0!=(b = *(buf++)))
    {
      Send(b);
    }
  }


  void TCPSocket::Flush()
  {
    if(0==m_buffered) return;
    SendNonBuffered(m_buffer.ptr(),m_buffered);
    m_buffered=0;
  }


  TCPSocket::eStatus TCPSocket::Receive(byte& b,timeout_t msec)
  {
    uint received;
    return Receive(&b,1,msec,received);
  }


  TCPSocket::eStatus TCPSocket::Receive(pvoid buf,uint bufSize,timeout_t msec,uint& received)
  {
    RUNTIME_CHECK_EXC3(IsValid(),SOCKET_NOT_VALID,"invalid socket")
    assert(NULL!=buf);
    assert(!m_receiveClosed);

    if(!WaitForData(msec))
      return SOCK_TIMEOUT;

    sockaddr_in  replyAddr;
    socklen_t replyAddrLength = sizeof(replyAddr);

    int lgt = recvfrom(m_sock,buf,bufSize,0,(sockaddr*)&replyAddr,&replyAddrLength);
    switch(lgt)
    {
    case 0:
      m_receiveClosed = true;
      return SOCK_CLOSED;

    case SOCKET_ERROR:
      THROW_ERROR(RECV_FAILED,"receive failed")
    }

    assert(lgt>=0);
    received = lgt;

    return SOCK_RECEIVED;
  }


  TCPSocket::eStatus TCPSocket::ReceiveLine(std::string& line,timeout_t msec)
  {
    line.erase();

    if(m_receiveClosed)
      return SOCK_CLOSED;

    std::string s;

    for(;;)
    {
      char c;

      uint received;
      eStatus status = Receive(&c,1,msec,received);
      if(SOCK_RECEIVED==status)
      {
        assert(1==received);
        if('\r'==c) continue;   // dump CR
        if('\n'==c) break;
        s += c;
      }
      else
      if(SOCK_CLOSED==status)
      {
        if(s.empty())
          return SOCK_CLOSED;
        else
          break;
      }
      else
      {
        assert(SOCK_TIMEOUT==status);
        return SOCK_TIMEOUT;
      }
    }

    line = s;
    return SOCK_RECEIVED;
  }


  static char HexChar(uint value)
  {
    static char hexChar[16]=
    {
      '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
    };

    return hexChar[value & 0xF];
  }


  void TCPSocket::SendNonBuffered(pcvoid buf,uint count)
  {
    assert(NULL!=buf);
    RUNTIME_CHECK_EXC3(IsValid(),SOCKET_NOT_VALID,"invalid socket")

    pcstr pb = (pcstr)buf;
    if(SOCKET_ERROR==send(m_sock,pb,count,0))
    {
      THROW_ERROR(SEND_FAILED,"send failed")
    }
  }

//-----------------------------------------------------------------------------

  std::string HTTPSocket::CgiEncode(pcstr p)
  {
    assert(NULL!=p);

    static pcstr cgiSpecial = "<>#%{}|^~[]`;?:@=&!$+-\'\"";

    std::string s;
    unsigned char c;

    while(0 != (c=*(p++)))
    {
      if(' '==c)
      {
        s += '+';
        continue;
      }

      if(c<' ' || c>=(unsigned char)127 || NULL!=strchr(cgiSpecial,c))
      {
        ((s+='%') += HexChar(c>>4)) += HexChar(c);
        continue;
      }

      s += c;
    }

    return s;
  }


  std::string HTTPSocket::MakeQuery(pcstr pairs,...)
  {
    std::string s;

    va_list marker;
    va_start(marker,pairs);

    pcstr p = pairs;
    while(NULL!=p)
    {
      if(!s.empty())
        s += '&';

      (s+=p) += '=';
      p = va_arg(marker,pcstr); assert(p);
      s+= CgiEncode(p);
      p = va_arg(marker,pcstr);
    }

    va_end(marker);

    return s;
  }


  std::string HTTPSocket::SendQuery(pcstr host, ushort port, pcstr url, pcstr query, timeout_t msec)
  {
    assert(host && url);

    std::string get(url);
    if(!empty(query))
      (get += '?') += query;

    util::TCPSocket socket(128); // 128 bytes buffer
    socket.Connect(host,port);
    socket.Send(sformat("GET %s HTTP/1.0\n"
                        "Host: %s\n"
                        // "User-Agent: ...\n",
                        "\n",
                        get.c_str(), host));
    socket.Flush();

    std::string line, res;
    bool header = true; // strip headers
    while(util::TCPSocket::SOCK_RECEIVED==socket.ReceiveLine(line,msec))
    {
      if(header)
      {
        if(line.empty())
          header  = false; // end of headers

        continue;
      }

      res += (line + '\n');
    }

    return res;
  }


//-----------------------------------------------------------------------------

  TCPChannel::TCPChannel(uint sendBufferSize)
    : super(INVALID_SOCKET,sendBufferSize)
  {
  }


  TCPChannel::~TCPChannel()
  {
    Close(true);
  }


  void TCPChannel::Open(pcstr host,ushort port,bool reusePort)
  {
    Close();

    // create socket
    m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(!IsValid()) THROW_ERROR(CANNOT_CREATE_SOCKET,"cannot create socket")

    if(reusePort)
    {
      int optOn = 1;
      setsockopt(m_sock,SOL_SOCKET,SO_REUSEADDR,&optOn,sizeof(optOn));
    }

    sockaddr_in saddr; bzero((pvoid)&saddr,sizeof(saddr));
    saddr.sin_family      = AF_INET;
    saddr.sin_port        = htons(port);
    saddr.sin_addr.s_addr = Addr(host);

    if(SOCKET_ERROR==bind(m_sock,(sockaddr*)&saddr,sizeof saddr))
    {
      Close();
      THROW_ERROR(CANNOT_BIND,"cannot bind socket")
    }

    m_port = port;
  }


  void TCPChannel::Close(bool /*dontThrow*/)
  {
    if(IsValid())
    {
    #ifdef WIN32
      closesocket(m_sock);
    #else
      close(m_sock);
    #endif
    }

    super::Close_();
  }


  void TCPChannel::Listen()
  {
    RUNTIME_CHECK_EXC3(IsValid(),SOCKET_NOT_VALID,"socket is not valid")
    if(SOCKET_ERROR==listen(m_sock,SOMAXCONN))
      THROW_ERROR(CANNOT_LISTEN,"cannot listen")
  }


  TCPSocket TCPChannel::Accept()
  {
    int sock = accept(m_sock,NULL,NULL);
    RUNTIME_CHECK_EXC3(INVALID_SOCKET!=sock,CANNOT_ACCEPT,"cannot accept")

    return TCPSocket(sock,m_sendBufferSize);
  }


//----------------------------------------------------------------------------

  UDPSocket::UDPSocket()
    : super(INVALID_SOCKET,0)
  {
  }


  UDPSocket::~UDPSocket()
  {
    Close(true);
  }

  void UDPSocket::Open(ushort port)
  {
    Close();

    // create socket
    m_sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(!IsValid()) THROW_ERROR(CANNOT_CREATE_SOCKET,"cannot create socket")

    sockaddr_in saddr; bzero((pvoid)&saddr,sizeof(saddr));
    saddr.sin_family      = AF_INET;
    saddr.sin_port        = htons(port);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(SOCKET_ERROR==bind(m_sock,(sockaddr*)&saddr,sizeof saddr))
    {
      Close();
      THROW_ERROR(CANNOT_BIND,"cannot bind socket")
    }

    m_port = port;
  }

//>>> TODO Close(), Open() etc. merge with TCP(Channel)
  void UDPSocket::Close(bool /*dontThrow*/)
  {
    if(IsValid())
    {
    #ifdef WIN32
      closesocket(m_sock);
    #else
      close(m_sock);
    #endif
    }

    super::Close_();
  }


  void UDPSocket::SetSendAddress(pcstr host,ushort port)
  {
    RUNTIME_CHECK_EXC3(!empty(host),EMPTY_NAME,"empty host name")

    bzero(&m_saddr,sizeof(m_saddr));
    m_saddr.sin_family      = AF_INET;
    m_saddr.sin_port        = htons(port);
    m_saddr.sin_addr.s_addr = Addr(host);
  }


  void UDPSocket::SetSendAddress(sockaddr_in& s)
  {
    m_saddr = s;
  }


  void UDPSocket::Send(pcvoid data,uint size)
  {
    SendTo(data,size,m_saddr);
  }


  void UDPSocket::SendTo(pcvoid data,uint size,sockaddr_in& sa)
  {
    RUNTIME_CHECK_EXC3(IsValid(),SOCKET_NOT_VALID,"invalid socket")
    ssize_t ss = sendto(m_sock,data,size,0,(sockaddr*)&sa, sizeof(sa));
    if(ss!=(ssize_t)size) THROW_ERROR(SEND_FAILED,"send failed")
  }


  void UDPSocket::Receive(pvoid buf,uint size,timeout_t msec,uint& received,sockaddr_in* replyAddr)
  {
    RUNTIME_CHECK_EXC3(IsValid(),SOCKET_NOT_VALID,"invalid socket")

    if(!WaitForData(msec))
    {
      received = 0; // timeout
      return;
    }

    socklen_t replyAddrLength = sizeof(sockaddr_in);
    ssize_t res = recvfrom(m_sock,buf,size,0,(sockaddr*)replyAddr,&replyAddrLength);
    if(SOCKET_ERROR==res)
      THROW_ERROR(RECV_FAILED,"receive failed")
    received = (uint)res;
  };


NAMESPACE_END() //** eof ******************************************************
