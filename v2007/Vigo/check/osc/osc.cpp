/*****************************************************************************\
*    Copyright (c) 2004--2007 by Ian G Burleigh                               *
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
* $Id: osc.cpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Unit tests for osc.cpp
 *
 */

#include "../vigotest.hpp"
#include "../../osc/osc.hpp"

#include <iostream>

  NAMESPACE_VIGO(vt_osc)

//----------------------------------------------------------------------------

VT_BEGIN(osc)

CPPUNIT_TEST_SUITE(VIGOTEST);
CPPUNIT_TEST(test1);
CPPUNIT_TEST(test2);
CPPUNIT_TEST(test3);
CPPUNIT_TEST_SUITE_END();

private:
  pstr m_buffer;

public:
  void setUp()
  {
    m_buffer = NULL;
  }

  void tearDown()
  {
  }

  pstr  AllocateAligned4( unsigned long size )
  {
    pstr s = new char[ size + 4 ];   //allocate on stack to get 4 byte alignment
    return (pstr)((long)(s-1) & (~0x03L)) + 4;
  }
  
  pstr NewMessageBuffer(pcstr s, ulong length)
  {
    pstr p = AllocateAligned4(length);
    memcpy(p, s, length);
    return p;
  }

  void test1()
  {
    char const s[] = "/test\0\0\0,fiT\0\0\0\0\0\0\0\0\0\0\0A";
    m_buffer = NewMessageBuffer(s, sizeof(s)-1);

    // test argument iterator interface
    bool unexpectedExceptionCaught = false;
    try
    {
      osc::ReceivedMessage m(osc::ReceivedPacket(m_buffer, sizeof(s)-1));

      CPPUNIT_ASSERT_EQUAL(0, strcmp( m.AddressPattern(), "/test"));
      CPPUNIT_ASSERT_EQUAL(0, strcmp( m.TypeTags(), "fiT"));
        
      osc::ReceivedMessage::const_iterator i = m.ArgumentsBegin();
      ++i; ++i; ++i;
      CPPUNIT_ASSERT(i==m.ArgumentsEnd());

      i = m.ArgumentsBegin();
      float f = (i++)->AsFloat();
      (void)f;
      int n = (i++)->AsInt32();
      (void)n;
      bool b = (i++)->AsBool();
      (void)b;
        
      i = m.ArgumentsBegin();
      bool exceptionThrown = false;
      try
      {
         int n = (i++)->AsInt32();
         (void)n;
      }
      catch(osc::Received::exc&)
      {
        exceptionThrown = true;
      }
      
      CPPUNIT_ASSERT_EQUAL(exceptionThrown, true);

    }
    catch(osc::Received::exc& e)
    {
      std::cout << "unexpected exception: " << e.What() << "\n";
      unexpectedExceptionCaught = true;
    }

    CPPUNIT_ASSERT_EQUAL(unexpectedExceptionCaught, false);

    // test argument stream interface
    unexpectedExceptionCaught = false;
    try
    {
      osc::ReceivedMessage m(osc::ReceivedPacket(m_buffer, sizeof(s)-1));
      osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
      CPPUNIT_ASSERT_EQUAL(args.Eos(), false);

      float f;
      int32 n;
      bool b;
      args >> f >> n >> b;

      (void) f;
      (void) n;
      (void) b;
        
      CPPUNIT_ASSERT_EQUAL(args.Eos(), true);

    }
    catch(osc::Received::exc& e)
    {
      std::cout << "unexpected exception: " << e.What() << "\n";
      unexpectedExceptionCaught = true;
    }
    
    CPPUNIT_ASSERT_EQUAL( unexpectedExceptionCaught, false );
  }

