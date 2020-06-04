#include "SignupMsg.h"
SignupMsg::SignupMsg()
{
	strUsername = nullptr;
	strPassword = nullptr;
}

array<Byte>^ SignupMsg::pack()
{
	List<Byte>^ byteData = gcnew List<Byte>();
	byteData->AddRange(BitConverter::GetBytes(int(MsgStruct::MessageType::Signup)));

	//add Username info
	if (strUsername != nullptr)
	{
		byteData->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(strUsername))); //Length of username
		byteData->AddRange(Encoding::UTF8->GetBytes(strUsername)); //Username string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));


	//add Password Info
	if (strPassword != nullptr)
	{
		byteData->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(strPassword))); //Length of password
		byteData->AddRange(Encoding::UTF8->GetBytes(strPassword)); //Password string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));

	//Return
	return byteData->ToArray();
}

MsgStruct^ SignupMsg::unpack(array<Byte>^ buff)
{
	int offset = 4; //Skip messageType
	int usernameLength, passwordLength;

	usernameLength = BitConverter::ToInt32(buff, offset);
	offset += 4; //Update Offset
	if (usernameLength > 0)
		strUsername = Encoding::UTF8->GetString(buff, offset, usernameLength);

	offset += usernameLength; //Update offset

	passwordLength = BitConverter::ToInt32(buff, offset);
	offset += 4; //Update offset
	if (passwordLength > 0)
		strPassword = Encoding::UTF8->GetString(buff, offset, passwordLength);

	return this;
}

ResSignupMsg::ResSignupMsg()
{
	IsSuccess = false; //Not success
	errorMsg = nullptr;
}

array<Byte>^ ResSignupMsg::pack()
{
	List<Byte>^ byteData = gcnew List<Byte>();
	byteData->AddRange(BitConverter::GetBytes(int(MsgStruct::MessageType::ResponseSignup)));
	byteData->AddRange(BitConverter::GetBytes(IsSuccess));

	if (errorMsg != nullptr)
	{
		byteData->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(errorMsg))); //errorMsg length
		byteData->AddRange(Encoding::UTF8->GetBytes(errorMsg)); //errorMsg string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));

	return byteData->ToArray();
}

MsgStruct^ ResSignupMsg::unpack(array<Byte>^ buff)
{
	int offset = 4;
	IsSuccess = BitConverter::ToBoolean(buff, offset);
	offset += 1;
	int errorMsgLength = BitConverter::ToInt32(buff, offset);
	offset += 4;
	if (errorMsgLength > 0)
		errorMsg = Encoding::UTF8->GetString(buff, offset, errorMsgLength);

	return this;
}
