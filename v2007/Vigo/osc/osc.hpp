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
* $Id: osc.hpp 54 2006-07-14 21:29:14Z ian_b $
\*****************************************************************************/

/** \file
 *
 *  OSC protocol
 */

#ifndef VIGO_OSC_OSC_HPP_INCLUDED
#define VIGO_OSC_OSC_HPP_INCLUDED

  #include "../common/types.hpp"
  #include "../common/socket.hpp"

NAMESPACE_VIGO(osc) //=========================================================
/** \defgroup grp_osc_hpp OSC
@{*/

//-----------------------------------------------------------------------------
// osc specific types

  struct NilType
  {
  };

  extern NilType Nil;


  struct InfinitumType
  {
  };

  extern InfinitumType Infinitum;


  struct RgbaColor
  {
    RgbaColor()
    {
    }
    
    explicit RgbaColor(uint32 value_): value(value_)
    {
    }

    uint32 value;

    operator uint32() const { return value; }
  };


  struct MidiMessage
  {
    MidiMessage()
    {
    }
    
    explicit MidiMessage(uint32 value_): value(value_)
    {
    }

    uint32 value;

    operator uint32() const { return value; }
  };


  struct TimeTag
  {
    TimeTag(TimeTag const& t = Immediate()): value(t.value) {}
    explicit TimeTag(uint64 v) : value(v) {}
    explicit TimeTag(double);
    
    TimeTag& operator+=(TimeTag const&);
    TimeTag  operator+ (TimeTag const&) const;
    TimeTag& operator-=(TimeTag const&);
    TimeTag  operator- (TimeTag const&) const;

    double   AsDouble() const;

    static TimeTag Immediate()
    {
      return TimeTag((uint64)1);
    }

    static TimeTag Now();

    static TimeTag FarFuture()
    {
      return TimeTag((uint64)-1);
    }

    uint64 value;
  };
  
  std::ostream& operator<<(std::ostream&,osc::TimeTag const&);


  struct Symbol
  {
    Symbol()
    {
    }

    explicit Symbol(pcstr value_): value(value_)
    {
    }

    pcstr value;

    operator pcstr() const { return value; }
  };


  struct Blob
  {
    Blob()
    {
    }

    explicit Blob(pcvoid data_, ulong size_): data(data_), size(size_)
    {
    }

    pcvoid data;
    ulong  size;
};

  struct BundleInitiator
  {
    explicit BundleInitiator(uint64 timeTag_): timeTag(timeTag_)
    {
    }
    
    uint64 timeTag;
  };

  extern BundleInitiator BeginBundleImmediate;

  inline BundleInitiator BeginBundle(TimeTag const& tt=TimeTag::Immediate())
  {
    return BundleInitiator(tt.value);
  }


  struct BundleTerminator
  {
  };

  extern BundleTerminator EndBundle;


  struct BeginMessage
  {
    explicit BeginMessage(pcstr addressPattern_): addressPattern(addressPattern_)
    {
    }
    
    pcstr addressPattern;
  };


  struct MessageTerminator
  {
  };

  extern MessageTerminator EndMessage;


  struct MessageSender
  {
  };

  extern MessageSender SendMessage;


