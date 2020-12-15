#include "MsgStruct.h"

void MsgStruct::pullMsg(String^ msg) {
	if (msg != nullptr)
	{
		content->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(msg))); //Length 
		content->AddRange(Encoding::UTF8->GetBytes(msg)); //string
	}
	else
		content->AddRange(BitConverter::GetBytes(0));
};

void MsgStruct::pullInt(int i) {
	content->AddRange(BitConverter::GetBytes(i));
}

void MsgStruct::pullBool(bool b) {
	content->AddRange(BitConverter::GetBytes(b));
}

void MsgStruct::pullData(array<Byte>^ data) {
	if (data != nullptr) {
		pullInt(data->Length);
		content->AddRange(data);
	}
}

void MsgStruct::pack(MessageType msgtype, String^ tousr, String^ usrname) {
	content->AddRange(BitConverter::GetBytes(int(msgtype)));
	pullMsg(tousr);
	pullMsg(usrname);
}

void RecMsgStruct::unpack(array<Byte>^ buff) {
	int offset = 0;
	int msglen;
	String^ strListOnlineUsers = "";
	msgType = (MessageType)BitConverter::ToInt32(buff, offset);
	offset += 4;
	switch (msgType) {
	case MessageType::Login:
		offset += 4;
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4;
		usrname = Encoding::UTF8->GetString(buff, offset, msglen);
		offset += msglen; //Update offset
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update offset
		password = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	case MessageType::LoginNotification:
		offset += 4;
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4;
		usrname = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	case MessageType::LogoutNotification:
		offset += 4;
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4;
		usrname = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	case MessageType::ResponseLogin:
		offset += 8;
		IsSuccess = BitConverter::ToBoolean(buff, offset);
		offset += 1;
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4;
		if (msglen > 0)
			errorMsg = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	case MessageType::Signup:
		offset += 4;
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4;
		usrname = Encoding::UTF8->GetString(buff, offset, msglen);
		offset += msglen; //Update offset
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update offset
		password = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	case MessageType::ResponseSignup:
		offset += 8;
		IsSuccess = BitConverter::ToBoolean(buff, offset);
		offset += 1;
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4;
		if (msglen > 0)
			errorMsg = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	case MessageType::PublicMessage:
		offset += 8;
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update offset
		strMessage = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	case MessageType::PrivateMessage:
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4;
		ToUsername = Encoding::UTF8->GetString(buff, offset, msglen);
		offset += msglen;
		offset += 4;
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update offset
		strMessage = Encoding::UTF8->GetString(buff, offset, msglen);
		break;
	case MessageType::UserStatus:
		offset += 8;


		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update Offset
		if (msglen > 0)
		{
			strListOnlineUsers = Encoding::UTF8->GetString(buff, offset, msglen);

			//Split string to list
			array<wchar_t>^ delimiterChars = gcnew array<wchar_t>(1);
			delimiterChars[0] = '|';
			lstOnlineUsers = strListOnlineUsers->Split(delimiterChars);
		}
		break;
	case MessageType::RequestSendFile:
		offset += 4;
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update Offset
		if (msglen > 0)
			usrname = Encoding::UTF8->GetString(buff, offset, msglen);

		offset += msglen; //Update offset

		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update offset
		if (msglen > 0)
			Filename = Encoding::UTF8->GetString(buff, offset, msglen);

		offset += msglen;
		FileSize = BitConverter::ToInt32(buff, offset);
		break;
	case MessageType::ResponseSendFile:
		offset += 4; 
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update Offset
		if (msglen > 0)
			usrname = Encoding::UTF8->GetString(buff, offset, msglen);

		offset += msglen; //Update offset
		IsSuccess = BitConverter::ToBoolean(buff, offset);
		break;
	case MessageType::PrivateFile:
		offset += 4;
		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update Offset
		if (msglen > 0)
			usrname = Encoding::UTF8->GetString(buff, offset, msglen);
		offset += msglen; //Update offset

		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update offset
		if (msglen > 0)
			Filename = Encoding::UTF8->GetString(buff, offset, msglen);
		offset += msglen;
		iPackageNumber = BitConverter::ToInt32(buff, offset);
		offset += 4;
		iTotalPackage = BitConverter::ToInt32(buff, offset);
		offset += 4;

		msglen = BitConverter::ToInt32(buff, offset);
		offset += 4;
		if (msglen > 0) {
			bData = gcnew array<Byte>(msglen);
			System::Array::Copy(buff, offset, bData, 0, msglen);
		}
			
		break;
	}
}