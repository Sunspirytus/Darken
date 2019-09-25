#pragma once
#include "TypeDefine.h"

class PropertyBase
{
public:
	PropertyBase() {};
	~PropertyBase() {};

	virtual void Save(std::string* Data) = 0;
	//virtual void Load(const std::string& Data) = 0;
};