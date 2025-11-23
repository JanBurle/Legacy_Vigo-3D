// Copyright (c) 2002-16 Jan Burle <jan@c53.be>

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

// eof
