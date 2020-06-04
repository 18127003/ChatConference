#pragma once
#include "MsgStruct.h"

ref class LoginMsg : public MsgStruct
{
public:
	String^ strUsername;
	String^ strPassword;

	LoginMsg();
	virtual array<Byte>^ pack() override;
	virtual MsgStruct^ unpack(array<Byte>^ buff) override;
};

ref class LoginNotifiMsg : public MsgStruct
{
public:
	String^ strUsername;

	LoginNotifiMsg();
	virtual array<Byte>^ pack() override;
	virtual MsgStruct^ unpack(array<Byte>^ buff) override;
};

ref class ResLoginMsg : public MsgStruct
{
public:
	bool IsSuccess;
	String^ errorMsg;
	ResLoginMsg(); 
	virtual array<Byte>^ pack() override;
	virtual MsgStruct^ unpack(array<Byte>^ buff) override;
};

ref class LogoutNotiMsg : public MsgStruct
{
public:
	String^ strUsername;

	LogoutNotiMsg();
	virtual array<Byte>^ pack() override;
	virtual MsgStruct^ unpack(array<Byte>^ buff) override;
};



