#include "PublicMsg.h"

PublicMsg::PublicMsg()
{
	strMessage = nullptr;
}

array<Byte>^ PublicMsg::pack()
{
	List<Byte>^ byteData = gcnew List<Byte>();
	byteData->AddRange(BitConverter::GetBytes(int(MsgStruct::MessageType::PublicMessage)));

	//add Message info
	if (strMessage != nullptr)
	{
		int lengthMessage = Encoding::UTF8->GetByteCount(strMessage);
		byteData->AddRange(BitConverter::GetBytes(lengthMessage)); //Length of Message
		byteData->AddRange(Encoding::UTF8->GetBytes(strMessage)); //Message string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));

	//Return
	return byteData->ToArray();
}

MsgStruct^ PublicMsg::unpack(array<Byte>^ buff)
{
	int offset = 4; //Skip messageType
	int messageLength;

	messageLength = BitConverter::ToInt32(buff, offset);
	offset += 4; //Update Offset
	if (messageLength > 0)
		strMessage = Encoding::UTF8->GetString(buff, offset, messageLength);

	return this;
}