//-----------------------------------------------------------------------------

  class OutboundPacketStream
  {
  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS OUT_OF_BUFFER_MEMORY,\
                    MESSAGE_IN_PROGRESS, MESSAGE_NOT_IN_PROGRESS,\
		    BUNDLE_NOT_IN_PROGRESS
    DECLARE_EXC_ROOT
    #undef EXC_IDS
#endif

    OutboundPacketStream(pstr buffer, ulong capacity);
   ~OutboundPacketStream();

    void  Clear();
    ulong Capacity() const;
    // invariant: size() is valid even while building a message.
    ulong Size()     const;
    pcstr Data()     const;

    // indicates that all messages have been closed with a matching EndMessage
    // and all bundles have been closed with a matching EndBundle
    bool IsReady() const;

    bool IsMessageInProgress() const;
    bool IsBundleInProgress()  const;

    OutboundPacketStream& operator<<(BundleInitiator const&);
    OutboundPacketStream& operator<<(BundleTerminator const&);
    
    OutboundPacketStream& operator<<(BeginMessage const&);
    OutboundPacketStream& operator<<(MessageTerminator const&);

    OutboundPacketStream& operator<<(bool);
    OutboundPacketStream& operator<<(NilType const&);
    OutboundPacketStream& operator<<(InfinitumType const&);
    OutboundPacketStream& operator<<(int32);
    OutboundPacketStream& operator<<(float);
    OutboundPacketStream& operator<<(char);
    OutboundPacketStream& operator<<(RgbaColor const&);
    OutboundPacketStream& operator<<(MidiMessage const&);
    OutboundPacketStream& operator<<(int64);
    OutboundPacketStream& operator<<(TimeTag const&);
    OutboundPacketStream& operator<<(double);
    OutboundPacketStream& operator<<(pcstr);
    OutboundPacketStream& operator<<(Symbol const&);
    OutboundPacketStream& operator<<(Blob const&);

  private:
    pstr BeginElement(pstr begin);
    void EndElement  (pstr end);

    bool ElementSizeSlotRequired() const;
    void CheckForAvailableBundleSpace();
    void CheckForAvailableMessageSpace(pcstr addressPattern);
    void CheckForAvailableArgumentSpace(ulong argumentLength);

    pstr m_data;
    pstr m_end;

    pstr m_typeTagsCurrent; // stored in reverse order
    pstr m_messageCursor;
    pstr m_argumentCurrent;

    // elementSizePtr_ has two special values: 0 indicates that a bundle
    // isn't open, and elementSizePtr_==data_ indicates that a bundle is
    // open but that it doesn't have a size slot (ie the outermost bundle)
    uint32 *m_elementSizePtr;

    bool m_messageIsInProgress;
  };

