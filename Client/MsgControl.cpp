#include "DataStruct.h"

MsgStruct^ MsgControl::unpack(array<Byte>^ buff)
{
	if (buff->Length == 0)
		return nullptr;

	MsgStruct^ result = nullptr; //The result
	MsgStruct::MessageType messageType = (MsgStruct::MessageType)BitConverter::ToInt32(buff, 0); //Read first 4 byte for messageType

	//
	switch (messageType)
	{
	case MsgStruct::MessageType::PublicMessage:
		result = gcnew PublicMsg();
		result->messageType = MsgStruct::MessageType::PublicMessage;
		result->unpack(buff);
		break;
		/*
		case MsgStruct::MessageType::PrivateMessage:
			result = gcnew PrivateMsg();
			result->messageType = MsgStruct::MessageType::PrivateMessage;
			result->unpack(buff);
			break;
		case MsgStruct::MessageType::PrivateFile:
			result = gcnew PrivateFileMsg();
			result->messageType = MsgStruct::MessageType::PrivateFile;
			result->unpack(buff);
			break;
		*/
	case MsgStruct::MessageType::UserStatus:
		result = gcnew UsrStatusLstMsg();
		result->messageType = MsgStruct::MessageType::UserStatus;
		result->unpack(buff);
		break;
		/*
		case MsgStruct::MessageType::RequestSendFile:
			result = gcnew RequestFileMsg();
			result->messageType = MsgStruct::MessageType::RequestSendFile;
			result->unpack(buff);
			break;
		case MsgStruct::MessageType::ResponseSendFile:
			result = gcnew ResponseFileMsg;
			result->messageType = MsgStruct::MessageType::ResponseSendFile;
			result->unpack(buff);
			break;
		*/
	case MsgStruct::MessageType::Login:
		result = gcnew LoginMsg(); //Login Unpack
		result->messageType = MsgStruct::MessageType::Login;
		result->unpack(buff); //Call Override function
		break;
	case MsgStruct::MessageType::ResponseLogin:
		result = gcnew ResLoginMsg();
		result->messageType = MsgStruct::MessageType::ResponseLogin;
		result->unpack(buff);
		break;
	case MsgStruct::MessageType::Signup:
		result = gcnew SignupMsg();
		result->messageType = MsgStruct::MessageType::Signup;
		result->unpack(buff);

		break;
	case MsgStruct::MessageType::ResponseSignup:
		result = gcnew ResSignupMsg();
		result->messageType = MsgStruct::MessageType::ResponseSignup;
		result->unpack(buff);

		break;
	case MsgStruct::MessageType::LoginNotification:
		result = gcnew LoginNotifiMsg();
		result->messageType = MsgStruct::MessageType::LoginNotification;
		result->unpack(buff);

		break;
	case MsgStruct::MessageType::LogoutNotification:
		result = gcnew LogoutNotiMsg();
		result->messageType = MsgStruct::MessageType::LogoutNotification;
		result->unpack(buff);
		break;

	default:
		break;
	}

	return result;
}