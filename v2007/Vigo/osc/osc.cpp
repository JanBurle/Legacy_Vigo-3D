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
* $Id: osc.cpp 175 2006-11-06 02:54:43Z ian_b $
\*****************************************************************************/

/** \file
 */

// adapted from oscpack; the original copyright notice follows:
/*
        oscpack -- Open Sound Control packet manipulation library
        http://www.audiomulch.com/~rossb/oscpack
        
        Copyright (c) 2004 Ross Bencina <rossb@audiomulch.com>
        
        Permission is hereby granted, free of charge, to any person obtaining
        a copy of this software and associated documentation files
        (the "Software"), to deal in the Software without restriction,
        including without limitation the rights to use, copy, modify, merge,
        publish, distribute, sublicense, and/or sell copies of the Software,
        and to permit persons to whom the Software is furnished to do so,
        subject to the following conditions:
        
        The above copyright notice and this permission notice shall be
        included in all copies or substantial portions of the Software.
        
        Any person wishing to distribute modifications to the Software is
        requested to send the modifications to the original developer so that
        they can be incorporated into the canonical version.
        
        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
        EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
        MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
        IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
        ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
        CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
        WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

  #include "./osc.hpp"
  #include <sys/time.h>
  #include <iostream>
  #include <iomanip>

#ifdef __SUNOS__
#include <alloca.h>
#endif

#define OSC_HOST_BIG_ENDIAN 1
#undef OSC_HOST_LITTLE_ENDIAN

/*>>>TODO Make sure either OSC_HOST_LITTLE_ENDIAN or OSC_HOST_BIG_ENDIAN is defined

    If you know a way to enhance the detection below for Linux and/or MacOSX
    please let me know! I've tried a few things which don't work.
*/

//>>> reformat and optimize THROW's etc.

NAMESPACE_VIGO(osc) //*********************************************************

  BundleInitiator   BeginBundleImmediate(1);
  BundleTerminator  EndBundle;
  MessageTerminator EndMessage;
  MessageSender     SendMessage;
  NilType           Nil;
  InfinitumType     Infinitum;

//-----------------------------------------------------------------------------

  static void FromInt32(pstr p, int32 x)
  {
  #ifdef OSC_HOST_LITTLE_ENDIAN
    union
    {
        int32 i;
        char  c[4];
    } u;

    u.i  = x;

    p[3] = u.c[0];
    p[2] = u.c[1];
    p[1] = u.c[2];
    p[0] = u.c[3];
  #else
    *reinterpret_cast<int32*>(p) = x;
  #endif
  }


  static void FromUInt32(pstr p, uint32 x)
  {
  #ifdef OSC_HOST_LITTLE_ENDIAN
    union
    {
        uint32 i;
        char   c[4];
    } u;

    u.i  = x;

    p[3] = u.c[0];
    p[2] = u.c[1];
    p[1] = u.c[2];
    p[0] = u.c[3];
  #else
    *reinterpret_cast<uint32*>(p) = x;
  #endif
  }


  static void FromInt64(pstr p, int64 x)
  {
  #ifdef OSC_HOST_LITTLE_ENDIAN
    union
    {
        int64 i;
        char  c[8];
    } u;

    u.i  = x;

    p[7] = u.c[0];
    p[6] = u.c[1];
    p[5] = u.c[2];
    p[4] = u.c[3];
    p[3] = u.c[4];
    p[2] = u.c[5];
    p[1] = u.c[6];
    p[0] = u.c[7];
  #else
    *reinterpret_cast<int64*>(p) = x;
  #endif
  }


  static void FromUInt64(pstr p, uint64 x)
  {
  #ifdef OSC_HOST_LITTLE_ENDIAN
    union
    {
        uint64 i;
        char   c[8];
    } u;

    u.i  = x;

    p[7] = u.c[0];
    p[6] = u.c[1];
    p[5] = u.c[2];
    p[4] = u.c[3];
    p[3] = u.c[4];
    p[2] = u.c[5];
    p[1] = u.c[6];
    p[0] = u.c[7];
  #else
    *reinterpret_cast<uint64*>(p) = x;
  #endif
  }


  static inline long RoundUp4(long x )
  {
    return ((x-1) & (~0x03L)) + 4;
  }

//-----------------------------------------------------------------------------

  static uint64 const SECONDS_FROM_1900_to_1970 = 2208988800ul;
  static double const SECONDS_TO_OSC            = 4294967296.0; // 2^32
  static double const MICROSECS_TO_OSC          = SECONDS_TO_OSC * 1e-6;
    
  TimeTag::TimeTag(double d)
    : value(uint64(d*SECONDS_TO_OSC))
  {
  }

  TimeTag& TimeTag::operator+=(TimeTag const& t)
  {
    value += t.value;
    return *this;
  }

  TimeTag  TimeTag::operator+ (TimeTag const& t) const
  {
    return TimeTag(*this)+=t;
  }

  TimeTag& TimeTag::operator-=(TimeTag const& t)
  {
    value -= t.value;
    return *this;
  }

  TimeTag  TimeTag::operator- (TimeTag const& t) const
  {
    return TimeTag(*this)-=t;
  }


  double TimeTag::AsDouble() const
  {
    return double(value)/SECONDS_TO_OSC;
  }
    
  TimeTag TimeTag::Now()
  {
    struct timeval tv;
    gettimeofday(&tv,NULL);

    static uint64 const OSC_ADJUST = uint64(14000.0*MICROSECS_TO_OSC);
    
    return
       TimeTag(((uint64(tv.tv_sec) + SECONDS_FROM_1900_to_1970) << 32)
               + uint64(tv.tv_usec * MICROSECS_TO_OSC + OSC_ADJUST));
  }

  std::ostream& operator<<(std::ostream& os,osc::TimeTag const& tt)
  {
    os << "T" << tt.AsDouble();
    return os;
  }

