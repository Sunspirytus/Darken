#pragma once
#include "TypeDefine.h"
#include <map>

struct PropertyData
{
	VariableType Type;
	void* Data;
	std::map<int32, String> DatasInString;
	struct PropertyData(VariableType type, void* data, const std::map<int32, String>& datasInString)
		:	Type(type)
		,	Data(data)
		,	DatasInString(datasInString)
	{
	}
};

class PropertyBase
{
public:
	PropertyBase() {};
	~PropertyBase() {};

	virtual void Save(String* Data);
	//virtual void Load(const String& Data) = 0;

	void AddProperty(const String& name, VariableType type, void* data, const std::map<int32, String>& datasInString = std::map<int32, String>{});

	std::vector<std::pair<String, std::shared_ptr<PropertyData>>> Properties;
private:
	String PropertyToString(const String& name, std::shared_ptr<PropertyData> property);
};