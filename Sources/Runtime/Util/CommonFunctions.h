#pragma once
#include "TypeDefine.h"
#include <string>

#define QStringToString(x)  x.toUtf8().constData()
#define TO_String(x)	#x

template<class T>
inline String DataToString(T Data)
{
	return std::to_string(Data);
}

template<class T>
T StringToData(const String& Str)
{
	if(typeid(T) == typeid(float32))
	{
		return (T)std::stof(Str);
	}
	else if(typeid(T) == typeid(float64))
	{
		return (T)std::stod(Str);
	}
	else if (typeid(T) == typeid(int8))
	{
		int32 D = std::stoi(Str);
		return (T)D;
	}
	else if (typeid(T) == typeid(int16))
	{
		int32 D = std::stoi(Str);
		return (T)D;
	}
	else if (typeid(T) == typeid(int32))
	{
		return (T)std::stoi(Str);
	}
	else if (typeid(T) == typeid(int64))
	{
		return (T)std::stoll(Str);
	}
	else if (typeid(T) == typeid(uint8))
	{
		int32 D = std::stoi(Str);
		return (T)D;
	}
	else if (typeid(T) == typeid(uint16))
	{
		int32 D = std::stoi(Str);
		return (T)D;
	}
	else if (typeid(T) == typeid(uint32))
	{
		uint64 D = std::stoull(Str);
		return (T)D;
	}
	else if (typeid(T) == typeid(uint64))
	{
		return (T)std::stoull(Str);
	}
	else
	{
		std::cout << "StringToData: convert not-support type" << std::endl;
		return (T)0;
	}
}

std::vector<String> split(const String& str, const String& delim); 

String GetNameFromPath(const String& Path);
String GetSuperPathFromPath(const String& Path);
String GetNameFromPathExceptSuffix(const String& Path);
String GetNameExceptSuffix(const String& FullName);
String GetSuffixFromPath(const String& Path);
void DeleteStringSpaces(String* Path);

//String SearchFileFromPath(const String& Path);
