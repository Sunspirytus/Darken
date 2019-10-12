#include "ShaderHelper.h"
#include "CommonFunctions.h"
#include "EngineRoot.h"
#include "FileIO.h"

ShaderHelper::ShaderHelper()
{
}

ShaderHelper::~ShaderHelper()
{
}

void ShaderHelper::InsertIncludeCode(String* SourceCode)
{
	std::vector<String> InternalShaderNames;
	std::vector<String> ExternalShaderNames;
	ShaderHelper::GetIncludeFileName(SourceCode, &ExternalShaderNames, &InternalShaderNames);

	if (InternalShaderNames.size() == 0 && ExternalShaderNames.size() == 0)
	{
		return;
	}

	String AssetFolder = DKEngine::GetInstance().GetAssetFolderPath();
	String InternalShaderFolder = DKEngine::GetInstance().GetInternalShaderPath();

	for(int32 ShaderIndex = 0; ShaderIndex < InternalShaderNames.size(); ShaderIndex++)
	{
		String InsertShaderCode;
		FileIO File;
		File.LoadFile(InternalShaderFolder + InternalShaderNames[ShaderIndex], &InsertShaderCode);
		String ReShaderTag = ReShaderTag_Internal + DataToString(ShaderIndex);
		int32 InsertPos = SourceCode->find(ReShaderTag);
		if(InsertPos == -1)
		{
			std::cout << "Error!!! Insert Internal Shader Include File!" << std::endl;
			continue;
		}
		SourceCode->replace(InsertPos, ReShaderTag.length(), InsertShaderCode);
	}
	for (int32 ShaderIndex = 0; ShaderIndex < ExternalShaderNames.size(); ShaderIndex++)
	{
		String InsertShaderCode;
		FileIO File;
		File.LoadFile(AssetFolder + ExternalShaderNames[ShaderIndex], &InsertShaderCode);
		String ReShaderTag = ReShaderTag_External + DataToString(ShaderIndex);
		int32 InsertPos = SourceCode->find(ReShaderTag);
		if (InsertPos == -1)
		{
			std::cout << "Error!!! Insert External Shader Include File!" << std::endl;
			continue;
		}
		SourceCode->replace(InsertPos, ReShaderTag.length(), InsertShaderCode);
	}
	ShaderHelper::InsertIncludeCode(SourceCode);
}

void ShaderHelper::GetIncludeFileName(String* SourceCode, std::vector<String>* ExternalShaders, std::vector<String>* InternalShaders)
{
	const String Tag = "#include";
	const int8 InternalShaderNameTag0 = '<';
	const int8 InternalShaderNameTag1 = '>';

	const int8 ExternalShaderNameTag0 = '"';
	const int8 ExternalShaderNameTag1 = '"';

	while (true)
	{
		int64 TagPos = SourceCode->find(Tag);
		if (TagPos == -1) break;

		int64 ShaderNamePos0 = TagPos + Tag.length();
		int64 ShaderNamePos1 = 0;
		while(SourceCode->data()[ShaderNamePos0] == ' ')
		{
			ShaderNamePos0++;
		}
		if(SourceCode->data()[ShaderNamePos0] == InternalShaderNameTag0)
		{
			ShaderNamePos1 = SourceCode->find(InternalShaderNameTag1, ShaderNamePos0 + (int64)1);
			String ShaderName = SourceCode->substr(ShaderNamePos0 + (int64)1, ShaderNamePos1 - ShaderNamePos0 - 1);
			SourceCode->replace(TagPos, ShaderNamePos1 - TagPos + 1, ReShaderTag_Internal + DataToString(InternalShaders->size()));
			InternalShaders->push_back(ShaderName);			
		}
		else if (SourceCode->data()[ShaderNamePos0] == ExternalShaderNameTag0)
		{
			ShaderNamePos1 = SourceCode->find(ExternalShaderNameTag1, ShaderNamePos0 + 1);
			String ShaderName = SourceCode->substr(ShaderNamePos0 + (int64)1, ShaderNamePos1 - ShaderNamePos0 - 1);
			SourceCode->replace(TagPos, ShaderNamePos1 - TagPos + 1, ReShaderTag_External + DataToString(ExternalShaders->size()));
			ExternalShaders->push_back(ShaderName);
		}
		else
		{
			std::cout << "Error!!! Find Shader Include File!" << std::endl;
		}
	}
}

ShaderType ShaderHelper::GetShaderType(const String& ShaderName)
{
	int32 loc = (int32)ShaderName.rfind('.');
	const int8 ShaderType = ShaderName[loc + 1];

	switch (ShaderType)
	{
	case 'v': return ShaderType::VertexShader;
	case 'f': return ShaderType::FragmentShader;
	case 'g': return ShaderType::GeometryShader;
	case 'e': return ShaderType::TessEvaluationShader;
	case 'c': 
		if (ShaderName[loc - 1] == 't') return ShaderType::TessControlShader; 
		else return ShaderType::ComputeShader;
	default:
		std::cout << "UnSupport Shader Type: " << ShaderName << std::endl;
		return VertexShader;
	}
}