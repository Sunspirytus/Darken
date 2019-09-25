#pragma once
#include "TypeDefine.h"

class PropertyBase
{
public:
	PropertyBase() {};
	~PropertyBase() {};

	virtual void Save(String* Data) = 0;
	//virtual void Load(const String& Data) = 0;
};