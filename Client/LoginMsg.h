#pragma once
#include "MsgStruct.h"
/*
ref class LoginMsg : public MsgStruct
{
public:
	//String^ strUsername;
	//String^ strPassword;
	bool IsSuccess;
	LoginMsg();
	//virtual array<Byte>^ pack() override;
	virtual void pack() override;
	virtual MsgStruct^ unpack(array<Byte>^ buff) override;
	void pullMsg(String^ msg);

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
*/

ref class LSMsg :public MsgStruct {
public:
	int IsSuccess;
	LSMsg(MessageType msgtype);
	//virtual array<Byte>^ pack() override;
	virtual void pack() override;
	//virtual MsgStruct^ unpack(array<Byte>^ buff) override;
};



ref class RecLSMsg: public RecMsgStruct {
public:
	String^ usrname;
	String^ password;
	int IsSuccess;
	String^ errorMsg;
	RecLSMsg(MessageType msgtype);
	RecMsgStruct^ unpack(array<Byte>^ buff) override;
};

