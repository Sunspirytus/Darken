#include "PropertyBase.h"
#include "CommonFunctions.h"

int32 TabCount = 0;

PropertyBase::PropertyBase()
{
};

PropertyBase::~PropertyBase()
{
};

void PropertyBase::AddProperty(const String& name, VariableType type, void* data, const std::map<int32, String>& datasInString)
{
	std::shared_ptr<PropertyData> NewProperty = std::shared_ptr<PropertyData>(new PropertyData(type, data, datasInString));
	Properties.push_back(std::pair<String, std::shared_ptr<PropertyData>>(name, NewProperty));
}

void PropertyBase::Save(String* Data)
{
	for(int32 Index = 0; Index < int32(Properties.size()); Index++)
	{
		const String& PropertyName = Properties[Index].first;
		std::shared_ptr<PropertyData> Property = Properties[Index].second;
		AddTab(Data);
		Data->append(PropertyToString(PropertyName, Property));
	}
}

void PropertyBase::Load(const String& Data)
{
	for (int32 Index = 0; Index < int32(Properties.size()); Index++)
	{
		const String& PropertyName = Properties[Index].first;
		std::shared_ptr<PropertyData> Property = Properties[Index].second;
		String BeginTag = PropertyName + ":";
		String EndTag = ";";
		int32 BeginPos = Data.find(BeginTag);
		int32 EndPos = Data.find(EndTag, BeginPos);
		String StrPorperty = Data.substr(BeginPos + BeginTag.length(), EndPos - BeginPos - BeginTag.length());
		StringToProperty(StrPorperty, Property);
	}
}


String PropertyBase::PropertyToString(const String& name, std::shared_ptr<PropertyData> property)
{
	VariableType Type = property->Type;
	void* Data = property->Data;

	String DataString;
	switch (Type)
	{
	case STRING:
		DataString = *(String*)Data;
		break;
	case BOOL_1:
	{
		Bool* D = (Bool*)Data;
		DataString = *D ? TO_String(true) : TO_String(false);
		break;
	}
	case FLOAT_16:
	{
		float16* D = (float16*)Data;
		float32 D32 = (float32)* D;
		DataString = DataToString(D32);
		break;
	}
	case FLOAT_32:
	{
		float32* D = (float32*)Data;
		DataString = DataToString(*D);
		break;
	}
	case FLOAT_64:
	{
		float64* D = (float64*)Data;
		DataString = DataToString(*D);
		break;
	}
	case INT_8:
	{
		int8* D = (int8*)Data;
		DataString = DataToString(*D);
		break;
	}
	case INT_16:
	{
		int16* D = (int16*)Data;
		DataString = DataToString(*D);
		break;
	}
	case ENUM:
	{
		int32* D = (int32*)Data;
		DataString = property->DatasInString[*D];
		break;
	}
	case INT_32:
	{
		int32* D = (int32*)Data;
		DataString = DataToString(*D);
		break;
	}
	case INT_64:
	{
		int64* D = (int64*)Data;
		DataString = DataToString(*D);
		break;
	}
	case UINT_8:
	{
		uint8* D = (uint8*)Data;
		DataString = DataToString(*D);
		break;
	}
	case UINT_16:
	{
		uint16* D = (uint16*)Data;
		DataString = DataToString(*D);
		break;
	}
	case UINT_32:
	{
		uint32* D = (uint32*)Data;
		DataString = DataToString(*D);
		break;
	}
	case UINT_64:
	{
		uint64* D = (uint64*)Data;
		char str[256];
		sprintf_s(str, "%llu", *D);
		DataString = String(str);
		break;
	}
	case VECTOR2_F:
	{
		Vector2f* D = (Vector2f*)Data;
		DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + ")";
		break;
	}
	case VECTOR3_F:
	{
		Vector3f* D = (Vector3f*)Data;
		DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + "," + DataToString(D->z) + ")";
		break;
	}
	case VECTOR4_F:
	{
		Vector4f* D = (Vector4f*)Data;
		DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + "," + DataToString(D->z) + "," + DataToString(D->w) + ")";
		break;
	}
	case VECTOR2_D:
	{
		Vector2d* D = (Vector2d*)Data;
		DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + ")";
		break;
	}
	case VECTOR3_D:
	{
		Vector3d* D = (Vector3d*)Data;
		DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + "," + DataToString(D->z) + ")";
		break;
	}
	case VECTOR4_D:
	{
		Vector4d* D = (Vector4d*)Data;
		DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + "," + DataToString(D->z) + "," + DataToString(D->w) + ")";
		break;
	}
	case VECTOR2_I:
	{
		Vector2i* D = (Vector2i*)Data;
		DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + ")";
		break;
	}
	case VECTOR3_I:
	{
		Vector3i* D = (Vector3i*)Data;
		DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + "," + DataToString(D->z) + ")";
		break;
	}
	case VECTOR4_I:
	{
		Vector4i* D = (Vector4i*)Data;
		DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + "," + DataToString(D->z) + "," + DataToString(D->w) + ")";
		break;
	}
	case MAT2_F:
	case MAT3_F:
	case MAT4_F:
	case MAT2_D:
	case MAT3_D:
	case MAT4_D:
		std::cout << "do not support save mat now" << std::endl;
		break;
	default:
		break;
	}
	return name + ":" + DataString + ";\n";
}

