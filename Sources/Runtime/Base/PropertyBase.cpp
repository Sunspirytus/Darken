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

void PropertyBase::AddPropertyArray(const String& name, VariableType type, void* data)
{
	std::shared_ptr<PropertyArrayData> NewProperty = std::shared_ptr<PropertyArrayData>(new PropertyArrayData(type, data));
	Properties_Array.push_back(std::pair<String, std::shared_ptr<PropertyArrayData>>(name, NewProperty));
}

void PropertyBase::Save(String* Data)
{
	for(int32 Index = 0; Index < int32(Properties.size()); Index++)
	{
		const String& PropertyName = Properties[Index].first;
		std::shared_ptr<PropertyData> Property = Properties[Index].second;
		AddTab(Data);
		Data->append(PropertyToString(PropertyName, Property));
	}for (int32 Index = 0; Index < int32(Properties_Array.size()); Index++)
	{
		const String& PropertyName = Properties_Array[Index].first;
		std::shared_ptr<PropertyArrayData> Property = Properties_Array[Index].second;
		AddTab(Data);
		Data->append(PropertyArrayToString(PropertyName, Property));
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
		String StrPorperty = Data.substr(BeginPos + BeginTag.length(), (int64)EndPos - BeginPos - BeginTag.length());
		StringToProperty(StrPorperty, Property);
	}
	for (int32 Index = 0; Index < int32(Properties_Array.size()); Index++)
	{
		const String& PropertyName = Properties_Array[Index].first;
		std::shared_ptr<PropertyArrayData> Property = Properties_Array[Index].second;
		String BeginTag = PropertyName + ":";
		String EndTag = ";";
		int32 BeginPos = Data.find(BeginTag);
		int32 EndPos = Data.find(EndTag, BeginPos);
		String StrPorperty = Data.substr(BeginPos + BeginTag.length(), (int64)EndPos - BeginPos - BeginTag.length());
		StringToPropertyArray(StrPorperty, Property);
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
	{
		Mat2f* D = (Mat2f*)Data;
		DataString.append("\n");
		int32 Row = 2, Col = 2;
		for (int32 i = 0; i < Row; i++)
		{
			AddTab(&DataString);
			DataString.append("    ");

			if (i == 0) DataString.append("(");
			else DataString.append(" ");
			for (int32 j = 0; j < Col; j++)
			{
				DataString.append(DataToString((*D)[i][j]));
				if (i != Row - 1 || j != Col - 1) DataString.append(",");
			}
			if (i == Row - 1)DataString.append(")");
			else DataString.append("\n");
		}
		break;
	}
	case MAT3_F:
	{
		Mat3f* D = (Mat3f*)Data;
		DataString.append("\n");
		int32 Row = 3, Col = 3;
		for (int32 i = 0; i < Row; i++)
		{
			AddTab(&DataString);
			DataString.append("    ");

			if (i == 0) DataString.append("(");
			else DataString.append(" ");
			for (int32 j = 0; j < Col; j++)
			{
				DataString.append(DataToString((*D)[i][j]));
				if (i != Row - 1 || j != Col - 1) DataString.append(",");
			}
			if (i == Row - 1)DataString.append(")");
			else DataString.append("\n");
		}
		break;
	}
	case MAT4_F:
	{
		Mat4f* D = (Mat4f*)Data;
		DataString.append("\n");
		int32 Row = 4, Col = 4;
		for (int32 i = 0; i < Row; i++)
		{
			AddTab(&DataString);
			DataString.append("    ");

			if (i == 0) DataString.append("(");
			else DataString.append(" ");
			for (int32 j = 0; j < Col; j++)			
			{
				DataString.append(DataToString((*D)[i][j]));
				if (i != Row - 1 || j != Col - 1) DataString.append(",");
			}
			if (i == Row - 1)DataString.append(")");
			else DataString.append("\n");
		}
		break;
	}
	case MAT2_D:
	{
		Mat2d* D = (Mat2d*)Data;
		DataString.append("\n");
		int32 Row = 2, Col = 2;
		for (int32 i = 0; i < Row; i++)
		{
			AddTab(&DataString);
			DataString.append("    ");

			if (i == 0) DataString.append("(");
			else DataString.append(" ");
			for (int32 j = 0; j < Col; j++)
			{
				DataString.append(DataToString((*D)[i][j]));
				if (i != Row - 1 || j != Col - 1) DataString.append(",");
			}
			if (i == Row - 1)DataString.append(")");
			else DataString.append("\n");
		}
		break;
	}
	case MAT3_D:
	{
		Mat3d* D = (Mat3d*)Data;
		DataString.append("\n");
		int32 Row = 3, Col = 3;
		for (int32 i = 0; i < Row; i++)
		{
			AddTab(&DataString);
			DataString.append("    ");

			if (i == 0) DataString.append("(");
			else DataString.append(" ");
			for (int32 j = 0; j < Col; j++)
			{
				DataString.append(DataToString((*D)[i][j]));
				if (i != Row - 1 || j != Col - 1) DataString.append(",");
			}
			if (i == Row - 1)DataString.append(")");
			else DataString.append("\n");
		}
		break;
	}
	case MAT4_D:
	{
		Mat4d* D = (Mat4d*)Data;
		DataString.append("\n");
		int32 Row = 4, Col = 4;
		for (int32 i = 0; i < Row; i++)
		{
			AddTab(&DataString);
			DataString.append("    ");

			if (i == 0) DataString.append("(");
			else DataString.append(" ");
			for (int32 j = 0; j < Col; j++)
			{
				DataString.append(DataToString((*D)[i][j]));
				if (i != Row - 1 || j != Col - 1) DataString.append(",");
			}
			if (i == Row - 1)DataString.append(")");
			else DataString.append("\n");
		}
		break;
	}
	default:
		std::cout << "Unknow Property Type!!" << std::endl;
		break;
	}
	return name + ":" + DataString + ";\n";
}

