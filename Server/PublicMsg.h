#pragma once
#include "MsgStruct.h"

ref class PublicMsg : public MsgStruct
{
public:
	//String^ strMessage;

	PublicMsg(MessageType msgtype);
	//virtual array<Byte>^ pack() override;  //pack msg for deliver
	virtual void pack() override;
	//virtual RecMsgStruct^ unpack(array<Byte>^ buff) override;  // get msg content
	//void pullMsg(String^ msg);
};

ref class RecPublicMsg : public RecMsgStruct
{
public:
	String^ strMessage;
	String^ ToUsername;
	RecPublicMsg(MessageType msgtype);
	//virtual array<Byte>^ pack() override;  //pack msg for deliver
	//virtual void pack() override;
	virtual RecMsgStruct^ unpack(array<Byte>^ buff) override;  // get msg content
	//void pullMsg(String^ msg);
};
