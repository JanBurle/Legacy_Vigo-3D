// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

/** \file
 */

  #include "./types.hpp"

#ifdef _MSC_VER
  #include <windows.h>
#endif

  #include <arpa/inet.h>

  #include <stdarg.h>
  #include <string>

#ifdef __SUNOS__
  #include <stdio.h>
#endif

//----------------------------------------------------------------------------

  bool IsBigEndian()
  {
    static bool const isBigEndian = (htonl(1)==1);
    return isBigEndian;
  }

  uint64 htonll(uint64 x)
  {
    if(IsBigEndian()) return x;
    return (uint64(htonl(x))<<32) | uint64(htonl(x>>32));
  }

  uint64 ntohll(uint64 x)
  {
    if(IsBigEndian()) return x;
    return (uint64(ntohl(x))<<32) | uint64(ntohl(x>>32));
  }

//----------------------------------------------------------------------------

NAMESPACE_VIGO_() //**********************************************************

  std::string exc::What() const
  {
    std::string ids;
    switch(Id())
    {
      case EXC_NONE:
        ids = "";
        break;
      case EXC_SYSTEM:
        ids = "system";
        break;
      case EXC_ASSERT:
        ids = "assert";
        break;
      case EXC_NOT_IMPLEMENTED:
        ids = "not implemented";
        break;
      case EXC_NOT_HERE:
        ids = "not here";
        break;
      case EXC_RUNTIME:
        ids = "runtime check";
        break;
      default /*EXC_USER*/:
        ids = sformat("user %u",Id());
        break;
    }

    return sformat("Exception [%s]: %s",ids.c_str(),Msg().c_str());
  }

//----------------------------------------------------------------------------

  std::string sformat(pcstr format,...)
  {
    pstr txt = NULL;

    try
    {
      size_t SIZE = 1024;  // initial size

      for(;;)
      {
        delete [] txt;
        txt = new char[SIZE+1];

        va_list marker;
        va_start(marker,format);

        int cnt =
        #ifdef _MSC_VER
          _vsnprintf(txt,SIZE,format,marker);
        #else
           vsnprintf(txt,SIZE,format,marker);
        #endif

        va_end(marker);

        if(cnt<0)
        {
          SIZE *= 2;    // try again with a bigger buffer
        }
        else
        {
          txt[cnt] = 0; // if cnt==SIZE, _vsnprintf does not write '\0'
          break;
        }
      }
    }
    catch(...)
    {
      delete [] txt;
      throw;
    }

    std::string s(txt);
    delete [] txt;

    return s;
  }

  static pcstr usualWhiteSpace = " \f\n\r\t\v";

  std::string trimleft(std::string const& s, pcstr whiteSpace)
  {
    if(!whiteSpace) whiteSpace = usualWhiteSpace;

    std::string::size_type f = s.find_first_not_of(whiteSpace);
    if(std::string::npos==f)
      return std::string();
    else
      return s.substr(f);
  }

  std::string trimright(std::string const& s, pcstr whiteSpace)
  {
    if(!whiteSpace) whiteSpace = usualWhiteSpace;

    std::string::size_type l = s.find_last_not_of(whiteSpace);
    if(std::string::npos==l)
      return std::string();
    return s.substr(0,l+1);
  }

  std::string trim(std::string const& s, pcstr whiteSpace)
  {
    if(!whiteSpace) whiteSpace = usualWhiteSpace;

    std::string::size_type f = s.find_first_not_of(whiteSpace);
    if(std::string::npos==f)
      return std::string();

    std::string::size_type l = s.find_last_not_of(whiteSpace);
    return s.substr(f,l+1-f);
  }

NAMESPACE_END_() // eof