String PropertyBase::PropertyArrayToString(const String& name, std::shared_ptr<PropertyArrayData> property)
{
	VariableType SourceType = property->Type;
	void* SourceData = property->Data;

	String DataString;
	switch (SourceType)
	{
	case STRING:
	{
		std::vector<String>* Data = (std::vector<String>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for(int32 Index = 0; Index < Data->size(); Index++)
		{
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + Data->at(Index) + ">");
		}
		break; 
	}
	case BOOL_1:
	{
		std::vector<Bool>* Data = (std::vector<Bool>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for(int32 Index = 0; Index < Data->size(); Index++)
		{
			String S = Data->at(Index) ? TO_String(true) : TO_String(false);
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + S + ">");
		}
		break; 
	}
	case FLOAT_16:
	{
		std::vector<float16>* Data = (std::vector<float16>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + DataToString(Data->at(Index)) + ">");
		}
		break;
	}
	case FLOAT_32:
	{
		std::vector<float32>* Data = (std::vector<float32>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + DataToString(Data->at(Index)) + ">");
		}
		break;
	}
	case FLOAT_64:
	{
		std::vector<float64>* Data = (std::vector<float64>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + DataToString(Data->at(Index)) + ">");
		}
		break;
	}
	case INT_8:
	{
		std::vector<int8>* Data = (std::vector<int8>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + DataToString(Data->at(Index)) + ">");
		}
		break;
	}
	case INT_16:
	{
		std::vector<int16>* Data = (std::vector<int16>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + DataToString(Data->at(Index)) + ">");
		}
		break;
	}
	case ENUM:
	{
		std::vector<int32>* Data = (std::vector<int32>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			int32 E = Data->at(Index);
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + property->DatasInString[E] + ">");
		}
		break;
	}
	case INT_32:
	{
		std::vector<int32>* Data = (std::vector<int32>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			AddTab(&DataString);
			DataString.append("\n");
			DataString.append("    ");
			DataString.append("<" + DataToString(Data->at(Index)) + ">");
		}
		break;
	}
	case INT_64:
	{
		std::vector<int64>* Data = (std::vector<int64>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + DataToString(Data->at(Index)) + ">");
		}
		break;
	}
	case UINT_8:
	{
		std::vector<uint8>* Data = (std::vector<uint8>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + DataToString(Data->at(Index)) + ">");
		}
		break;
	}
	case UINT_16:
	{
		std::vector<uint16>* Data = (std::vector<uint16>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + DataToString(Data->at(Index)) + ">");
		}
		break;
	}
	case UINT_32:
	{
		std::vector<uint32>* Data = (std::vector<uint32>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + DataToString(Data->at(Index)) + ">");
		}
		break;
	}
	case UINT_64:
	{
		std::vector<uint64>* Data = (std::vector<uint64>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + DataToString(Data->at(Index)) + ">");
		}
		break;
	}
	case VECTOR2_F:
	{
		std::vector<Vector2f>* Data = (std::vector<Vector2f>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			String V = "(" + DataToString(Data->at(Index).x) + "," + DataToString(Data->at(Index).y) + ")";
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + V  + ">");
		}
		break;
	}
	case VECTOR3_F:
	{
		std::vector<Vector3f>* Data = (std::vector<Vector3f>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			String V = "(" + DataToString(Data->at(Index).x) + "," + DataToString(Data->at(Index).y) + "," + DataToString(Data->at(Index).z) + ")";
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + V + ">");
		}
		break;
	}
	case VECTOR4_F:
	{
		std::vector<Vector4f>* Data = (std::vector<Vector4f>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			String V = "(" + DataToString(Data->at(Index).x) + "," + DataToString(Data->at(Index).y) + "," + DataToString(Data->at(Index).z) + "," + DataToString(Data->at(Index).w) + ")";
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + V + ">");
		}
		break;
	}
	case VECTOR2_D:
	{
		std::vector<Vector2d>* Data = (std::vector<Vector2d>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			String V = "(" + DataToString(Data->at(Index).x) + "," + DataToString(Data->at(Index).y) + ")";
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + V + ">");
		}
		break;
	}
	case VECTOR3_D:
	{
		std::vector<Vector3d>* Data = (std::vector<Vector3d>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			String V = "(" + DataToString(Data->at(Index).x) + "," + DataToString(Data->at(Index).y) + "," + DataToString(Data->at(Index).z) + ")";
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + V + ">");
		}
		break;
	}
	case VECTOR4_D:
	{
		std::vector<Vector4d>* Data = (std::vector<Vector4d>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			String V = "(" + DataToString(Data->at(Index).x) + "," + DataToString(Data->at(Index).y) + "," + DataToString(Data->at(Index).z) + "," + DataToString(Data->at(Index).w) + ")";
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + V + ">");
		}
		break;
	}
	case VECTOR2_I:
	{
		std::vector<Vector2i>* Data = (std::vector<Vector2i>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			String V = "(" + DataToString(Data->at(Index).x) + "," + DataToString(Data->at(Index).y) + ")";
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + V + ">");
		}
		break;
	}
	case VECTOR3_I:
	{
		std::vector<Vector3i>* Data = (std::vector<Vector3i>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			String V = "(" + DataToString(Data->at(Index).x) + "," + DataToString(Data->at(Index).y) + "," + DataToString(Data->at(Index).z) + ")";
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + V + ">");
		}
		break;
	}
	case VECTOR4_I:
	{
		std::vector<Vector4i>* Data = (std::vector<Vector4i>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")");
		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			String V = "(" + DataToString(Data->at(Index).x) + "," + DataToString(Data->at(Index).y) + "," + DataToString(Data->at(Index).z) + "," + DataToString(Data->at(Index).w) + ")";
			DataString.append("\n");
			AddTab(&DataString);
			DataString.append("    ");
			DataString.append("<" + V + ">");
		}
		break;
	}
	case MAT2_F:
	{
		std::vector<Mat2f>* Data = (std::vector<Mat2f>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")\n");

		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			Mat2f D = Data->at(Index);
			int32 Row = 2, Col = 2;
			String M = "";
			for (int32 i = 0; i < Row; i++)
			{
				AddTab(&M);
				M.append("    ");

				if (i == 0) M.append("<(");
				else M.append(" ");
				for (int32 j = 0; j < Col; j++)
				{
					M.append(DataToString(D[i][j]));
					if (i != Row - 1 || j != Col - 1) M.append(",");
				}
				if (i == Row - 1)M.append(")>");
				else M.append("\n");
			}
			DataString.append(M);
		}
		break;
	}
	case MAT3_F:
	{
		std::vector<Mat3f>* Data = (std::vector<Mat3f>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")\n");

		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			Mat3f D = Data->at(Index);
			int32 Row = 3, Col = 3;
			String M = "";
			for (int32 i = 0; i < Row; i++)
			{
				AddTab(&M);
				M.append("    ");

				if (i == 0) M.append("<(");
				else M.append(" ");
				for (int32 j = 0; j < Col; j++)
				{
					M.append(DataToString(D[i][j]));
					if (i != Row - 1 || j != Col - 1) M.append(",");
				}
				if (i == Row - 1)M.append(")>");
				else M.append("\n");
			}
			DataString.append(M);
		}
		break;
	}
	case MAT4_F:
	{
		std::vector<Mat4f>* Data = (std::vector<Mat4f>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")\n");

		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			Mat4f D = Data->at(Index);
			int32 Row = 4, Col = 4;
			String M = "";
			for (int32 i = 0; i < Row; i++)
			{
				AddTab(&M);
				M.append("    ");

				if (i == 0) M.append("<(");
				else M.append(" ");
				for (int32 j = 0; j < Col; j++)
				{
					M.append(DataToString(D[i][j]));
					if (i != Row - 1 || j != Col - 1) M.append(",");
				}
				if (i == Row - 1)M.append(")>");
				else M.append("\n");
			}
			DataString.append(M);
		}
		break;
	}
	case MAT2_D:
	{
		std::vector<Mat2d>* Data = (std::vector<Mat2d>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")\n");

		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			Mat2d D = Data->at(Index);
			int32 Row = 2, Col = 2;
			String M = "";
			for (int32 i = 0; i < Row; i++)
			{
				AddTab(&M);
				M.append("    ");

				if (i == 0) M.append("<(");
				else M.append(" ");
				for (int32 j = 0; j < Col; j++)
				{
					M.append(DataToString(D[i][j]));
					if (i != Row - 1 || j != Col - 1) M.append(",");
				}
				if (i == Row - 1)M.append(")>");
				else M.append("\n");
			}
			DataString.append(M);
		}
		break;
	}
	case MAT3_D:
	{
		std::vector<Mat3d>* Data = (std::vector<Mat3d>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")\n");

		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			Mat3d D = Data->at(Index);
			int32 Row = 3, Col = 3;
			String M = "";
			for (int32 i = 0; i < Row; i++)
			{
				AddTab(&M);
				M.append("    ");

				if (i == 0) M.append("<(");
				else M.append(" ");
				for (int32 j = 0; j < Col; j++)
				{
					M.append(DataToString(D[i][j]));
					if (i != Row - 1 || j != Col - 1) M.append(",");
				}
				if (i == Row - 1)M.append(")>");
				else M.append("\n");
			}
			DataString.append(M);
		}
		break;
	}
	case MAT4_D:
	{
		std::vector<Mat4d>* Data = (std::vector<Mat4d>*)SourceData;
		DataString.append("(" + DataToString(Data->size()) + ")\n");

		for (int32 Index = 0; Index < Data->size(); Index++)
		{
			Mat4d D = Data->at(Index);
			int32 Row = 4, Col = 4;
			String M = "";
			for (int32 i = 0; i < Row; i++)
			{
				AddTab(&M);
				M.append("    ");

				if (i == 0) M.append("<(");
				else M.append(" ");
				for (int32 j = 0; j < Col; j++)
				{
					M.append(DataToString(D[i][j]));
					if (i != Row - 1 || j != Col - 1) M.append(",");
				}
				if (i == Row - 1)M.append(")>");
				else M.append("\n");
			}
			DataString.append(M);
		}
		break;
	}
	default:
		std::cout << "Unknow Property Type!!" << std::endl;
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
		String D = StrData.substr(1, StrData.length() - 2);
		std::vector<String> VectorD = split(D, ",");
		float32 X = StringToData<float32>(VectorD[0]);
		float32 Y = StringToData<float32>(VectorD[1]);
		*(Vector2f*)Data = Vector2f(X, Y);
		break;
	}
	case VECTOR3_F:
	{
		String D = StrData.substr(1, StrData.length() - 2);
		std::vector<String> VectorD = split(D, ",");
		float32 X = StringToData<float32>(VectorD[0]);
		float32 Y = StringToData<float32>(VectorD[1]);
		float32 Z = StringToData<float32>(VectorD[2]);
		*(Vector3f*)Data = Vector3f(X, Y, Z);
		break;
	}
	case VECTOR4_F:
	{
		String D = StrData.substr(1, StrData.length() - 2);
		std::vector<String> VectorD = split(D, ",");
		float32 X = StringToData<float32>(VectorD[0]);
		float32 Y = StringToData<float32>(VectorD[1]);
		float32 Z = StringToData<float32>(VectorD[2]);
		float32 W = StringToData<float32>(VectorD[3]);
		*(Vector4f*)Data = Vector4f(X, Y, Z, W);
		break;
	}
	case VECTOR2_D:
	{
		String D = StrData.substr(1, StrData.length() - 2);
		std::vector<String> VectorD = split(D, ",");
		float64 X = StringToData<float64>(VectorD[0]);
		float64 Y = StringToData<float64>(VectorD[1]);
		*(Vector2d*)Data = Vector2d(X, Y);
		break;
	}
	case VECTOR3_D:
	{
		String D = StrData.substr(1, StrData.length() - 2);
		std::vector<String> VectorD = split(D, ",");
		float64 X = StringToData<float64>(VectorD[0]);
		float64 Y = StringToData<float64>(VectorD[1]);
		float64 Z = StringToData<float64>(VectorD[2]);
		*(Vector3d*)Data = Vector3d(X, Y, Z);
		break;
	}
	case VECTOR4_D:
	{
		String D = StrData.substr(1, StrData.length() - 2);
		std::vector<String> VectorD = split(D, ",");
		float64 X = StringToData<float64>(VectorD[0]);
		float64 Y = StringToData<float64>(VectorD[1]);
		float64 Z = StringToData<float64>(VectorD[2]);
		float64 W = StringToData<float64>(VectorD[3]);
		*(Vector4d*)Data = Vector4d(X, Y, Z, W);
		break;
	}
	case VECTOR2_I:
	{
		String D = StrData.substr(1, StrData.length() - 2);
		std::vector<String> VectorD = split(D, ",");
		int32 X = StringToData<int32>(VectorD[0]);
		int32 Y = StringToData<int32>(VectorD[1]);
		*(Vector2i*)Data = Vector2i(X, Y);
		break;
	}
	case VECTOR3_I:
	{
		String D = StrData.substr(1, StrData.length() - 2);
		std::vector<String> VectorD = split(D, ",");
		int32 X = StringToData<int32>(VectorD[0]);
		int32 Y = StringToData<int32>(VectorD[1]);
		int32 Z = StringToData<int32>(VectorD[2]);
		*(Vector3i*)Data = Vector3i(X, Y, Z);
		break;
	}
	case VECTOR4_I:
	{
		String D = StrData.substr(1, StrData.length() - 2);
		std::vector<String> VectorD = split(D, ",");
		int32 X = StringToData<int32>(VectorD[0]);
		int32 Y = StringToData<int32>(VectorD[1]);
		int32 Z = StringToData<int32>(VectorD[2]);
		int32 W = StringToData<int32>(VectorD[3]);
		*(Vector4i*)Data = Vector4i(X, Y, Z, W);
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

void PropertyBase::StringToPropertyArray(const String& StrData, std::shared_ptr<PropertyArrayData> property)
{
	VariableType Type = property->Type;
	void* Data = property->Data;

	String SubData = StrData;

	//Read Size
	int32 SizePos0 = SubData.find("(");
	int32 SizePos1 = SubData.find(")", SizePos0);
	int32 Size = StringToData<int32>(SubData.substr((int64)SizePos0 + 1, (int64)SizePos1 - SizePos0 - 1));
	SubData.erase(SizePos0, (int64)SizePos1 - SizePos0 + 1);

	switch (Type)
	{
	case STRING:
	{
		std::vector<String>* D = (std::vector<String>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			(*D)[Index] = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break; 
	}
	case BOOL_1:
	{
		std::vector<Bool>* D = (std::vector<Bool>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			(*D)[Index] = (SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1) == TO_String(true)) ? true : false;
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case FLOAT_16:
	{
		std::vector<float16>* D = (std::vector<float16>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			float32 D32 = StringToData<float32>(SD);
			float16 D16 = (float16)D32;
			(*D)[Index] = D16;
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case FLOAT_32:
	{
		std::vector<float32>* D = (std::vector<float32>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			(*D)[Index] = StringToData<float32>(SD);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case FLOAT_64:
	{
		std::vector<float64>* D = (std::vector<float64>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			(*D)[Index] = StringToData<float64>(SD);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case INT_8:
	{
		std::vector<int8>* D = (std::vector<int8>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			(*D)[Index] = StringToData<int8>(SD);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case INT_16:
	{
		std::vector<int16>* D = (std::vector<int16>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			(*D)[Index] = StringToData<int16>(SD);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case ENUM:
	{
		std::vector<int8>* D = (std::vector<int8>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			for (std::map<int32, String>::iterator it = property->DatasInString.begin(); it != property->DatasInString.end(); it++)
			{
				if (SD == it->second)
				{
					(*D)[Index] = it->first;
					break;
				}
			}
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case INT_32:
	{
		std::vector<int32>* D = (std::vector<int32>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			(*D)[Index] = StringToData<int32>(SD);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case INT_64:
	{
		std::vector<int64>* D = (std::vector<int64>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			(*D)[Index] = StringToData<int64>(SD);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case UINT_8:
	{
		std::vector<uint8>* D = (std::vector<uint8>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			(*D)[Index] = StringToData<uint8>(SD);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case UINT_16:
	{
		std::vector<uint16>* D = (std::vector<uint16>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			(*D)[Index] = StringToData<uint16>(SD);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case UINT_32:
	{
		std::vector<uint32>* D = (std::vector<uint32>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			(*D)[Index] = StringToData<uint32>(SD);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case UINT_64:
	{
		std::vector<uint64>* D = (std::vector<uint64>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			(*D)[Index] = StringToData<uint64>(SD);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case VECTOR2_F:
	{
		std::vector<Vector2f>* D = (std::vector<Vector2f>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			std::vector<String> VectorD = split(SD, ",");
			float32 X = StringToData<float32>(VectorD[0]);
			float32 Y = StringToData<float32>(VectorD[1]);
			(*D)[Index] = Vector2f(X, Y);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case VECTOR3_F:
	{
		std::vector<Vector3f>* D = (std::vector<Vector3f>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			std::vector<String> VectorD = split(SD, ",");
			float32 X = StringToData<float32>(VectorD[0]);
			float32 Y = StringToData<float32>(VectorD[1]);
			float32 Z = StringToData<float32>(VectorD[2]);
			(*D)[Index] = Vector3f(X, Y, Z);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case VECTOR4_F:
	{
		std::vector<Vector4f>* D = (std::vector<Vector4f>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			std::vector<String> VectorD = split(SD, ",");
			float32 X = StringToData<float32>(VectorD[0]);
			float32 Y = StringToData<float32>(VectorD[1]);
			float32 Z = StringToData<float32>(VectorD[2]);
			float32 W = StringToData<float32>(VectorD[3]);
			(*D)[Index] = Vector4f(X, Y, Z, W);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case VECTOR2_D:
	{
		std::vector<Vector2d>* D = (std::vector<Vector2d>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			std::vector<String> VectorD = split(SD, ",");
			float32 X = StringToData<float32>(VectorD[0]);
			float32 Y = StringToData<float32>(VectorD[1]);
			(*D)[Index] = Vector2d(X, Y);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case VECTOR3_D:
	{
		std::vector<Vector3d>* D = (std::vector<Vector3d>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			std::vector<String> VectorD = split(SD, ",");
			float32 X = StringToData<float32>(VectorD[0]);
			float32 Y = StringToData<float32>(VectorD[1]);
			float32 Z = StringToData<float32>(VectorD[2]);
			(*D)[Index] = Vector3d(X, Y, Z);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case VECTOR4_D:
	{
		std::vector<Vector4d>* D = (std::vector<Vector4d>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			std::vector<String> VectorD = split(SD, ",");
			float32 X = StringToData<float32>(VectorD[0]);
			float32 Y = StringToData<float32>(VectorD[1]);
			float32 Z = StringToData<float32>(VectorD[2]);
			float32 W = StringToData<float32>(VectorD[3]);
			(*D)[Index] = Vector4d(X, Y, Z, W);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case VECTOR2_I:
	{
		std::vector<Vector2i>* D = (std::vector<Vector2i>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			std::vector<String> VectorD = split(SD, ",");
			float32 X = StringToData<float32>(VectorD[0]);
			float32 Y = StringToData<float32>(VectorD[1]);
			(*D)[Index] = Vector2i(X, Y);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case VECTOR3_I:
	{
		std::vector<Vector3i>* D = (std::vector<Vector3i>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			std::vector<String> VectorD = split(SD, ",");
			float32 X = StringToData<float32>(VectorD[0]);
			float32 Y = StringToData<float32>(VectorD[1]);
			float32 Z = StringToData<float32>(VectorD[2]);
			(*D)[Index] = Vector3i(X, Y, Z);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
		break;
	}
	case VECTOR4_I:
	{
		std::vector<Vector4i>* D = (std::vector<Vector4i>*)Data;
		D->resize(Size);
		for (int32 Index = 0; Index < Size; Index++)
		{
			int32 SPos0 = SubData.find("<");
			int32 SPos1 = SubData.find(">");
			String SD = SubData.substr((int64)SPos0 + 1, (int64)SPos1 - SPos0 - 1);
			std::vector<String> VectorD = split(SD, ",");
			float32 X = StringToData<float32>(VectorD[0]);
			float32 Y = StringToData<float32>(VectorD[1]);
			float32 Z = StringToData<float32>(VectorD[2]);
			float32 W = StringToData<float32>(VectorD[3]);
			(*D)[Index] = Vector4i(X, Y, Z, W);
			SubData.erase(SPos0, (int64)SPos1 - SPos0 + 1);
		}
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

void PropertyBase::PrepareToWrite(String* Data)
{
	Data->append("{\n");
}

void PropertyBase::FinishWrite(String* Data)
{
	Data->append("}");
}

void PropertyBase::BeginWriteProperty(String* Data, PropertyType type)
{
	switch (type)
	{
	case BaseInfo:
		TabCount = BaseInfo;
		break;
	case ComponentInfo:
	{
		TabCount = ComponentInfo - 1;
		AddTab(Data);
		Data->append("[\n");
		TabCount = ComponentInfo;
		break;
	}
	default:
		break;
	}
}

void PropertyBase::EndWriteProperty(String* Data, PropertyType type)
{
	switch (type)
	{
	case BaseInfo:
		TabCount = 0;
		break;
	case ComponentInfo:
	{
		TabCount = ComponentInfo - 1;
		AddTab(Data);
		Data->append("]\n");
		TabCount = 0;
		break;
	}
	default:
		break;
	}
}


void PropertyBase::BeginReadProperty(String* OutDataToProcess, const String& Data, PropertyType type)
{
	switch (type)
	{
	case BaseInfo:
	{
		int32 BeginPos, EndPos = -1;
		String BeginTag = "{\n";
		String EndTag = "[\n";
		BeginPos = Data.find(BeginTag);
		EndPos = Data.find(EndTag);
		*OutDataToProcess = Data.substr(BeginPos + BeginTag.length(), EndPos - BeginPos - BeginTag.length());
		break;
	}
	case ComponentInfo:
	{
		int32 BeginPos, EndPos = -1;
		String BeginTag = "[\n";
		String EndTag = "]\n";
		BeginPos = Data.find(BeginTag);
		EndPos = Data.find(EndTag);
		*OutDataToProcess = Data.substr(BeginPos + BeginTag.length(), EndPos - BeginPos - BeginTag.length());
		break;
	}
	default:
		break;
	}
}

void PropertyBase::EndReadProperty(String* OutDataToProcess, PropertyType type)
{
	switch (type)
	{
	case BaseInfo:
	{
		int32 BeginPos = -1;
		String BeginTag = "[\n";
		BeginPos = OutDataToProcess->find(BeginTag);
		*OutDataToProcess = OutDataToProcess->substr(BeginPos, OutDataToProcess->length() - BeginPos);
		break;
	}
	case ComponentInfo:
	{
		int32 BeginPos = -1;
		String BeginTag = "]\n";
		BeginPos = OutDataToProcess->find(BeginTag);
		*OutDataToProcess = OutDataToProcess->substr(BeginPos + BeginTag.length(), OutDataToProcess->length() - BeginPos - BeginTag.length());
		break;
	}
	default:
		break;
	}
}