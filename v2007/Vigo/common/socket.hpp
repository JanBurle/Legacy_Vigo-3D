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
* $Id: socket.hpp 209 2007-03-20 14:50:16Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Portable tcp and udp socket wrappers.
 */

#ifndef VIGO_COMMON_SOCKET_HPP_INCLUDED
#define VIGO_COMMON_SOCKET_HPP_INCLUDED

  #include "./types.hpp"
  #include "./array.hpp"

#ifdef WIN32
  #include <winsock2.h>
#else
  //#include <sys/socket.h>
  #include <netinet/in.h>
#endif

NAMESPACE_VIGO(util) //========================================================
/** \defgroup grp_sockets_hpp Sockets
@{*/

  class SocketBase
  {
    NO_GEN(SocketBase)

  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS SOCKETS_NOT_STARTED,EMPTY_NAME,CANNOT_CREATE_SOCKET,CANNOT_RESOLVE_HOST,CANNOT_CONNECT,CANNOT_BIND,SOCKET_NOT_VALID,CANNOT_LISTEN,CANNOT_ACCEPT,SEND_FAILED,RECV_FAILED
    DECLARE_EXC_ROOT
    #undef EXC_IDS
#endif

    typedef uint timeout_t;                     // [msec]
    static timeout_t const FOREVER = (uint)-1;  // wait really long

  #ifdef WIN32
    typedef uint sock_t;
  #else
    typedef int  sock_t;
  #endif

    SocketBase(sock_t sock, uint sendBufferSize);
    virtual ~SocketBase();

    GETTER(Sock,sock_t,m_sock)

    bool IsValid() const;

    virtual void Close(bool dontThrow) = 0;

    bool WaitForData(timeout_t msec);

  protected:
    void Close_();

    sock_t  m_sock;
    uint16  m_port;
    uint    m_sendBufferSize;

    typedef unsigned long IPAddress;
    /// INADDR_ANY if empty(host)
    IPAddress Addr(pcstr host);
  };
 
//----------------------------------------------------------------------------

  class TCPSocket: public SocketBase
  {
    NO_ASSIGN(TCPSocket)
    SUPER(SocketBase)

  public:
    /** Create the socket.
     *  @param sendBufferSize The size of the socket buffer. 0: non-buffered, >0: call Flush() to send
     */
    TCPSocket(uint sendBufferSize);        // 0..nonbuffered
    TCPSocket(sock_t,uint sendBufferSize);
    TCPSocket(TCPSocket const&);
   ~TCPSocket();

    void Connect(pcstr name,ushort port);
    void Close(bool dontThrow=false);

    void Send(byte);
    void Send(pcvoid data,uint size);
    void Send(pcstr);
    void Send(std::string const& s)
    {
      Send(s.c_str());
    }

    void Flush();

  public:
    enum eStatus
    {
      SOCK_RECEIVED,
      SOCK_CLOSED,
      SOCK_TIMEOUT
    };

    eStatus Receive(byte&,timeout_t msec);
    eStatus Receive(pvoid buf,uint size,timeout_t msec,uint& received);
    eStatus ReceiveLine(std::string& line,timeout_t msec);

  private:
    void SendNonBuffered(pcvoid buf,uint size);

    array1d<byte> m_buffer;
    uint          m_buffered;
    bool          m_receiveClosed;
  };

//-----------------------------------------------------------------------------

  class HTTPSocket: public TCPSocket
  {
    NO_GEN(HTTPSocket)
    SUPER(TCPSocket)

  public:
    HTTPSocket(uint sendBufferSize): super(sendBufferSize)
    {
    }

    static std::string CgiEncode(pcstr); ///< Encode text for cgi query.
    static std::string MakeQuery(pcstr,...);

    static std::string SendQuery(pcstr host,ushort port,pcstr url,pcstr query,timeout_t msec);
  };

//-----------------------------------------------------------------------------

  class TCPChannel: public SocketBase
  {
    NO_GEN(TCPChannel)
    SUPER(SocketBase)

  public:
    TCPChannel(uint sendBufferSize=0); // for accepted socket
   ~TCPChannel();

    /// host may be empty
    void Open(pcstr host,ushort port,bool reusePort=false);
    void Close(bool dontThrow=false);

    void      Listen();
    TCPSocket Accept();
  };

//----------------------------------------------------------------------------

  class UDPSocket: public SocketBase
  {
    NO_GEN(UDPSocket)
    SUPER(SocketBase)

  public:
    UDPSocket();
   ~UDPSocket();

    void Open(ushort port);
    void Close(bool dontThrow=false);

    void SetSendAddress(pcstr host,ushort port);
    void SetSendAddress(sockaddr_in&);

    void Send(pcvoid data,uint size);
    void SendTo(pcvoid data,uint sizei,sockaddr_in&);

    void Receive(pvoid buf,uint size,timeout_t msec,uint& received,
                 sockaddr_in* replyAddr=NULL);

  private:
    struct sockaddr_in m_saddr;
  };


/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

//** eof **********************************************************************
