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

std::vector<std::shared_ptr<UniformProxyBase>> ShaderHelper::ExpandUniformProperty(String* SourceCode)
{
	String UniformBasicTag = "#UNIFORM";
	String UniformBufferTag = "#UNIFORM_BUFFER";
	String UniformTextureTag = "#UNIFORM_TEXTURE";

	std::vector<std::shared_ptr<UniformProxyBase>> Uniforms;
	while (true)
	{
		int32 Pos0 = SourceCode->find(UniformBufferTag);
		if (Pos0 == -1) break;
		int32 Pos1 = SourceCode->find("\n", Pos0);
		String UniformInfo = SourceCode->substr(Pos0 + UniformBufferTag.length(), Pos1 - Pos0 - UniformBufferTag.length());
		DeleteStringSpaces(&UniformInfo);
		UniformInfo = UniformInfo.substr(1, UniformInfo.length() - 2);
		std::vector<String> Infos = split(UniformInfo, ",");
		String Name = Infos[0];
		String AllocType = Infos[1];
		String Belong = Infos[2];
		std::shared_ptr<UniformProxyBlock> BlockProxy = std::shared_ptr<UniformProxyBlock>(new UniformProxyBlock());
		BlockProxy->Name = Name;
		BlockProxy->AllocType = String_BlockMemoryAllocType_Map[AllocType];
		BlockProxy->Belong = String_UniformBelong_Map[Belong];
		SourceCode->replace(Pos0, Pos1 - Pos0, "layout(std140) uniform " + Name);
		Uniforms.push_back(BlockProxy);
	}
	while (true)
	{
		int32 Pos0 = SourceCode->find(UniformBasicTag);
		if (Pos0 == -1) break;
		int32 Pos1 = SourceCode->find("\n", Pos0);
		String UniformInfo = SourceCode->substr(Pos0 + UniformBasicTag.length(), Pos1 - Pos0 - UniformBasicTag.length());
		DeleteStringSpaces(&UniformInfo);
		UniformInfo = UniformInfo.substr(1, UniformInfo.length() - 2);
		std::vector<String> Infos = split(UniformInfo, ",");
		String Type = Infos[0];
		String Name = Infos[1];
		String Belong = Infos[2];
		std::shared_ptr<UniformProxyBasic> BasicProxy = std::shared_ptr<UniformProxyBasic>(new UniformProxyBasic());
		BasicProxy->Name = Name;
		BasicProxy->Belong = String_UniformBelong_Map[Belong];
		SourceCode->replace(Pos0, Pos1 - Pos0, "uniform " + Type + " " + Name + ";");
		Uniforms.push_back(BasicProxy);
	}
	return Uniforms;
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
		return ShaderType::VertexShader;
	}
}