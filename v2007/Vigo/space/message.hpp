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
* $Id: message.hpp 193 2007-01-13 03:55:19Z ian_b $
\*****************************************************************************/

/** \file
 *  \brief Messaging
 *
 *  Structures that are passed as messages among scene nodes.
 */

#ifndef VIGO_SPACE_MESSAGE_HPP_INCLUDED
#define VIGO_SPACE_MESSAGE_HPP_INCLUDED

  #include "../common/types.hpp"

NAMESPACE_VIGO(space) //=======================================================
/** \defgroup grp_message_hpp Messaging
@{*/

  class ANode;

  /// Basic message, with a numerical \c id.
  struct Message
  {
  public:
    Message(uint id_): id(id_)  {}
    virtual ~Message()          {}
    
    virtual Message& Copy() const
    {
      return *new Message(id);
    }
    
    GETTER(Id,uint,id)
    VGETTER(Param, pvoid, 	NULL)
    VGETTER(Text,  std::string, "")

    uint id;
  };
  
  /// Also has a pvoid all-purpose parameter.
  struct MessageParam: Message
  {
  private:
    SUPER(Message)
    
  public:
    MessageParam(uint id_,pvoid param_): super(id_), param(param_) {}

    virtual Message& Copy() const
    {
      return *new MessageParam(id,param);
    }
    
    VGETTER(Param, pvoid, param)
    
    pvoid param;
  };
  
  /// Also has a text parameter.
  struct MessageText: Message
  {
  private:
    SUPER(Message)
    
  public:
    MessageText(uint id_,pcstr text_): super(id_), text(text_?text_:"") 
    {
    }
    
    virtual Message& Copy() const
    {
      return *new MessageText(id,text.c_str());
    }
    
    VGETTER(Text, std::string, text)
    
    std::string text;
  };
  
  /// Also has a pvoid and a text parameters.
  struct MessageParamText: MessageParam
  {
  private:
    SUPER(MessageParam)
    
  public:
    MessageParamText(uint id_,pvoid param_,pcstr text_)
      : super(id_,param_), text(text_?text_:"") 
    {
    }
    
    virtual Message& Copy() const
    {
      return *new MessageParamText(id,param,text.c_str());
    }
    
    VGETTER(Text, std::string, text)
    
    std::string text;
  };
  
//----------------------------------------------------------------------------

  struct MessageDeliveryTime
  {
    MessageDeliveryTime(double dt): deliveryTime(dt) {}
    double deliveryTime;
  };
  
  /// An envelope for message delivery. Used internally.
  struct MessageEnvelope: MessageDeliveryTime
  {
  private:
    SUPER(MessageDeliveryTime)
    
  public:
    MessageEnvelope(Message const& msg_,
                    ANode* addressee_,ANode* sender_)
      : super(0.), msg(msg_.Copy()), addressee(addressee_), sender(sender_)
    {
    }
    
    MessageEnvelope(double deliveryTime_,Message const& msg_,
                    ANode* addressee_,ANode* sender_)
      : super(deliveryTime_), msg(msg_.Copy()), addressee(addressee_), sender(sender_)
    {
    }
    
   ~MessageEnvelope()
    {
      delete &msg;
    }
  
    Message& msg;
    ANode *addressee, *sender;
  };
  
//----------------------------------------------------------------------------

/**@} (defgroup)*/
NAMESPACE_END() //=============================================================

#endif

/*** eof *********************************************************************/
