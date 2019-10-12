#pragma once
#include "TypeDefine.h"
#include <map>

enum PropertyType
{
	BaseInfo = 1,
	ComponentInfo
};

struct PropertyData
{
	VariableType Type;
	void* Data;
	std::map<int32, String> DatasInString;
	struct PropertyData(VariableType type, void* data, const std::map<int32, String>& datasInString = std::map<int32, String>{})
		:	Type(type)
		,	Data(data)
		,	DatasInString(datasInString)
	{
	}
};

struct PropertyArrayData
{
	VariableType Type;
	void* Data;
	std::map<int32, String> DatasInString;
	struct PropertyArrayData(VariableType type, void* data, const std::map<int32, String>& datasInString = std::map<int32, String>{})
		: Type(type)
		, Data(data)
		, DatasInString(datasInString)
	{
	}
};

class PropertyBase
{
public:
	PropertyBase();
	~PropertyBase();

	virtual void Save(String* Data);
	virtual void Load(const String& Data);

protected:
	void AddProperty(const String& name, VariableType type, void* data, const std::map<int32, String>& datasInString = std::map<int32, String>{});
	void AddPropertyArray(const String& name, VariableType type, void* data);

	void AddTab(String* Data);

	void PrepareToWrite(String* Data);
	void FinishWrite(String* Data);
	void BeginWriteProperty(String* Data, PropertyType type);
	void EndWriteProperty(String* Data, PropertyType type);
	void BeginReadProperty(String* OutDataToProcess, const String& Data, PropertyType type);
	void EndReadProperty(String* OutDataToProcess, PropertyType type);

	std::vector<std::pair<String, std::shared_ptr<PropertyData>>> Properties;
	std::vector<std::pair<String, std::shared_ptr<PropertyArrayData>>> Properties_Array;

	String PropertyToString(const String& name, std::shared_ptr<PropertyData> property);
	String PropertyArrayToString(const String& name, std::shared_ptr<PropertyArrayData> property);
	void StringToProperty(const String& StrData, std::shared_ptr<PropertyData> property);
	void StringToPropertyArray(const String& StrData, std::shared_ptr<PropertyArrayData> property);

private:
};