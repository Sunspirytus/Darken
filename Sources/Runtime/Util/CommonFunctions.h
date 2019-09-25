#pragma once
#include "TypeDefine.h"
#include <string>

template<class T>
inline std::string DataToString(T Data)
{
	return std::to_string(Data);
}

template<class T>
std::string PropertyToString(std::string Name, VariableType Type, T* Data)
{
	std::string DataString;
	switch (Type)
	{
	case STRING: 
		DataString = *(std::string*)Data;
		break;
	case BOOL_1:
	{
		Bool* D = (Bool*)Data;
		DataString = D ? TO_String(true) : TO_String(false);
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
		DataString = std::string(str);
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
	return "\t" + Name + ":" + DataString + ";\n";
}
