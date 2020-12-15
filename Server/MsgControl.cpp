#include "MsgControl.h"

RecMsgStruct^ MsgControl::unpack(array<Byte>^ buff)
{
	if (buff->Length == 0)
		return nullptr;

	RecMsgStruct^ result = nullptr; //The result
	MessageType messageType = (MessageType)BitConverter::ToInt32(buff, 0); //Read first 4 byte for messageType

	//
	switch (messageType)
	{
	case MessageType::PublicMessage:
		result = gcnew RecPublicMsg(MessageType::PublicMessage);
		//result->msgType = MessageType::PublicMessage;
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
	case MessageType::UserStatus:
		result = gcnew RecUsrStatusLstMsg();
		result->msgType = MessageType::UserStatus;
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
	case MessageType::Login:
		result = gcnew RecLSMsg(MessageType::Login); //Login Unpack
		//result->messageType = MsgStruct::MessageType::Login;
		result->unpack(buff); //Call Override function
		break;
	case MessageType::ResponseLogin:
		result = gcnew RecLSMsg(MessageType::ResponseLogin);
		//result->messageType = MsgStruct::MessageType::ResponseLogin;
		result->unpack(buff);
		break;
	case MessageType::Signup:
		result = gcnew RecLSMsg(MessageType::Signup);
		//result->messageType = MsgStruct::MessageType::Signup;
		result->unpack(buff);

		break;
	case MessageType::ResponseSignup:
		result = gcnew RecLSMsg(MessageType::ResponseSignup);
		//result->messageType = MsgStruct::MessageType::ResponseSignup;
		result->unpack(buff);

		break;
	case MessageType::LoginNotification:
		result = gcnew RecLSMsg(MessageType::LoginNotification);
		//result->messageType = MsgStruct::MessageType::LoginNotification;
		result->unpack(buff);

		break;
	case MessageType::LogoutNotification:
		result = gcnew RecLSMsg(MessageType::LogoutNotification);
		//result->messageType = MsgStruct::MessageType::LogoutNotification;
		result->unpack(buff);
		break;

	default:
		break;
	}

	return result;
}