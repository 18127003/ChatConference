#pragma once
#include "MsgStruct.h"
ref class UsrStatusLstMsg : public MsgStruct
{
public:
	array<String^>^ lstOnlineUsers;

	UsrStatusLstMsg();
	virtual array<Byte>^ pack() override;
	virtual MsgStruct^ unpack(array<Byte>^ buff) override;
};


