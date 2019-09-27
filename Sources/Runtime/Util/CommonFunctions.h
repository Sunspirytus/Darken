#pragma once
#include "TypeDefine.h"
#include <string>

#define QStringToString(x)  x.toUtf8().constData()

template<class T>
inline String DataToString(T Data)
{
	return std::to_string(Data);
}


String GetNameFromPath(const String& Path);
String GetNameFromPathExceptSuffix(const String& Path);
String GetNameExceptSuffix(const String& FullName);