//-----------------------------------------------------------------------------

  enum TypeTagValues 
  {
    TRUE_TYPE_TAG         = 'T',
    FALSE_TYPE_TAG        = 'F',
    NIL_TYPE_TAG          = 'N',
    INFINITUM_TYPE_TAG    = 'I',
    INT32_TYPE_TAG        = 'i',
    FLOAT_TYPE_TAG        = 'f',
    CHAR_TYPE_TAG         = 'c',
    RGBA_COLOR_TYPE_TAG   = 'r',
    MIDI_MESSAGE_TYPE_TAG = 'm',
    INT64_TYPE_TAG        = 'h',
    TIME_TAG_TYPE_TAG     = 't',
    DOUBLE_TYPE_TAG       = 'd',
    STRING_TYPE_TAG       = 's',
    SYMBOL_TYPE_TAG       = 'S',
    BLOB_TYPE_TAG         = 'b'
  };


  class Received
  {
  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    #define EXC_IDS MISSING_ARGUMENT, WRONG_ARGUMENT, EXCESS_ARGUMENT, MALFORMED_MESSAGE
    DECLARE_EXC_ROOT
    #undef EXC_IDS
#endif
  };

  class ReceivedPacket
  {
  public:
    ReceivedPacket(pcstr contents, uint32 size)
      : m_contents(contents), m_size(size)
    {
    }

    virtual ~ReceivedPacket() {}

    bool IsMessage()  const { return !IsBundle(); }
    bool IsBundle()   const;

    uint32 Size()     const { return m_size; }
    pcstr  Contents() const { return m_contents; }

  protected:
    pcstr  m_contents;
    uint32 m_size;
  };


  class ReceivedBundleElement
  {
  public:
    ReceivedBundleElement(pcstr size)
      : m_size(size)
    {
    }

    friend class ReceivedBundleElementIterator;

    bool IsMessage()  const { return !IsBundle(); }
    bool IsBundle()   const;

    uint32 Size()     const;
    pcstr  Contents() const { return m_size + 4; }

  private:
    pcstr m_size;
  };


  class ReceivedBundleElementIterator
  {
  public:
    ReceivedBundleElementIterator(pcstr sizePtr)
      : m_value(sizePtr)
    {
    }

    ReceivedBundleElementIterator operator++()
    {
      Advance();
      return *this;
    }

    ReceivedBundleElementIterator operator++(int)
    {
      ReceivedBundleElementIterator old(*this);
      Advance();
      return old;
    }

    const ReceivedBundleElement& operator*()  const { return  m_value; }
    const ReceivedBundleElement* operator->() const { return &m_value; }

    friend bool operator==(const ReceivedBundleElementIterator& lhs,
                           const ReceivedBundleElementIterator& rhs);
  private:
    ReceivedBundleElement m_value;

    void Advance() { m_value.m_size = m_value.Contents() + m_value.Size(); }

    bool IsEqualTo(const ReceivedBundleElementIterator& rhs) const
    {
      return m_value.m_size == rhs.m_value.m_size;
    }
  };

  inline bool operator==(ReceivedBundleElementIterator const& lhs,
                         ReceivedBundleElementIterator const& rhs)
  {
    return lhs.IsEqualTo(rhs);
  }

  inline bool operator!=(ReceivedBundleElementIterator const& lhs,
                         ReceivedBundleElementIterator const& rhs)
  {
    return !(lhs == rhs);
  }


  class ReceivedMessageArgument: public Received
  {
  public:
    ReceivedMessageArgument(pcstr typeTag, pcstr argument)
      : m_typeTag(typeTag), m_argument(argument)
    {
    }

    friend class ReceivedMessageArgumentIterator;
 
    char TypeTag() const { return *m_typeTag; }

    // the unchecked methods below don't check whether the argument actually
    // is of the specified type. they should only be used if you've already
    // checked the type tag or the associated IsType() method.

    bool IsBool() const  { return *m_typeTag == TRUE_TYPE_TAG || *m_typeTag == FALSE_TYPE_TAG; }
    bool AsBool() const;
    bool AsBoolUnchecked() const;

    bool IsNil() const   { return *m_typeTag == NIL_TYPE_TAG; }
    bool IsInfinitum() const { return *m_typeTag == INFINITUM_TYPE_TAG; }

    bool IsInt32() const { return *m_typeTag == INT32_TYPE_TAG; }
    int32 AsInt32() const;
    int32 AsInt32Unchecked() const;

    bool  IsFloat() const { return *m_typeTag == FLOAT_TYPE_TAG; }
    float AsFloat() const;
    float AsFloatUnchecked() const;

    bool IsChar() const  { return *m_typeTag == CHAR_TYPE_TAG; }
    char AsChar() const;
    char AsCharUnchecked() const;

    bool IsRgbaColor() const { return *m_typeTag == RGBA_COLOR_TYPE_TAG; }
    uint32 AsRgbaColor() const;
    uint32 AsRgbaColorUnchecked() const;

    bool IsMidiMessage() const { return *m_typeTag == MIDI_MESSAGE_TYPE_TAG; }
    uint32 AsMidiMessage() const;
    uint32 AsMidiMessageUnchecked() const;

    bool IsInt64() const { return *m_typeTag == INT64_TYPE_TAG; }
    int64 AsInt64() const;
    int64 AsInt64Unchecked() const;

    bool IsTimeTag() const { return *m_typeTag == TIME_TAG_TYPE_TAG; }
    uint64 AsTimeTag() const;
    uint64 AsTimeTagUnchecked() const;

    bool IsDouble() const { return *m_typeTag == DOUBLE_TYPE_TAG; }
    double AsDouble() const;
    double AsDoubleUnchecked() const;

    bool IsString() const { return *m_typeTag == STRING_TYPE_TAG; }
    pcstr AsString() const;
    pcstr AsStringUnchecked() const { return m_argument; }

    bool IsSymbol() const { return *m_typeTag == SYMBOL_TYPE_TAG; }
    pcstr AsSymbol() const;
    pcstr AsSymbolUnchecked() const { return m_argument; }

    bool IsBlob() const { return *m_typeTag == BLOB_TYPE_TAG; }
    void AsBlob(const void*& data, ulong& size) const;
    void AsBlobUnchecked(const void*& data, ulong& size) const;
    
  private:
    pcstr m_typeTag;
    pcstr m_argument;
  };


  class ReceivedMessageArgumentIterator
  {
  public:
    ReceivedMessageArgumentIterator(pcstr typeTags, pcstr arguments)
      : m_value(typeTags, arguments)
    {
    }

    ReceivedMessageArgumentIterator operator++()
    {
      Advance();
      return *this;
    }

    ReceivedMessageArgumentIterator operator++(int)
    {
      ReceivedMessageArgumentIterator old(*this);
      Advance();
      return old;
    }

    ReceivedMessageArgument const& operator*() const  { return  m_value; }
    ReceivedMessageArgument const* operator->() const { return &m_value; }

    friend bool operator==(ReceivedMessageArgumentIterator const& lhs,
                           ReceivedMessageArgumentIterator const& rhs);

  private:
    ReceivedMessageArgument m_value;

    void Advance();

    bool IsEqualTo(ReceivedMessageArgumentIterator const& rhs) const
    {
      return m_value.m_typeTag == rhs.m_value.m_typeTag;
    }
  };

  inline bool operator==(ReceivedMessageArgumentIterator const& lhs,
                         ReceivedMessageArgumentIterator const& rhs)
  {
    return lhs.IsEqualTo(rhs);
  }

  inline bool operator!=(ReceivedMessageArgumentIterator const& lhs,
                         ReceivedMessageArgumentIterator const& rhs)
  {
    return !(lhs == rhs);
  }


  class ReceivedMessageArgumentStream: public Received
  {
    friend class ReceivedMessage;

    ReceivedMessageArgumentStream(ReceivedMessageArgumentIterator const& begin,
                                  ReceivedMessageArgumentIterator const& end)
      : m_p(begin), m_end(end)
    {
    }

    ReceivedMessageArgumentIterator m_p, m_end;

    void NeedArgument()
    {
      RUNTIME_CHECK_EXC3(!Eos(), MISSING_ARGUMENT, "Missing argument")
    }
    
    void ExcessArgument()
    {
      RUNTIME_CHECK_EXC3(Eos(),  EXCESS_ARGUMENT,  "Excess argument")
    }
    
  public:
    // end of stream
    bool Eos() const { return m_p == m_end; }

    ReceivedMessageArgumentStream& operator>>(bool& rhs)
    {
      NeedArgument();

      rhs = (*m_p++).AsBool();
      return *this;
    }

    // not sure if it would be useful to stream Nil and Infinitum
    // for now it's not possible
    ReceivedMessageArgumentStream& operator>>(int32& rhs)
    {
      NeedArgument();

      rhs = (*m_p++).AsInt32();
      return *this;
    }     

    ReceivedMessageArgumentStream& operator>>(float& rhs)
    {
      NeedArgument();

      rhs = (*m_p++).AsFloat();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>(char& rhs)
    {
      NeedArgument();

      rhs = (*m_p++).AsChar();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>(RgbaColor& rhs)
    {
      NeedArgument();

      rhs.value = (*m_p++).AsRgbaColor();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>(MidiMessage& rhs)
    {
      NeedArgument();

      rhs.value = (*m_p++).AsMidiMessage();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>(int64& rhs)
    {
      NeedArgument();

      rhs = (*m_p++).AsInt64();
      return *this;
    }
    
    ReceivedMessageArgumentStream& operator>>(TimeTag& rhs)
    {
      NeedArgument();

      rhs.value = (*m_p++).AsTimeTag();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>(double& rhs)
    {
      NeedArgument();

      rhs = (*m_p++).AsDouble();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>(Blob& rhs)
    {
      NeedArgument();

      (*m_p++).AsBlob(rhs.data, rhs.size);
      return *this;
    }
    
    ReceivedMessageArgumentStream& operator>>(pcstr & rhs)
    {
      NeedArgument();

      rhs = (*m_p++).AsString();
      return *this;
    }
    
    ReceivedMessageArgumentStream& operator>>(Symbol& rhs)
    {
      NeedArgument();

      rhs.value = (*m_p++).AsSymbol();
      return *this;
    }

    ReceivedMessageArgumentStream& operator>>(MessageTerminator&)
    {
      ExcessArgument();

      return *this;
    }
  };


  class ReceivedMessage: public Received
  {
    void Init(pcstr bundle, ulong size);

  public:
    explicit ReceivedMessage(ReceivedPacket const& packet);
    explicit ReceivedMessage(ReceivedBundleElement const& bundleElement);

    pcstr AddressPattern() const { return m_addressPattern; }
    ulong ArgumentCount()  const { return m_typeTagsEnd - m_typeTagsBegin; }
    pcstr TypeTags()       const { return m_typeTagsBegin; }

    typedef ReceivedMessageArgumentIterator const_iterator;
    
    ReceivedMessageArgumentIterator ArgumentsBegin() const
    {
      return ReceivedMessageArgumentIterator(m_typeTagsBegin, m_arguments);
    }
     
    ReceivedMessageArgumentIterator ArgumentsEnd() const
    {
      return ReceivedMessageArgumentIterator(m_typeTagsEnd, 0);
    }

    ReceivedMessageArgumentStream ArgumentStream() const
    {
      return ReceivedMessageArgumentStream(ArgumentsBegin(), ArgumentsEnd());
    }

  private:
    pcstr m_addressPattern;
    pcstr m_typeTagsBegin;
    pcstr m_typeTagsEnd;
    pcstr m_arguments;
  };


  class ReceivedBundle: public Received
  {
    void Init(pcstr message, ulong size);

  public:
    explicit ReceivedBundle(ReceivedPacket const& packet);
    explicit ReceivedBundle(ReceivedBundleElement const& bundleElement);

    uint64 TimeTag() const;

    ulong ElementCount() const { return m_elementCount; }

    typedef ReceivedBundleElementIterator const_iterator;
    
    ReceivedBundleElementIterator ElementsBegin() const
    {
      return ReceivedBundleElementIterator(m_timeTag + 8);
    }
     
    ReceivedBundleElementIterator ElementsEnd() const
    {
      return ReceivedBundleElementIterator(m_end);
    }

  private:
    pcstr m_timeTag;
    pcstr m_end;
    ulong m_elementCount;
  };


//-----------------------------------------------------------------------------

  std::ostream& operator<<(std::ostream&, ReceivedPacket const&);
  std::ostream& operator<<(std::ostream&, ReceivedMessageArgument const&);
  std::ostream& operator<<(std::ostream&, ReceivedMessage const&);
  std::ostream& operator<<(std::ostream&, ReceivedBundle const&);

//-----------------------------------------------------------------------------

  // with its own buffer
  class OSCPacket: public ReceivedPacket
  {
    SUPER(ReceivedPacket)
    
  public:
    OSCPacket(pcstr contents, uint32 size);
    OSCPacket(OSCPacket const&);

    OSCPacket& operator=(OSCPacket const&);

  private:
    util::array1d<char> m_buffer;
  };


  class OSCSocket: public util::UDPSocket, public OutboundPacketStream
  {
  public:
    OSCSocket(ushort port, pcstr hostSend,ushort portSend);

  public: // sending
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  #define OUTBOUNDPACKETSTREAM_OPERATOR(type) \
    OSCSocket& operator<<(type value) { return (OSCSocket&)(OutboundPacketStream::operator<<(value)); }
    OUTBOUNDPACKETSTREAM_OPERATOR(BundleInitiator const&)
    OUTBOUNDPACKETSTREAM_OPERATOR(BundleTerminator const&)
    OUTBOUNDPACKETSTREAM_OPERATOR(BeginMessage const&)
    OUTBOUNDPACKETSTREAM_OPERATOR(MessageTerminator const&)
    OUTBOUNDPACKETSTREAM_OPERATOR(bool)
    OUTBOUNDPACKETSTREAM_OPERATOR(NilType const&)
    OUTBOUNDPACKETSTREAM_OPERATOR(InfinitumType const&)
    OUTBOUNDPACKETSTREAM_OPERATOR(int32)
    OUTBOUNDPACKETSTREAM_OPERATOR(float)
    OUTBOUNDPACKETSTREAM_OPERATOR(char)
    OUTBOUNDPACKETSTREAM_OPERATOR(RgbaColor const&)
    OUTBOUNDPACKETSTREAM_OPERATOR(MidiMessage const&)
    OUTBOUNDPACKETSTREAM_OPERATOR(int64)
    OUTBOUNDPACKETSTREAM_OPERATOR(TimeTag const&)
    OUTBOUNDPACKETSTREAM_OPERATOR(double)
    OUTBOUNDPACKETSTREAM_OPERATOR(pcstr)
    OUTBOUNDPACKETSTREAM_OPERATOR(Symbol const&)
    OUTBOUNDPACKETSTREAM_OPERATOR(Blob const&)
  #undef OUTBOUNDPACKETSTREAM_OPERATOR
#endif

    OSCSocket& operator<<(MessageSender const&);

  public: // receiving
    OSCPacket Receive(util::UDPSocket::timeout_t=util::UDPSocket::FOREVER);

  private:
    static uint const IP_MTU_SIZE = 1536;
    char m_buffer[IP_MTU_SIZE];
  };

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

//** eof **********************************************************************