#define TEST2_PRINT( ss )\
    {\
        const char s[] = ss;\
        osc::ReceivedPacket p( NewMessageBuffer( s, sizeof(s)-1 ), sizeof(s)-1 ); \
        osc::ReceivedMessage m( p );\
        /*std::cout << m << "\n";*/ \
    }

  void test2()
  {
    bool unexpectedExceptionCaught = false;
    try{
        //            012301230 1 2 3
        TEST2_PRINT( "/no_args\0\0\0\0" );

        //            012301230 1 2 3 01 2 3
        TEST2_PRINT( "/no_args\0\0\0\0,\0\0\0" );
        
        //            01230123 012 3 0 1 2 3
        TEST2_PRINT( "/an_int\0,i\0\0\0\0\0A" );
        //            012301230 1 2 3 012 3 0 1 2 3 
        TEST2_PRINT( "/a_float\0\0\0\0,f\0\0\0\0\0\0" );
        //            0123012301 2 3 012 3 012301230123 
        TEST2_PRINT( "/a_string\0\0\0,s\0\0hello world\0" );
        //            01230123 012 3 0 1 2 3  0  1  2  3
        TEST2_PRINT( "/a_blob\0,b\0\0\0\0\0\x4\x0\x1\x2\x3" );

        //            0123012301 2 3 012 3 0 1 2 3 0 1 2 3
        TEST2_PRINT( "/an_int64\0\0\0,h\0\0\0\0\0\0\0\0\0\x1" );
        //            01230123012 3 012 3 0 1 2 3 0 1 2 3
        TEST2_PRINT( "/a_timetag\0\0,t\0\0\0\0\0\0\0\0\0\x1" );
        //            0123012301 2 3 012 3 0 1 2 3 0 1 2 3
        TEST2_PRINT( "/a_double\0\0\0,d\0\0\0\0\0\0\0\0\0\0" );
        //            0123012301 2 3 012 3 012301230123 
        TEST2_PRINT( "/a_symbol\0\0\0,S\0\0hello world\0" );
        //            01230123 012 3 0 1 2 3
        TEST2_PRINT( "/a_char\0,c\0\0\0\0\0A" );
        //            012301230 1 2 3 012 3 0 1 2 3 
        TEST2_PRINT( "/a_color\0\0\0\0,r\0\0\0\0\0\0" );
        //            012301230123012 3 012 3 0 1 2 3
        TEST2_PRINT( "/a_midimessage\0\0,m\0\0\0\0\0\0" );
        //            01230123 012 3 
        TEST2_PRINT( "/a_bool\0,T\0\0" );
        //            01230123 012 3 
        TEST2_PRINT( "/a_bool\0,F\0\0" );
        //            01230 1 2 3 012 3
        TEST2_PRINT( "/Nil\0\0\0\0,N\0\0" );
        //            01230 1 2 3 012 3
        TEST2_PRINT( "/Inf\0\0\0\0,I\0\0" );

        TEST2_PRINT( "/test\0\0\0,fiT\0\0\0\0\0\0\0\0\0\0\0A" );
                                                        
        bool exceptionThrown = false;
        try{
            TEST2_PRINT( "/a_char\0,x\0\0\0\0\0A" ); // unknown type tag 'x'
        }catch(osc::Received::exc&){
            exceptionThrown = true;
        }
        CPPUNIT_ASSERT_EQUAL(exceptionThrown, true);
        
    }catch(osc::Received::exc& e){
        std::cout << "unexpected exception: " << e.What() << "\n";
        unexpectedExceptionCaught = true;
    }
    CPPUNIT_ASSERT_EQUAL(unexpectedExceptionCaught, false);
  }

#define TEST_PACK_UNPACK0( addressPattern, argument, value, recieveGetter ) \
    {                                    \
        memset( buffer, 0x74, bufferSize );   \
        osc::OutboundPacketStream ps( buffer, bufferSize ); \
        ps << osc::BeginMessage( addressPattern )  \
            << argument \
            << osc::EndMessage;\
        CPPUNIT_ASSERT_EQUAL( ps.IsReady(), true );\
        osc::ReceivedMessage m( osc::ReceivedPacket(ps.Data(), ps.Size()) );\
        /*std::cout << m << "\n";*/ \
        CPPUNIT_ASSERT_EQUAL( m.ArgumentsBegin()-> recieveGetter () , value );\
    }  \
    {                                    \
        memset( buffer, 0x74, bufferSize );   \
        osc::OutboundPacketStream ps( buffer, bufferSize ); \
        ps << osc::BeginBundle(osc::TimeTag((uint64)1234)) \
            << osc::BeginMessage( addressPattern )  \
            << argument \
            << osc::EndMessage \
            << osc::EndBundle;\
        CPPUNIT_ASSERT_EQUAL( ps.IsReady(), true );\
        osc::ReceivedBundle b( osc::ReceivedPacket(ps.Data(), ps.Size()) );\
        osc::ReceivedMessage m( *b.ElementsBegin() );\
        /*std::cout << m << "\n";*/ \
        CPPUNIT_ASSERT_EQUAL( m.ArgumentsBegin()-> recieveGetter () , value );\
    }
    
