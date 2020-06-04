#pragma  once
#include "MsgStruct.h"

ref class MsgControl
{
public:
	static MsgStruct^ unpack(array<Byte>^ buff);
};