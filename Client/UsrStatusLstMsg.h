#pragma once
#include "MsgStruct.h"
ref class UsrStatusLstMsg : public MsgStruct
{
public:
	//array<String^>^ lstOnlineUsers;

	UsrStatusLstMsg();
	virtual void pack() override;
	//virtual array<Byte>^ pack() override;
	//virtual MsgStruct^ unpack(array<Byte>^ buff) override;
};

ref class RecUsrStatusLstMsg : public RecMsgStruct {
public:
	array<String^>^ lstOnlineUsers;
	virtual RecMsgStruct^ unpack(array<Byte>^ buff) override;
};