#define TEST_PACK_UNPACK( addressPattern, argument, type, recieveGetter ) \
    {                                    \
        memset( buffer, 0x74, bufferSize );   \
        osc::OutboundPacketStream ps( buffer, bufferSize ); \
        ps << osc::BeginMessage( addressPattern )  \
            << argument \
            << osc::EndMessage;\
        CPPUNIT_ASSERT_EQUAL( ps.IsReady(), true );\
        osc::ReceivedMessage m( osc::ReceivedPacket(ps.Data(), ps.Size()) );\
        /*std::cout << m << "\n";*/ \
        CPPUNIT_ASSERT_EQUAL( m.ArgumentsBegin()-> recieveGetter () , ( type ) argument );\
    }  \
    {                                    \
        memset( buffer, 0x74, bufferSize );   \
        osc::OutboundPacketStream ps( buffer, bufferSize ); \
        ps << osc::BeginBundle(osc::TimeTag((uint64)1234)) \
            << osc::BeginMessage( addressPattern )  \
            << argument \
            << osc::EndMessage \
            << osc::EndBundle;\
        CPPUNIT_ASSERT_EQUAL( ps.IsReady(), true );\
        osc::ReceivedBundle b( osc::ReceivedPacket(ps.Data(), ps.Size()) );\
        osc::ReceivedMessage m( *b.ElementsBegin() );\
        /*std::cout << m << "\n";*/ \
        CPPUNIT_ASSERT_EQUAL( m.ArgumentsBegin()-> recieveGetter () , ( type ) argument );\
    }

