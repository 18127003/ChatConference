#pragma once
#include "MsgStruct.h"

ref class PrivateFileMsg : public MsgStruct
{
public:
	//String^ strUsername;
	//String^ strFilename;
	//int iPackageNumber;
	//int iTotalPackage;

	//array<Byte>^ bData;

	PrivateFileMsg();
	void pack(MessageType msgtype, String^ username, String^ fname, int PackageNumber, int TotalPackage, array<Byte>^ Data);
};
/*
ref class RequestFileMsg : public MsgStruct
{
public:
	String^ strUsername;
	String^ strFileName;
	int iFileSize;

	RequestFileMsg();
	//virtual array<Byte>^ pack() override;
	//virtual MsgStruct^ unpack(array<Byte>^ buff) override;
};

ref class ResponseFileMsg : public MsgStruct
{
public:
	String^ strUsername;
	bool IsAccept;

	ResponseFileMsg();
	//virtual array<Byte>^ pack() override;
	//virtual MsgStruct^ unpack(array<Byte>^ buff) override;
};

*/