void PropertyBase::StringToProperty(const String& StrData, std::shared_ptr<PropertyData> property)
{
	VariableType Type = property->Type;
	void* Data = property->Data;

	switch (Type)
	{
	case STRING:
		*(String*)Data = StrData;
		break;
	case BOOL_1:
	{
		Bool D = StrData == TO_String(true)? true : false;
		*(Bool*)Data = D;
		break;
	}
	case FLOAT_16:
	{
		float32 D32 = StringToData<float32>(StrData);
		float16 D = (float16)D32;
		*(float16*)Data = D;
		break;
	}
	case FLOAT_32:
	{
		float32 D = StringToData<float32>(StrData);
		*(float32*)Data = D;
		break;
	}
	case FLOAT_64:
	{
		float64 D = StringToData<float64>(StrData);
		*(float64*)Data = D;
		break;
	}
	case INT_8:
	{
		int8 D = StringToData<int8>(StrData);
		*(int8*)Data = D;
		break;
	}
	case INT_16:
	{
		int16 D = StringToData<int16>(StrData);
		*(int16*)Data = D;
		break;
	}
	case ENUM:
	{
		for(std::map<int32, String>::iterator it = property->DatasInString.begin(); it != property->DatasInString.end(); it++)
		{
			if(StrData == it->second)
			{
				*(int32*)Data = it->first;
				break;
			}
		}
		break;
	}
	case INT_32:
	{
		int32 D = StringToData<int32>(StrData);
		*(int32*)Data = D;
		break;
	}
	case INT_64:
	{
		int64 D = StringToData<int64>(StrData);
		*(int64*)Data = D;
		break;
	}
	case UINT_8:
	{
		uint8 D = StringToData<uint8>(StrData);
		*(uint8*)Data = D;
		break;
	}
	case UINT_16:
	{
		uint16 D = StringToData<uint16>(StrData);
		*(uint16*)Data = D;
		break;
	}
	case UINT_32:
	{
		uint32 D = StringToData<uint32>(StrData);
		*(uint32*)Data = D;
		break;
	}
	case UINT_64:
	{
		uint64 D = StringToData<uint64>(StrData);
		*(uint64*)Data = D;
		break;
	}
	case VECTOR2_F:
	{
	
		Vector2f* D = (Vector2f*)Data;
		//DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + ")";
		break;
	}
	case VECTOR3_F:
	{
		Vector3f* D = (Vector3f*)Data;
		//DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + "," + DataToString(D->z) + ")";
		break;
	}
	case VECTOR4_F:
	{
		Vector4f* D = (Vector4f*)Data;
		//DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + "," + DataToString(D->z) + "," + DataToString(D->w) + ")";
		break;
	}
	case VECTOR2_D:
	{
		Vector2d* D = (Vector2d*)Data;
		//DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + ")";
		break;
	}
	case VECTOR3_D:
	{
		Vector3d* D = (Vector3d*)Data;
		//DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + "," + DataToString(D->z) + ")";
		break;
	}
	case VECTOR4_D:
	{
		Vector4d* D = (Vector4d*)Data;
		//DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + "," + DataToString(D->z) + "," + DataToString(D->w) + ")";
		break;
	}
	case VECTOR2_I:
	{
		Vector2i* D = (Vector2i*)Data;
		//DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + ")";
		break;
	}
	case VECTOR3_I:
	{
		Vector3i* D = (Vector3i*)Data;
		//DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + "," + DataToString(D->z) + ")";
		break;
	}
	case VECTOR4_I:
	{
		Vector4i* D = (Vector4i*)Data;
		//DataString = "(" + DataToString(D->x) + "," + DataToString(D->y) + "," + DataToString(D->z) + "," + DataToString(D->w) + ")";
		break;
	}
	case MAT2_F:
	case MAT3_F:
	case MAT4_F:
	case MAT2_D:
	case MAT3_D:
	case MAT4_D:
		std::cout << "do not support load mat now" << std::endl;
		break;
	default:
		break;
	}
}

void PropertyBase::AddTab(String* Data)
{
	for(int32 i = 0; i < TabCount; i++)
	{
		Data->append("    ");
	}
}