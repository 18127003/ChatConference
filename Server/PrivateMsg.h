#pragma once
#include "MsgStruct.h"
ref class PrivateMsg : public MsgStruct
{
public:
	String^ strToUsername;
	String^ strMessage;

	PrivateMsg();
	virtual array<Byte>^ pack() override;
	virtual MsgStruct^ unpack(array<Byte>^ buff) override;
};