//-----------------------------------------------------------------------------

  OutboundPacketStream::OutboundPacketStream(pstr buffer, ulong capacity)
    : m_data(buffer), m_end(m_data + capacity),
      m_typeTagsCurrent(m_end), m_messageCursor(m_data), m_argumentCurrent(m_data),
      m_elementSizePtr(NULL), m_messageIsInProgress(false)
  {
  }


  OutboundPacketStream::~OutboundPacketStream()
  {
  }


  void OutboundPacketStream::Clear()
  {
    m_typeTagsCurrent     = m_end;
    m_messageCursor       = m_data;
    m_argumentCurrent     = m_data;
    m_elementSizePtr      = NULL;
    m_messageIsInProgress = false;
  }


  ulong OutboundPacketStream::Capacity() const
  {
    return m_end - m_data;
  }


  ulong OutboundPacketStream::Size() const
  {
    ulong result = m_argumentCurrent - m_data;
    if(IsMessageInProgress())
    {
        // account for the length of the type tag string. the total type tag
        // includes an initial comma, plus at least one terminating \0
        result += RoundUp4((m_end - m_typeTagsCurrent) + 2);
    }

    return result;
  }


  pcstr OutboundPacketStream::Data() const
  {
    return m_data;
  }


  bool OutboundPacketStream::IsReady() const
  {
    return (!IsMessageInProgress() && !IsBundleInProgress());
  }


  bool OutboundPacketStream::IsMessageInProgress() const
  {
    return m_messageIsInProgress;
  }


  bool OutboundPacketStream::IsBundleInProgress() const
  {
    return (NULL!=m_elementSizePtr);
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(BundleInitiator const& rhs)
  {
    RUNTIME_CHECK_EXC3(!IsMessageInProgress(), MESSAGE_IN_PROGRESS,
                       "Message is in progress")

    CheckForAvailableBundleSpace();

    m_messageCursor = BeginElement(m_messageCursor);
    memcpy(m_messageCursor, "#bundle\0", 8 );
    FromUInt64(m_messageCursor + 8, rhs.timeTag );

    m_messageCursor  += 16;
    m_argumentCurrent = m_messageCursor;

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(BundleTerminator const& rhs)
  {
    (void) rhs;

    RUNTIME_CHECK_EXC3(IsBundleInProgress(), BUNDLE_NOT_IN_PROGRESS,
                       "Bundle not in progress")
    RUNTIME_CHECK_EXC3(!IsMessageInProgress(), MESSAGE_IN_PROGRESS,
                       "Message is in progress")

    EndElement(m_messageCursor);

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(BeginMessage const& rhs)
  {
    RUNTIME_CHECK_EXC3(!IsMessageInProgress(), MESSAGE_IN_PROGRESS,
                       "Message is in progress")

    CheckForAvailableMessageSpace(rhs.addressPattern);

    m_messageCursor = BeginElement(m_messageCursor);

    strcpy(m_messageCursor, rhs.addressPattern);
    m_messageCursor += strlen(rhs.addressPattern) + 1;
    while(reinterpret_cast<int>(m_messageCursor) & 0x03)
        *m_messageCursor++ = '\0';

    m_argumentCurrent = m_messageCursor;
    m_typeTagsCurrent = m_end;

    m_messageIsInProgress = true;

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(MessageTerminator const& rhs)
  {
    (void) rhs;

    RUNTIME_CHECK_EXC3(IsMessageInProgress(), MESSAGE_NOT_IN_PROGRESS,
                       "Message not in progress")

    ulong typeTagsCount = m_end - m_typeTagsCurrent;

    if(typeTagsCount)
    {
      pstr tempTypeTags = (pstr)alloca(typeTagsCount);
      memcpy(tempTypeTags, m_typeTagsCurrent, typeTagsCount);

      // slot size includes comma and null terminator
      int typeTagSlotSize = RoundUp4(typeTagsCount + 2);

      uint32 argumentsSize = m_argumentCurrent - m_messageCursor;
      memmove(m_messageCursor + typeTagSlotSize, m_messageCursor, argumentsSize);

      m_messageCursor[0] = ',';
      // copy type tags in reverse (really forward) order
      for(uint i=0; i < typeTagsCount; ++i)
        m_messageCursor[i+1] = tempTypeTags[ (typeTagsCount-1) - i ];

      pstr p = m_messageCursor + 1 + typeTagsCount;
      for(uint i=0; i < (typeTagSlotSize - (typeTagsCount + 1)); ++i)
        *p++ = '\0';

      m_typeTagsCurrent = m_end;

      // advance messageCursor_ for next message
      m_messageCursor += typeTagSlotSize + argumentsSize;
    }
    else
    {
      // send an empty type tags string
      memcpy(m_messageCursor, ",\0\0\0", 4);

      // advance messageCursor_ for next message
      m_messageCursor += 4;
    }

    m_argumentCurrent = m_messageCursor;
    EndElement(m_messageCursor);
    m_messageIsInProgress = false;

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(bool rhs)
  {
    CheckForAvailableArgumentSpace(0);

    *(--m_typeTagsCurrent) = (char)((rhs) ? TRUE_TYPE_TAG : FALSE_TYPE_TAG);

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(NilType const& rhs)
  {
    (void) rhs;
    CheckForAvailableArgumentSpace(0);

    *(--m_typeTagsCurrent) = NIL_TYPE_TAG;

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(InfinitumType const& rhs)
  {
    (void) rhs;
    CheckForAvailableArgumentSpace(0);

    *(--m_typeTagsCurrent) = INFINITUM_TYPE_TAG;

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(int32 rhs)
  {
    CheckForAvailableArgumentSpace(4);

    *(--m_typeTagsCurrent) = INT32_TYPE_TAG;
    FromInt32( m_argumentCurrent, rhs );
    m_argumentCurrent += 4;

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(float rhs)
  {
    CheckForAvailableArgumentSpace(4);

    *(--m_typeTagsCurrent) = FLOAT_TYPE_TAG;

  #ifdef OSC_HOST_LITTLE_ENDIAN
    union
    {
      float f;
      char  c[4];
    } u;

    u.f = rhs;

    m_argumentCurrent[3] = u.c[0];
    m_argumentCurrent[2] = u.c[1];
    m_argumentCurrent[1] = u.c[2];
    m_argumentCurrent[0] = u.c[3];
  #else
    *reinterpret_cast<float*>(m_argumentCurrent) = rhs;
  #endif

    m_argumentCurrent += 4;

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(char rhs)
  {
    CheckForAvailableArgumentSpace(4);

    *(--m_typeTagsCurrent) = CHAR_TYPE_TAG;
    FromInt32(m_argumentCurrent, rhs);
    m_argumentCurrent += 4;

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(RgbaColor const& rhs)
  {
    CheckForAvailableArgumentSpace(4);

    *(--m_typeTagsCurrent) = RGBA_COLOR_TYPE_TAG;
    FromUInt32(m_argumentCurrent, rhs);
    m_argumentCurrent += 4;

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(MidiMessage const& rhs)
  {
    CheckForAvailableArgumentSpace(4);

    *(--m_typeTagsCurrent) = MIDI_MESSAGE_TYPE_TAG;
    FromUInt32(m_argumentCurrent, rhs);
    m_argumentCurrent += 4;

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(int64 rhs)
  {
    CheckForAvailableArgumentSpace(8);

    *(--m_typeTagsCurrent) = INT64_TYPE_TAG;
    FromInt64(m_argumentCurrent, rhs);
    m_argumentCurrent += 8;

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(TimeTag const& rhs)
  {
    CheckForAvailableArgumentSpace(8);

    *(--m_typeTagsCurrent) = TIME_TAG_TYPE_TAG;
    FromUInt64(m_argumentCurrent, rhs.value);
    m_argumentCurrent += 8;

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(double rhs)
  {
    CheckForAvailableArgumentSpace(8);

    *(--m_typeTagsCurrent) = DOUBLE_TYPE_TAG;

  #ifdef OSC_HOST_LITTLE_ENDIAN
    union
    {
      double f;
      char   c[8];
    } u;

    u.f = rhs;

    m_argumentCurrent[7] = u.c[0];
    m_argumentCurrent[6] = u.c[1];
    m_argumentCurrent[5] = u.c[2];
    m_argumentCurrent[4] = u.c[3];
    m_argumentCurrent[3] = u.c[4];
    m_argumentCurrent[2] = u.c[5];
    m_argumentCurrent[1] = u.c[6];
    m_argumentCurrent[0] = u.c[7];
  #else
    *reinterpret_cast<double*>(m_argumentCurrent) = rhs;
  #endif

    m_argumentCurrent += 8;

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(pcstr rhs)
  {
    CheckForAvailableArgumentSpace(RoundUp4(strlen(rhs) + 1));

    *(--m_typeTagsCurrent) = STRING_TYPE_TAG;
    strcpy(m_argumentCurrent, rhs);
    m_argumentCurrent += strlen(rhs) + 1;
    while(reinterpret_cast<int>(m_argumentCurrent) & 0x03)
      *m_argumentCurrent++ = '\0';

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(Symbol const& rhs)
  {
    CheckForAvailableArgumentSpace(RoundUp4(strlen(rhs) + 1));

    *(--m_typeTagsCurrent) = SYMBOL_TYPE_TAG;
    strcpy(m_argumentCurrent, rhs);
    m_argumentCurrent += strlen(rhs) + 1;
    while(reinterpret_cast<int>(m_argumentCurrent) & 0x03)
      *m_argumentCurrent++ = '\0';

    return *this;
  }


  OutboundPacketStream& OutboundPacketStream::operator<<(Blob  const& rhs)
  {
    CheckForAvailableArgumentSpace(4 + RoundUp4(rhs.size));

    *(--m_typeTagsCurrent) = BLOB_TYPE_TAG;
    FromUInt32(m_argumentCurrent, rhs.size);
    m_argumentCurrent += 4;
    memcpy(m_argumentCurrent, rhs.data, rhs.size);
    m_argumentCurrent += rhs.size;
    while(reinterpret_cast<int>(m_argumentCurrent) & 0x03)
      *m_argumentCurrent++ = '\0';

    return *this;
  }

    
  pstr OutboundPacketStream::BeginElement(pstr beginPtr)
  {
    if(0==m_elementSizePtr)
    {
      m_elementSizePtr = reinterpret_cast<uint32*>(m_data);
      return beginPtr;
    }

    // store an offset to the old element size ptr in the element size slot
    // we store an offset rather than the actual pointer to be 64 bit clean.
    *reinterpret_cast<uint32*>(beginPtr) =
      (uint32)(reinterpret_cast<char*>(m_elementSizePtr) - m_data);
    m_elementSizePtr = reinterpret_cast<uint32*>(beginPtr);
    return beginPtr + 4;
  }


  void OutboundPacketStream::EndElement(pstr endPtr)
  {
    assert(0 != m_elementSizePtr);

    if(m_elementSizePtr == reinterpret_cast<uint32*>(m_data))
    {
      m_elementSizePtr = 0;
    }
    else
    {
      // while building an element, an offset to the containing element's
      // size slot is stored in the elements size slot (or a ptr to data_
      // if there is no containing element). We retrieve that here
      uint32 *previousElementSizePtr =
        (uint32*)(m_data + *reinterpret_cast<uint32*>(m_elementSizePtr));

      // then we store the element size in the slot, note that the element
      // size does not include the size slot, hence the - 4 below.
      uint32 elementSize =
        (endPtr - reinterpret_cast<char*>(m_elementSizePtr)) - 4;
      FromUInt32( reinterpret_cast<char*>(m_elementSizePtr), elementSize);

      // finally, we reset the element size ptr to the containing element
      m_elementSizePtr = previousElementSizePtr;
    }
  }


  bool OutboundPacketStream::ElementSizeSlotRequired() const
  {
    return (0 != m_elementSizePtr);
  }


  void OutboundPacketStream::CheckForAvailableBundleSpace()
  {
    ulong required = Size() + ((ElementSizeSlotRequired())?4:0) + 16;

    RUNTIME_CHECK_EXC3(required <= Capacity(),OUT_OF_BUFFER_MEMORY,
                       "Out of buffer memory")
  }


  void OutboundPacketStream::CheckForAvailableMessageSpace(pcstr addressPattern)
  {
    // plus 4 for at least four bytes of type tag
     ulong required = Size() + ((ElementSizeSlotRequired())?4:0)
                             + RoundUp4(strlen(addressPattern) + 1) + 4;

    RUNTIME_CHECK_EXC3(required <= Capacity(),OUT_OF_BUFFER_MEMORY,
                       "Out of buffer memory")
  }


  void OutboundPacketStream::CheckForAvailableArgumentSpace(ulong argumentLength)
  {
    // plus three for extra type tag, comma and null terminator
     ulong required = (m_argumentCurrent - m_data) + argumentLength
                      + RoundUp4((m_end - m_typeTagsCurrent) + 3);

    RUNTIME_CHECK_EXC3(required <= Capacity(),OUT_OF_BUFFER_MEMORY,
                       "Out of buffer memory")
  }


//-----------------------------------------------------------------------------
// return the first 4 byte boundary after the end of a str4
// be careful about calling this version if you don't know whether
// the string is terminated correctly.
  static inline pcstr FindStr4End(pcstr p)
  {
    assert(((int)p & 0x03L) == 0);

    p += 3;

    while(*p) p += 4;

    return p + 1;
  }


  // return the first 4 byte boundary after the end of a str4
  // returns 0 if p == end or if the string is unterminated
  static inline pcstr FindStr4End(pcstr p,pcstr end )
  {
    assert(((int)p & 0x03L) == 0);
    assert(((int)end & 0x03L) == 0);

    if(p >= end) return 0;

    p += 3;
    end -= 1;

    while(p != end && *p) p += 4;

    if(*p) return 0;
    else   return p + 1;
  }


  static inline pcstr RoundUp4(pcstr p)
  {
    return (pcstr)((long)(p-1) & (~0x03L)) + 4;
  }


  static inline int32 ToInt32(pcstr p)
  {
  #ifdef OSC_HOST_LITTLE_ENDIAN
    union
    {
        osc::int32 i;
        char       c[4];
    } u;

    u.c[0] = p[3];
    u.c[1] = p[2];
    u.c[2] = p[1];
    u.c[3] = p[0];

    return u.i;
  #else
    return *(int32*)p;
  #endif
  }


  static inline uint32 ToUInt32(pcstr p)
  {
  #ifdef OSC_HOST_LITTLE_ENDIAN
    union
    {
        osc::uint32 i;
        char        c[4];
    } u;

    u.c[0] = p[3];
    u.c[1] = p[2];
    u.c[2] = p[1];
    u.c[3] = p[0];

    return u.i;
  #else
    return *(uint32*)p;
  #endif
  }


  int64 ToInt64(pcstr p)
  {
  #ifdef OSC_HOST_LITTLE_ENDIAN
    union
    {
        osc::int64 i;
        char       c[4];
    } u;

    u.c[0] = p[7];
    u.c[1] = p[6];
    u.c[2] = p[5];
    u.c[3] = p[4];
    u.c[4] = p[3];
    u.c[5] = p[2];
    u.c[6] = p[1];
    u.c[7] = p[0];

    return u.i;
  #else
    return *(int64*)p;
  #endif
  }


  uint64 ToUInt64(pcstr p)
  {
  #ifdef OSC_HOST_LITTLE_ENDIAN
    union
    {
        osc::uint64 i;
        char        c[4];
    } u;

    u.c[0] = p[7];
    u.c[1] = p[6];
    u.c[2] = p[5];
    u.c[3] = p[4];
    u.c[4] = p[3];
    u.c[5] = p[2];
    u.c[6] = p[1];
    u.c[7] = p[0];

    return u.i;
  #else
    return *(uint64*)p;
  #endif
  }

//------------------------------------------------------------------------------

  bool ReceivedPacket::IsBundle() const
  {
    return (Size() > 0 && Contents()[0] == '#');
  }

//------------------------------------------------------------------------------

  bool ReceivedBundleElement::IsBundle() const
  {
    return (Size() > 0 && Contents()[0] == '#');
  }


  uint32 ReceivedBundleElement::Size() const
  {
    return ToUInt32(m_size);
  }

//------------------------------------------------------------------------------

  bool ReceivedMessageArgument::AsBool() const
  {
    RUNTIME_CHECK_EXC3(m_typeTag, MISSING_ARGUMENT, "Missing argument")
    if( *m_typeTag == TRUE_TYPE_TAG )
      return true;
    if( *m_typeTag == FALSE_TYPE_TAG )
      return false;
    THROW_EXC2(WRONG_ARGUMENT,"Wrong argument") return false;
  }


  bool ReceivedMessageArgument::AsBoolUnchecked() const
  {
    RUNTIME_CHECK_EXC3(m_typeTag, MISSING_ARGUMENT, "Missing argument")
    if( *m_typeTag == TRUE_TYPE_TAG )
       return true;
    return false;
  }


  int32 ReceivedMessageArgument::AsInt32() const
  {
    RUNTIME_CHECK_EXC3(m_typeTag, MISSING_ARGUMENT, "Missing argument")
    if( *m_typeTag == INT32_TYPE_TAG )
      return AsInt32Unchecked();
    THROW_EXC2(WRONG_ARGUMENT,"Wrong argument") return 0;
  }


  int32 ReceivedMessageArgument::AsInt32Unchecked() const
  {
  #ifdef OSC_HOST_LITTLE_ENDIAN
    union
    {
        osc::int32 i;
        char       c[4];
    } u;

    u.c[0] = m_argument[3];
    u.c[1] = m_argument[2];
    u.c[2] = m_argument[1];
    u.c[3] = m_argument[0];

    return u.i;
  #else
    return *(int32*)m_argument;
  #endif
  }


  float ReceivedMessageArgument::AsFloat() const
  {
    RUNTIME_CHECK_EXC3(m_typeTag, MISSING_ARGUMENT, "Missing argument")
    if( *m_typeTag == FLOAT_TYPE_TAG )
      return AsFloatUnchecked();
    THROW_EXC2(WRONG_ARGUMENT,"Wrong argument") return 0.0f;
  }


  float ReceivedMessageArgument::AsFloatUnchecked() const
  {
  #ifdef OSC_HOST_LITTLE_ENDIAN
    union
    {
        float f;
        char c[4];
    } u;

    u.c[0] = m_argument[3];
    u.c[1] = m_argument[2];
    u.c[2] = m_argument[1];
    u.c[3] = m_argument[0];

    return u.f;
  #else
    return *(float*)m_argument;
  #endif
  }


  char ReceivedMessageArgument::AsChar() const
  {
    RUNTIME_CHECK_EXC3(m_typeTag, MISSING_ARGUMENT, "Missing argument")
    if( *m_typeTag == CHAR_TYPE_TAG )
      return AsCharUnchecked();
    THROW_EXC2(WRONG_ARGUMENT,"Wrong argument") return '\0';
  }


  char ReceivedMessageArgument::AsCharUnchecked() const
  {
    return (char)ToInt32( m_argument );
  }


  uint32 ReceivedMessageArgument::AsRgbaColor() const
  {
    RUNTIME_CHECK_EXC3(m_typeTag, MISSING_ARGUMENT, "Missing argument")
    if( *m_typeTag == RGBA_COLOR_TYPE_TAG )
      return AsRgbaColorUnchecked();
    THROW_EXC2(WRONG_ARGUMENT,"Wrong argument") return 0;
  }


  uint32 ReceivedMessageArgument::AsRgbaColorUnchecked() const
  {
    return ToUInt32( m_argument );
  }


  uint32 ReceivedMessageArgument::AsMidiMessage() const
  {
    RUNTIME_CHECK_EXC3(m_typeTag, MISSING_ARGUMENT, "Missing argument")
    if( *m_typeTag == MIDI_MESSAGE_TYPE_TAG )
      return AsMidiMessageUnchecked();
    THROW_EXC2(WRONG_ARGUMENT,"Wrong argument") return 0;
  }


  uint32 ReceivedMessageArgument::AsMidiMessageUnchecked() const
  {
    return ToUInt32( m_argument );
  }


  int64 ReceivedMessageArgument::AsInt64() const
  {
    RUNTIME_CHECK_EXC3(m_typeTag, MISSING_ARGUMENT, "Missing argument")
    if( *m_typeTag == INT64_TYPE_TAG )
      return AsInt64Unchecked();
    THROW_EXC2(WRONG_ARGUMENT,"Wrong argument") return 0;
  }


  int64 ReceivedMessageArgument::AsInt64Unchecked() const
  {
    return ToInt64( m_argument );
  }


  uint64 ReceivedMessageArgument::AsTimeTag() const
  {
    RUNTIME_CHECK_EXC3(m_typeTag, MISSING_ARGUMENT, "Missing argument")
    if( *m_typeTag == TIME_TAG_TYPE_TAG )
      return AsTimeTagUnchecked();
    THROW_EXC2(WRONG_ARGUMENT,"Wrong argument") return 0;
  }


  uint64 ReceivedMessageArgument::AsTimeTagUnchecked() const
  {
    return ToUInt64( m_argument );
  }


  double ReceivedMessageArgument::AsDouble() const
  {
    RUNTIME_CHECK_EXC3(m_typeTag, MISSING_ARGUMENT, "Missing argument")
    if( *m_typeTag == DOUBLE_TYPE_TAG )
      return AsDoubleUnchecked();
    THROW_EXC2(WRONG_ARGUMENT,"Wrong argument") return 0.0;
  }


  double ReceivedMessageArgument::AsDoubleUnchecked() const
  {
  #ifdef OSC_HOST_LITTLE_ENDIAN
    union
    {
        double d;
        char   c[8];
    } u;

    u.c[0] = m_argument[7];
    u.c[1] = m_argument[6];
    u.c[2] = m_argument[5];
    u.c[3] = m_argument[4];
    u.c[4] = m_argument[3];
    u.c[5] = m_argument[2];
    u.c[6] = m_argument[1];
    u.c[7] = m_argument[0];

    return u.d;
  #else
    return *(double*)m_argument;
  #endif
  }


  pcstr ReceivedMessageArgument::AsString() const
  {
    RUNTIME_CHECK_EXC3(m_typeTag, MISSING_ARGUMENT, "Missing argument")
    if( *m_typeTag == STRING_TYPE_TAG )
      return m_argument;
    THROW_EXC2(WRONG_ARGUMENT,"Wrong argument") return NULL;
  }


  pcstr ReceivedMessageArgument::AsSymbol() const
  {
    RUNTIME_CHECK_EXC3(m_typeTag, MISSING_ARGUMENT, "Missing argument")
    if( *m_typeTag == SYMBOL_TYPE_TAG )
      return m_argument;
    THROW_EXC2(WRONG_ARGUMENT,"Wrong argument") return NULL;
  }


  void ReceivedMessageArgument::AsBlob( const void*& data, unsigned long& size ) const
  {
    RUNTIME_CHECK_EXC3(m_typeTag, MISSING_ARGUMENT, "Missing argument")
    if( *m_typeTag == BLOB_TYPE_TAG )
      AsBlobUnchecked( data, size );
    else
      THROW_EXC2(WRONG_ARGUMENT,"Wrong argument")
  }


  void ReceivedMessageArgument::AsBlobUnchecked( const void*& data, unsigned long& size ) const
  {
    size = ToUInt32( m_argument );
    data = (void*)(m_argument+4);
  }

//------------------------------------------------------------------------------

  void ReceivedMessageArgumentIterator::Advance()
  {
    if( !m_value.m_typeTag )
        return;
        
    switch( *m_value.m_typeTag++ )
    {
      case '\0':
          // don't advance past end
          --m_value.m_typeTag;
          break;
            
      case TRUE_TYPE_TAG:
      case FALSE_TYPE_TAG:
      case NIL_TYPE_TAG:
      case INFINITUM_TYPE_TAG:

        // zero length
          break;

      case INT32_TYPE_TAG:
      case FLOAT_TYPE_TAG:                                      
      case CHAR_TYPE_TAG:
      case RGBA_COLOR_TYPE_TAG:
      case MIDI_MESSAGE_TYPE_TAG:

          m_value.m_argument += 4;
          break;

      case INT64_TYPE_TAG:
      case TIME_TAG_TYPE_TAG:
      case DOUBLE_TYPE_TAG:
                                
          m_value.m_argument += 8;
          break;

      case STRING_TYPE_TAG: 
      case SYMBOL_TYPE_TAG:
        
          m_value.m_argument = FindStr4End( m_value.m_argument );
          break;

      case BLOB_TYPE_TAG:
        {
          uint32 blobSize = ToUInt32( m_value.m_argument );
          m_value.m_argument = RoundUp4( m_value.m_argument + 4 + blobSize );
        }
          break;

      default:    // unknown type tag
          // don't advance
          --m_value.m_typeTag;
          break;
            

      //    not handled:
      //    [ Indicates the beginning of an array. The tags following are for
      //        data in the Array until a close brace tag is reached.
      //    ] Indicates the end of an array.
    }
  }

//------------------------------------------------------------------------------

  ReceivedMessage::ReceivedMessage( const ReceivedPacket& packet )
    : m_addressPattern( packet.Contents() )
  {
    Init( packet.Contents(), packet.Size() );
  }


  ReceivedMessage::ReceivedMessage( const ReceivedBundleElement& bundleElement )
    : m_addressPattern( bundleElement.Contents() )
  {
    Init( bundleElement.Contents(), bundleElement.Size() );
  }


  void ReceivedMessage::Init(pcstr message, unsigned long size )
  {
    if( size == 0 )
      THROW_EXC2(MALFORMED_MESSAGE,"zero length message")

    if( (size & 0x03L) != 0 )
      THROW_EXC2(MALFORMED_MESSAGE,"message size must be multiple of four")

     pcstr end = message + size;

     m_typeTagsBegin = FindStr4End( m_addressPattern, end );
     if( m_typeTagsBegin == 0 ){
        // address pattern was not terminated before end
        THROW_EXC2(MALFORMED_MESSAGE,"unterminated address pattern")
     }

     if( m_typeTagsBegin == end ){
        // message consists of only the address pattern - no arguments or type tags.
        m_typeTagsBegin = 0;
        m_typeTagsEnd = 0;
        m_arguments = 0;
            
     }
     else
     {
       if( *m_typeTagsBegin != ',' )
         THROW_EXC2(MALFORMED_MESSAGE,"type tags not present")

       if( *(m_typeTagsBegin + 1) == '\0' ){
            // zero length type tags
            m_typeTagsBegin = 0;
            m_typeTagsEnd = 0;
            m_arguments = 0;

      }
      else
      {
          // check that all arguments are present and well formed
                
          m_arguments = FindStr4End( m_typeTagsBegin, end );
          if( m_arguments == 0 ){
            THROW_EXC2(MALFORMED_MESSAGE,"type tags not terminated before message ends")
          }

          ++m_typeTagsBegin; // advance past initial ','
            
         pcstr typeTag = m_typeTagsBegin;
         pcstr argument = m_arguments;
                        
            do{
                switch( *typeTag ){
                    case TRUE_TYPE_TAG:
                    case FALSE_TYPE_TAG:
                    case NIL_TYPE_TAG:
                    case INFINITUM_TYPE_TAG:

                        // zero length
                        break;

                    case INT32_TYPE_TAG:
                    case FLOAT_TYPE_TAG:
                    case CHAR_TYPE_TAG:
                    case RGBA_COLOR_TYPE_TAG:
                    case MIDI_MESSAGE_TYPE_TAG:

                        if( argument == end )
                            THROW_EXC2(MALFORMED_MESSAGE,"arguments exceed message size")
                        argument += 4;
                        if( argument > end )
                           THROW_EXC2(MALFORMED_MESSAGE,"arguments exceed message size")
                        break;

                    case INT64_TYPE_TAG:
                    case TIME_TAG_TYPE_TAG:
                    case DOUBLE_TYPE_TAG:

                        if( argument == end )
                           THROW_EXC2(MALFORMED_MESSAGE,"arguments exceed message size")
                        argument += 8;
                        if( argument > end )
                           THROW_EXC2(MALFORMED_MESSAGE,"arguments exceed message size")
                        break;

                    case STRING_TYPE_TAG: 
                    case SYMBOL_TYPE_TAG:
                    
                        if( argument == end )
                           THROW_EXC2(MALFORMED_MESSAGE,"arguments exceed message size")
                        argument = FindStr4End( argument, end );
                        if( argument == 0 )
                           THROW_EXC2(MALFORMED_MESSAGE,"unterminated string argument")
                        break;

                    case BLOB_TYPE_TAG:
                        {
                            if( argument + 4 > end )
                               THROW_EXC2(MALFORMED_MESSAGE,"arguments exceed message size")
                                
                            uint32 blobSize = ToUInt32( argument );
                            argument = RoundUp4( argument + 4 + blobSize );
                            if( argument > end )
                               THROW_EXC2(MALFORMED_MESSAGE,"arguments exceed message size")
                        }
                        break;
                        
                    default:
                       THROW_EXC2(MALFORMED_MESSAGE,"unknown type tag")

                    //    not handled:
                    //    [ Indicates the beginning of an array. The tags following are for
                    //        data in the Array until a close brace tag is reached.
                    //    ] Indicates the end of an array.
                }

            }while( *++typeTag != '\0' );
            m_typeTagsEnd = typeTag;
        }
    }
  }

//------------------------------------------------------------------------------

  ReceivedBundle::ReceivedBundle( const ReceivedPacket& packet )
    : m_elementCount( 0 )
  {
    Init( packet.Contents(), packet.Size() );
  }


  ReceivedBundle::ReceivedBundle( const ReceivedBundleElement& bundleElement )
    : m_elementCount( 0 )
  {
    Init( bundleElement.Contents(), bundleElement.Size() );
  }


  void ReceivedBundle::Init(pcstr bundle, unsigned long size )
  {
    if( size < 16 )
      THROW_EXC2(MALFORMED_MESSAGE,"packet too short for a bundle")

    if( (size & 0x03L) != 0 )
      THROW_EXC2(MALFORMED_MESSAGE,"bundle size must be multiple of four")

    if( bundle[0] != '#'
        || bundle[1] != 'b'
        || bundle[2] != 'u'
        || bundle[3] != 'n'
        || bundle[4] != 'd'
        || bundle[5] != 'l'
        || bundle[6] != 'e'
        || bundle[7] != '\0' )
      THROW_EXC2(MALFORMED_MESSAGE,"bad bundle address pattern")

    m_end = bundle + size;

    m_timeTag = bundle + 8;

    pcstr p = m_timeTag + 8;
        
    while( p < m_end ){
      if( p + 4 > m_end )
        THROW_EXC2(MALFORMED_MESSAGE,"packet too short for elementSize")

        uint32 elementSize = ToUInt32( p );
        if( (elementSize & 0x03L) != 0 )
          THROW_EXC2(MALFORMED_MESSAGE,"bundle element size must be multiple of four")

        p += 4 + elementSize;
        if( p > m_end )
          THROW_EXC2(MALFORMED_MESSAGE,"packet too short for bundle element")

        ++m_elementCount;
    }

    if( p != m_end )
      THROW_EXC2(MALFORMED_MESSAGE,"bundle contents")
  }


  uint64 ReceivedBundle::TimeTag() const
  {
    return ToUInt64( m_timeTag );
  }

//-----------------------------------------------------------------------------

  std::ostream& operator<<(std::ostream& os, ReceivedMessageArgument const& arg)
  {
    switch( arg.TypeTag() )
    {
        case TRUE_TYPE_TAG:
            os << "bool:true";
            break;
                
        case FALSE_TYPE_TAG:
            os << "bool:false";
            break;

        case NIL_TYPE_TAG:
            os << "(Nil)";
            break;

        case INFINITUM_TYPE_TAG:
            os << "(Infinitum)";
            break;

        case INT32_TYPE_TAG:
            os << "int32:" << arg.AsInt32Unchecked();
            break;

        case FLOAT_TYPE_TAG:
            os << "float32:" << arg.AsFloatUnchecked();
            break;

        case CHAR_TYPE_TAG:
            {
                char s[2] = {0};
                s[0] = arg.AsCharUnchecked();
                os << "char:'" << s << "'";
            }
            break;

        case RGBA_COLOR_TYPE_TAG:
            {
                uint32 color = arg.AsRgbaColorUnchecked();
                
                os << "RGBA:0x"
                        << std::hex << std::setfill('0')
                        << std::setw(2) << (int)((color>>24) & 0xFF)
                        << std::setw(2) << (int)((color>>16) & 0xFF)
                        << std::setw(2) << (int)((color>>8) & 0xFF)
                        << std::setw(2) << (int)(color & 0xFF)
                        << std::setfill(' ');
                os.unsetf(std::ios::basefield);
            }
            break;

        case MIDI_MESSAGE_TYPE_TAG:
            {
                uint32 m = arg.AsMidiMessageUnchecked();
                os << "midi (port, status, data1, data2):<<"
                        << std::hex << std::setfill('0')
                        << "0x" << std::setw(2) << (int)((m>>24) & 0xFF)
                        << " 0x" << std::setw(2) << (int)((m>>16) & 0xFF)
                        << " 0x" << std::setw(2) << (int)((m>>8) & 0xFF)
                        << " 0x" << std::setw(2) << (int)(m & 0xFF)
                        << std::setfill(' ') << ">>";
                os.unsetf(std::ios::basefield);
            }
            break;
                                
        case INT64_TYPE_TAG:
            os << "int64:" << arg.AsInt64Unchecked();
            break;

        case TIME_TAG_TYPE_TAG:
            {
                os << "OSC-timetag:" << arg.AsTimeTagUnchecked();

                std::time_t t =
                        (unsigned long)( arg.AsTimeTagUnchecked() >> 32 );

                // strip trailing newline from string returned by ctime
                const char *timeString = std::ctime( &t );
                int len = strlen( timeString );
                char *s = new char[ len + 1 ];
                strcpy( s, timeString );
                if( len )
                    s[ len - 1 ] = '\0';
                    
                os << " " << s;
            }
            break;
                
        case DOUBLE_TYPE_TAG:
            os << "double:" << arg.AsDoubleUnchecked();
            break;

        case STRING_TYPE_TAG:
            os << "OSC-string:`" << arg.AsStringUnchecked() << "'";
            break;
                
        case SYMBOL_TYPE_TAG: 
            os << "OSC-string (symbol):`" << arg.AsSymbolUnchecked() << "'";
            break;

        case BLOB_TYPE_TAG:
            {
                unsigned long size;
                const void *data;
                arg.AsBlobUnchecked( data, size );
                os << "OSC-blob:<<" << std::hex << std::setfill('0');
                unsigned char *p = (unsigned char*)data;
                for( unsigned long i = 0; i < size; ++i ){
                    os << "0x" << std::setw(2) << int(p[i]);
                    if( i != size-1 )
                        os << ' ';
                }
                os.unsetf(std::ios::basefield);
                os << ">>" << std::setfill(' ');
            }
            break;

        default:
            os << "unknown";
    }

    return os;
  }


  std::ostream& operator<<(std::ostream& os, ReceivedMessage const& m)
  {
    os << "[" << m.AddressPattern();
    bool first = true;

    for( ReceivedMessage::const_iterator i = m.ArgumentsBegin();
            i != m.ArgumentsEnd(); ++i ){
        if( first ){
            os << " ";
            first = false;
        }else{
            os << ", ";
        }

        os << *i;
    }

    os << "]";

    return os;
  }


  std::ostream& operator<<(std::ostream& os, ReceivedBundle const& b)
  {
    static int indent = 0;

    for( int j=0; j < indent; ++j )
        os << "  ";
    os << "{ ( ";
    if( b.TimeTag() == 1 )
        os << "immediate";
    else
        os << b.TimeTag();
    os << " )\n";

    ++indent;
    
    for( ReceivedBundle::const_iterator i = b.ElementsBegin();
            i != b.ElementsEnd(); ++i ){
        if( i->IsBundle() ){
            ReceivedBundle b(*i);
            os << b << "\n";
        }else{
            ReceivedMessage m(*i);
            for( int j=0; j < indent; ++j )
                os << "  ";
            os << m << "\n";
        }
    }

    --indent;

    for( int j=0; j < indent; ++j )
        os << "  ";
    os << "}";

    return os;
  }


  std::ostream& operator<<(std::ostream & os, ReceivedPacket const& p)
  {
    if( p.IsBundle() ){
        ReceivedBundle b(p);
        os << b;
    }else{
        ReceivedMessage m(p);
        os << m;
    }

    return os;
  }

//-----------------------------------------------------------------------------

  OSCPacket::OSCPacket(pcstr contents, uint32 size)
    : super(NULL,size), m_buffer(size,contents)
  {
    m_contents = m_buffer.ptr();
  }

  OSCPacket::OSCPacket(OSCPacket const& p)
    : super(NULL,p.m_buffer.size()), m_buffer(p.m_buffer)
  {
    m_contents = m_buffer.ptr();
  }

  OSCPacket& OSCPacket::operator=(OSCPacket const& p)
  {
    m_buffer   = p.m_buffer;
    m_size     = m_buffer.size();
    m_contents = m_buffer.ptr();
    return *this;
  }


  OSCSocket::OSCSocket(ushort port, pcstr hostSend,ushort portSend)
    : UDPSocket(), OutboundPacketStream(m_buffer,IP_MTU_SIZE)
  {
    UDPSocket::Open(port);
    UDPSocket::SetSendAddress(hostSend,portSend);
    OutboundPacketStream::Clear();
  }


  OSCSocket& OSCSocket::operator<<(MessageSender const&)
  {
    //TRACE("OSCSocket::Send",ReceivedPacket(OutboundPacketStream::Data(),OutboundPacketStream::Size()))
    UDPSocket::Send(OutboundPacketStream::Data(),OutboundPacketStream::Size());
    OutboundPacketStream::Clear();
    return *this;
  }
  
  OSCPacket OSCSocket::Receive(util::UDPSocket::timeout_t tout)
  {
    uint const MAXSIZE=10*1024;
    char buf[MAXSIZE]; uint size;

    util::UDPSocket::Receive((pvoid)buf,MAXSIZE,tout,size);
    return OSCPacket(buf,size);
  }


NAMESPACE_END() //** eof ******************************************************
