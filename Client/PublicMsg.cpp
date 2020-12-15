#include "PublicMsg.h"

PublicMsg::PublicMsg(MessageType msgtype)
{
	content->AddRange(BitConverter::GetBytes(int(msgtype)));
	//strMessage = nullptr;
}


RecPublicMsg::RecPublicMsg(MessageType msgtype)
{
	strMessage = nullptr;
	ToUsername = nullptr;
	msgType = msgtype;
}

void PublicMsg::pack()
{
	//List<Byte>^ byteData = gcnew List<Byte>();
	//content->AddRange(BitConverter::GetBytes(int(MessageType::PublicMessage)));

	//add Message info
	//if (content != nullptr)
	//{
		//int lengthMessage = Encoding::UTF8->GetByteCount(strMessage);
		//byteData->AddRange(BitConverter::GetBytes(lengthMessage)); //Length of Message
		//byteData->AddRange(Encoding::UTF8->GetBytes(strMessage)); //Message string
	//}
	//else
		//byteData->AddRange(BitConverter::GetBytes(0));

	//Return
	//return byteData->ToArray();
	
}

RecMsgStruct^ RecPublicMsg::unpack(array<Byte>^ buff)
{
	int offset = 4; //Skip messageType
	int messageLength;
	if (msgType == MessageType::PrivateFile) {
		messageLength = BitConverter::ToInt32(buff, offset);
		offset += 4; //Update Offset
		if (messageLength > 0) {
			ToUsername = Encoding::UTF8->GetString(buff, offset, messageLength);
			offset += messageLength;
		}
			
	}
	messageLength = BitConverter::ToInt32(buff, offset);
	offset += 4; //Update Offset
	if (messageLength > 0)
		strMessage = Encoding::UTF8->GetString(buff, offset, messageLength);

	return this;
}