#include "FileMsg.h"
/*
PrivateFileMsg::PrivateFileMsg()
{
	strUsername = nullptr;
	strFilename = nullptr;
	iPackageNumber = 0;
	iTotalPackage = 0;
	bData = nullptr;
}

array<Byte>^ PrivateFileMsg::pack()
{
	List<Byte>^ byteData = gcnew List<Byte>();
	byteData->AddRange(BitConverter::GetBytes(int(MessageType::PrivateFile)));

	//add Username info
	if (strUsername != nullptr)
	{
		byteData->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(strUsername))); //Length of username
		byteData->AddRange(Encoding::UTF8->GetBytes(strUsername)); //Username string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));


	//add Filename Info
	if (strFilename != nullptr)
	{
		byteData->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(strFilename))); //Length of strFilename
		byteData->AddRange(Encoding::UTF8->GetBytes(strFilename)); //strFilename string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));

	//file package number and total package info
	byteData->AddRange(BitConverter::GetBytes(iPackageNumber));
	byteData->AddRange(BitConverter::GetBytes(iTotalPackage));

	//Add data info
	byteData->AddRange(BitConverter::GetBytes(bData->Length));
	byteData->AddRange(bData);
	//Return
	return byteData->ToArray();
}

MsgStruct^ PrivateFileMsg::unpack(array<Byte>^ buff)
{
	int offset = 4; //Skip messageType
	int usernameLength, filenameLength;

	usernameLength = BitConverter::ToInt32(buff, offset);
	offset += 4; //Update Offset
	if (usernameLength > 0)
		strUsername = Encoding::UTF8->GetString(buff, offset, usernameLength);

	offset += usernameLength; //Update offset

	filenameLength = BitConverter::ToInt32(buff, offset);
	offset += 4; //Update offset
	if (filenameLength > 0)
		strFilename = Encoding::UTF8->GetString(buff, offset, filenameLength);

	offset += filenameLength;
	iPackageNumber = BitConverter::ToInt32(buff, offset);
	offset += 4;
	iTotalPackage = BitConverter::ToInt32(buff, offset);
	offset += 4;

	int dataSize = BitConverter::ToInt32(buff, offset);
	offset += 4;
	if (dataSize > 0)
		System::Array::Copy(buff, offset, bData, 0, dataSize);

	return this;
}

RequestFileMsg::RequestFileMsg()
{
	strUsername = nullptr;
	strFileName = nullptr;
	iFileSize = 0;
}

array<Byte>^ RequestFileMsg::pack()
{
	List<Byte>^ byteData = gcnew List<Byte>();
	byteData->AddRange(BitConverter::GetBytes(int(MessageType::RequestSendFile)));

	//add Username info
	if (strUsername != nullptr)
	{
		byteData->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(strUsername))); //Length of username
		byteData->AddRange(Encoding::UTF8->GetBytes(strUsername)); //Username string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));


	//add FileName Info
	if (strFileName != nullptr)
	{
		byteData->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(strFileName))); //Length of FileName
		byteData->AddRange(Encoding::UTF8->GetBytes(strFileName)); //FileName string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));

	//add FileSize info
	byteData->AddRange(BitConverter::GetBytes(iFileSize));

	//Return
	return byteData->ToArray();
}

MsgStruct^ RequestFileMsg::unpack(array<Byte>^ buff)
{
	int offset = 4; //Skip messageType
	int usernameLength, filenameLength;

	usernameLength = BitConverter::ToInt32(buff, offset);
	offset += 4; //Update Offset
	if (usernameLength > 0)
		strUsername = Encoding::UTF8->GetString(buff, offset, usernameLength);

	offset += usernameLength; //Update offset

	filenameLength = BitConverter::ToInt32(buff, offset);
	offset += 4; //Update offset
	if (filenameLength > 0)
		strFileName = Encoding::UTF8->GetString(buff, offset, filenameLength);

	offset += filenameLength;
	iFileSize = BitConverter::ToInt32(buff, offset);

	return this;
}

ResponseFileMsg::ResponseFileMsg()
{
	strUsername = nullptr;
	IsAccept = false;
}

array<Byte>^ ResponseFileMsg::pack()
{
	List<Byte>^ byteData = gcnew List<Byte>();
	byteData->AddRange(BitConverter::GetBytes(int(MessageType::ResponseSendFile)));

	//add Username info
	if (strUsername != nullptr)
	{
		byteData->AddRange(BitConverter::GetBytes(Encoding::UTF8->GetByteCount(strUsername))); //Length of username
		byteData->AddRange(Encoding::UTF8->GetBytes(strUsername)); //Username string
	}
	else
		byteData->AddRange(BitConverter::GetBytes(0));


	//add IsAccept Info
	byteData->AddRange(BitConverter::GetBytes(IsAccept));

	//Return
	return byteData->ToArray();
}

MsgStruct^ ResponseFileMsg::unpack(array<Byte>^ buff)
{
	int offset = 4; //Skip messageType
	int usernameLength;

	usernameLength = BitConverter::ToInt32(buff, offset);
	offset += 4; //Update Offset
	if (usernameLength > 0)
		strUsername = Encoding::UTF8->GetString(buff, offset, usernameLength);

	offset += usernameLength; //Update offset
	IsAccept = BitConverter::ToBoolean(buff, offset);

	return this;
}
*/