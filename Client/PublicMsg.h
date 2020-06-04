#pragma once
#include "MsgStruct.h"

ref class PublicMsg : public MsgStruct
{
public:
	String^ strMessage;

	PublicMsg();
	virtual array<Byte>^ pack() override;  //pack msg for deliver
	virtual MsgStruct^ unpack(array<Byte>^ buff) override;  // get msg content
};