#define TEST_PACK_UNPACK_S( addressPattern, argument, type, recieveGetter ) \
    {                                    \
        memset( buffer, 0x74, bufferSize );   \
        osc::OutboundPacketStream ps( buffer, bufferSize ); \
        ps << osc::BeginMessage( addressPattern )  \
            << argument \
            << osc::EndMessage;\
        CPPUNIT_ASSERT_EQUAL( ps.IsReady(), true );\
        osc::ReceivedMessage m( osc::ReceivedPacket(ps.Data(), ps.Size()) );\
        /*std::cout << m << "\n";*/ \
        CPPUNIT_ASSERT(0==strcmp( m.ArgumentsBegin()-> recieveGetter () , argument) );\
    }  \
    {                                    \
        memset( buffer, 0x74, bufferSize );   \
        osc::OutboundPacketStream ps( buffer, bufferSize ); \
        ps << osc::BeginBundle(osc::TimeTag((uint64)1234)) \
            << osc::BeginMessage( addressPattern )  \
            << argument \
            << osc::EndMessage \
            << osc::EndBundle;\
        CPPUNIT_ASSERT_EQUAL( ps.IsReady(), true );\
        osc::ReceivedBundle b( osc::ReceivedPacket(ps.Data(), ps.Size()) );\
        osc::ReceivedMessage m( *b.ElementsBegin() );\
        /*std::cout << m << "\n";*/ \
        CPPUNIT_ASSERT( 0==strcmp(m.ArgumentsBegin()-> recieveGetter (), argument) );\
    }

  void test3()
  {
    int bufferSize = 1000;
    char *buffer = AllocateAligned4( bufferSize );

  // single message tests
    // empty message
    {
        memset( buffer, 0x74, bufferSize );
        osc::OutboundPacketStream ps( buffer, bufferSize );
        ps << osc::BeginMessage( "/no_arguments" )
            << osc::EndMessage;
        CPPUNIT_ASSERT_EQUAL( ps.IsReady(), true );
        osc::ReceivedMessage m( osc::ReceivedPacket(ps.Data(), ps.Size()) );
        /*std::cout << m << "\n";*/ \
    }

    TEST_PACK_UNPACK( "/a_bool", true, bool, AsBool );
    TEST_PACK_UNPACK( "/a_bool", false, bool, AsBool );
    TEST_PACK_UNPACK( "/a_bool", (bool)1, bool, AsBool );

    TEST_PACK_UNPACK0( "/nil", osc::Nil, true, IsNil );
    TEST_PACK_UNPACK0( "/inf", osc::Infinitum, true, IsInfinitum );

    TEST_PACK_UNPACK( "/an_int", (int32)1234, int32, AsInt32 );

    TEST_PACK_UNPACK( "/a_float", 3.1415926f, float, AsFloat );

    TEST_PACK_UNPACK( "/a_char", 'c', char, AsChar );

    TEST_PACK_UNPACK( "/an_rgba_color", osc::RgbaColor(0x22334455), uint32, AsRgbaColor );

    TEST_PACK_UNPACK( "/a_midi_message", osc::MidiMessage(0x7F), uint32, AsMidiMessage );

    TEST_PACK_UNPACK( "/an_int64", (int64)(0xFFFFFFFF), int64, AsInt64 );

    //>>>IN TEST_PACK_UNPACK( "/a_time_tag", osc::TimeTag((uint64)0xFFFFFFFF), uint64, AsTimeTag );

    TEST_PACK_UNPACK( "/a_double", (double)3.1415926, double, AsDouble );

    // blob
    {
        char blobData[] = "abcd";
        memset( buffer, 0x74, bufferSize );
        osc::OutboundPacketStream ps( buffer, bufferSize );
        ps << osc::BeginMessage( "/a_blob" )
            << osc::Blob( blobData, 4 )
            << osc::EndMessage;
        CPPUNIT_ASSERT_EQUAL( ps.IsReady(), true );
        osc::ReceivedMessage m( osc::ReceivedPacket(ps.Data(), ps.Size()) );
        /*std::cout << m << "\n";*/

        const void *value;
        unsigned long size;
        m.ArgumentsBegin()->AsBlob( value, size );
        CPPUNIT_ASSERT_EQUAL( size, (unsigned long)4 );
        CPPUNIT_ASSERT_EQUAL( (memcmp( value, blobData, 4 ) == 0), true );
    }

    TEST_PACK_UNPACK_S( "/a_string", "hello world", const char*, AsString );
    TEST_PACK_UNPACK_S( "/a_symbol", osc::Symbol("foobar"), const char*, AsSymbol );


    // nested bundles, and multiple messages in bundles...

    {
        memset( buffer, 0x74, bufferSize );
        osc::OutboundPacketStream ps( buffer, bufferSize );
        ps << osc::BeginBundle()
            << osc::BeginMessage( "/message_one" ) << 1 << 2 << 3 << 4 << osc::EndMessage
            << osc::BeginMessage( "/message_two" ) << 1 << 2 << 3 << 4 << osc::EndMessage
            << osc::BeginMessage( "/message_three" ) << 1 << 2 << 3 << 4 << osc::EndMessage
            << osc::BeginMessage( "/message_four" ) << 1 << 2 << 3 << 4 << osc::EndMessage
            << osc::EndBundle;
        CPPUNIT_ASSERT_EQUAL( ps.IsReady(), true );
        osc::ReceivedBundle b( osc::ReceivedPacket(ps.Data(), ps.Size()) );
        /*std::cout << b << "\n";*/
    }
    
}
VT_END()

//----------------------------------------------------------------------------

#if RUN_ALL_TESTS || RUN_OSC_TESTS
  VT_REGISTER(osc)
#endif

NAMESPACE_END() //** eof ******************************************************
  
