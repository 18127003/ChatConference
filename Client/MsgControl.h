#pragma  once
#include "DataStruct.h"

ref class MsgControl
{
public:
	static RecMsgStruct^ unpack(array<Byte>^ buff);
};