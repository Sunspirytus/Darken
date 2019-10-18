#include "MaterialInstance.h"
#include "EngineRoot.h"
#include "CommonFunctions.h"

MaterialInstanceBase::MaterialInstanceBase()
{
}
	
MaterialInstanceBase::MaterialInstanceBase(const String& path, const String& parentName)
		: Path(path)
		, ParentName(parentName)
{
	AddProperty("Path", VariableType::STRING, &Path);
	AddProperty("ParentPath", VariableType::STRING, &ParentName);
}

MaterialInstanceBase::~MaterialInstanceBase() 
{
}

String MaterialInstanceBase::GetName()
{
	return Path;
}

MaterialInstance::MaterialInstance(const String& path, std::shared_ptr<Material> parentMaterial)
	:	MaterialInstanceBase(path, parentMaterial->GetPath())
{
	SetParent(parentMaterial);
}

MaterialInstance::~MaterialInstance()
{
	for (std::map<int32, std::shared_ptr<BasicUniformData>>::iterator it = BasicUniformID_DataPtrMap.begin(); it != BasicUniformID_DataPtrMap.end(); it++)
	{
		free(it->second->Data);
	}

	for (std::map<int32, std::shared_ptr<TextureUniformData>>::iterator it = TextureUniformID_DataPtrMap.begin(); it != TextureUniformID_DataPtrMap.end(); it++)
	{
		delete it->second->IDPtr;
	}
	
	for (std::set<std::shared_ptr<UniformItem_Block>>::iterator ItBlock = ParentMaterial->ProgramGPU->Uniforms_Block.begin(); ItBlock != ParentMaterial->ProgramGPU->Uniforms_Block.end(); ItBlock++)
	{
		free((*ItBlock)->DataPtr);
	}
}

void MaterialInstance::SetParent(std::shared_ptr<Material> parentMaterial)
{
	ParentMaterial = parentMaterial;

	void* BlockData;
	for(std::set<std::shared_ptr<UniformItem_Block>>::iterator ItBlock = ParentMaterial->ProgramGPU->Uniforms_Block.begin(); ItBlock != ParentMaterial->ProgramGPU->Uniforms_Block.end(); ItBlock++)
	{
		std::shared_ptr<UniformItem_Block> Block = *ItBlock;
		std::hash<String> hs;
		BlockData = malloc(Block->DataSize_Byte);
		Block->DataPtr = BlockData;
		int32 BlockID = (int32) hs(Block->Name);
		std::map<int32, std::shared_ptr<UniformDataInBlock>> UniformID_PtrMap;
		for(std::map<uint32, std::shared_ptr<UniformItem_WithinBlock>>::iterator ItUniform = Block->Uniforms.begin(); ItUniform != Block->Uniforms.end(); ItUniform++)
		{
			int32 UniformID = (int32) hs(ItUniform->second->Name);
			std::shared_ptr<UniformDataInBlock> UniformPtr = std::shared_ptr<UniformDataInBlock>(new UniformDataInBlock());
			UniformPtr->Data = (void*)((Address)BlockData + ItUniform->second->Offset_Byte);
			UniformPtr->BlockName = Block->Name;
			UniformPtr->UniformName = ItUniform->second->Name;
			UniformID_PtrMap.insert(std::pair<int32, std::shared_ptr<UniformDataInBlock>>(UniformID, UniformPtr));
		}
		BlockID_UniformID_DataPtrMap.insert(std::pair<int32, std::map<int32, std::shared_ptr<UniformDataInBlock>>>(BlockID, UniformID_PtrMap));
		BlockID_BufferPtrMap.insert(std::pair<int32, std::shared_ptr<UniformItem_Block>>(BlockID, Block));
	}

	for (std::set<std::shared_ptr<UniformItem_Basic>>::iterator it = ParentMaterial->ProgramGPU->Uniforms_Basic.begin(); it != ParentMaterial->ProgramGPU->Uniforms_Basic.end(); it++)
	{
		std::shared_ptr<UniformItem_Basic> Uniform = *it;
		std::hash<String> hs;
		int32 ID = (int32) hs(Uniform->Name);
		std::shared_ptr<BasicUniformData> UniformData = std::shared_ptr<BasicUniformData>(new BasicUniformData());
		switch (Uniform->DataType)
		{
		case UniformVariableType::GLSL_INT: UniformData->Data = malloc(sizeof(int32) * Uniform->Size); break;
		case UniformVariableType::GLSL_FLOAT: UniformData->Data = malloc(sizeof(float32) * Uniform->Size); break;
		case UniformVariableType::GLSL_VEC2: UniformData->Data = malloc(sizeof(Vector2f) * Uniform->Size);	break;
		case UniformVariableType::GLSL_VEC3:	UniformData->Data = malloc(sizeof(Vector3f) * Uniform->Size);	break;
		case UniformVariableType::GLSL_VEC4:	UniformData->Data = malloc(sizeof(Vector4f) * Uniform->Size);	break;
		case UniformVariableType::GLSL_IVEC2: UniformData->Data = malloc(sizeof(Vector2i) * Uniform->Size); break;
		case UniformVariableType::GLSL_IVEC3: UniformData->Data = malloc(sizeof(Vector3i) * Uniform->Size); break;
		case UniformVariableType::GLSL_IVEC4: UniformData->Data = malloc(sizeof(Vector4i) * Uniform->Size); break;
		case UniformVariableType::GLSL_MAT3:	UniformData->Data = malloc(sizeof(Mat3f) * Uniform->Size);	break;
		case UniformVariableType::GLSL_MAT4: UniformData->Data = malloc(sizeof(Mat4f) * Uniform->Size); break;
		default:
			UniformData->Data = nullptr;
			break;
		}
		UniformData->Name = Uniform->Name;
		Uniform->DataPtr = UniformData->Data;
		BasicUniformID_DataPtrMap.insert(std::pair<int32, std::shared_ptr<BasicUniformData>>(ID, UniformData));
	}
	for (std::set<std::shared_ptr<UniformItem_Texture>>::iterator it = ParentMaterial->ProgramGPU->Uniforms_Texture.begin(); it != ParentMaterial->ProgramGPU->Uniforms_Texture.end(); it++)
	{
		std::shared_ptr<UniformItem_Texture> TexUniform = *it;
		std::hash<String> hs;
		int32 ID = (int32)hs(TexUniform->Name);
		std::shared_ptr<TextureUniformData> TextureData = std::shared_ptr<TextureUniformData>(new TextureUniformData());
		TextureData->Name = TexUniform->Name;
		TextureData->IDPtr = new uint32;
		TexUniform->IDPtr = TextureData->IDPtr;
		TextureData->Tex = std::shared_ptr<Texture>(new Texture());
		TextureUniformID_DataPtrMap.insert(std::pair<int32, std::shared_ptr<TextureUniformData>>(ID, TextureData));
	}
}

//void MaterialInstance::ChangedParentDynamic(std::shared_ptr<Material> newParentMaterial)
//{	 
//	for(std::unordered_map<String, std::shared_ptr<UniformItem_Block>>::iterator it = newParentMaterial->MaterialProgram->Uniforms_Block.begin(); it != newParentMaterial->MaterialProgram->Uniforms_Block.end(); it++)
//	{
//		std::unordered_map<String, std::shared_ptr<UniformItem_Block>>::iterator it2 = ParentMaterial->MaterialProgram->Uniforms_Block.find(it->first);
//		if(it2 == ParentMaterial->MaterialProgram->Uniforms_Block.end()) std::cout << "No " << it->first << " in new Shader." << std::endl;
//		it->second->DataPtr = it2->second->DataPtr;
//	}
//
//
//	for (std::unordered_map<String, UniformItem_Basic>::iterator it = newParentMaterial->MaterialProgram->Uniforms_Basic.begin(); it != newParentMaterial->MaterialProgram->Uniforms_Basic.end(); it++)
//	{
//		std::unordered_map<String, UniformItem_Basic>::iterator it2 = ParentMaterial->MaterialProgram->Uniforms_Basic.find(it->first);
//		if (it2 == ParentMaterial->MaterialProgram->Uniforms_Basic.end()) std::cout << "No " << it->first << " in new Shader." << std::endl;
//		it->second.DataPtr = it2->second.DataPtr;
//	}
//
//	for (std::unordered_map<String,UniformItem_Texture>::iterator it = newParentMaterial->MaterialProgram->Uniforms_Texture.begin(); it != newParentMaterial->MaterialProgram->Uniforms_Texture.end(); it++)
//	{
//		std::unordered_map<String, UniformItem_Texture>::iterator it2 = ParentMaterial->MaterialProgram->Uniforms_Texture.find(it->first);
//		if (it2 == ParentMaterial->MaterialProgram->Uniforms_Texture.end()) std::cout << "No " << it->first << " in new Shader." << std::endl;
//		it->second.IDPtr = it2->second.IDPtr;
//	}
//	ParentMaterial = newParentMaterial;
//}

std::shared_ptr<Material> MaterialInstance::GetParent()
{
	return ParentMaterial;
}

int32 MaterialInstance::GetID(const String& ParameterName)
{
	std::hash<String> hs;
	return (int32) hs(ParameterName);
}

void MaterialInstance::MarkDirty(std::shared_ptr<UniformItem_Block> BlockPtr)
{
	DKEngine::GetInstance().GetGPUBufferManager()->MarkBufferDirty(BlockPtr);
}

void MaterialInstance::Save(String* Data)
{
	PropertyBase::PrepareToWrite(Data);

	PropertyBase::BeginWriteProperty(Data, PropertyType::BaseInfo);
	MaterialInstanceBase::Save(Data);
	PropertyBase::EndWriteProperty(Data, PropertyType::BaseInfo);

	PropertyBase::BeginWriteProperty(Data, PropertyType::ComponentInfo);
	WriteInstanceData(Data, DataGroup::UNIFORM);
	PropertyBase::EndWriteProperty(Data, PropertyType::ComponentInfo);

	PropertyBase::BeginWriteProperty(Data, PropertyType::ComponentInfo);
	WriteInstanceData(Data, DataGroup::TEXTURE);
	PropertyBase::EndWriteProperty(Data, PropertyType::ComponentInfo);

	PropertyBase::BeginWriteProperty(Data, PropertyType::ComponentInfo);
	WriteInstanceData(Data, DataGroup::UNIFORMBUFFER);
	PropertyBase::EndWriteProperty(Data, PropertyType::ComponentInfo);
	

	PropertyBase::FinishWrite(Data);
}

void MaterialInstance::WriteInstanceData(String* Data, DataGroup type)
{
	switch (type)
	{
	case MaterialInstance::DataGroup::UNIFORM:
	{
		for (std::set<std::shared_ptr<UniformItem_Basic>>::iterator it = ParentMaterial->ProgramGPU->Uniforms_Basic.begin(); it != ParentMaterial->ProgramGPU->Uniforms_Basic.end(); it++)
		{
			std::shared_ptr<UniformItem_Basic> Uniform = *it;
			PropertyBase::AddTab(Data);
			Data->append(PropertyToString(Uniform->Name, std::shared_ptr<PropertyData>(new PropertyData(GPU_CPU_TypeMap[Uniform->DataType], Uniform->DataPtr))));
		}
		break;
	}
	case MaterialInstance::DataGroup::TEXTURE:
		for (std::set<std::shared_ptr<UniformItem_Texture>>::iterator it = ParentMaterial->ProgramGPU->Uniforms_Texture.begin(); it != ParentMaterial->ProgramGPU->Uniforms_Texture.end(); it++)
		{
			std::shared_ptr<UniformItem_Texture> TexUniform = *it;
			PropertyBase::AddTab(Data);
			std::hash<String> hs;
			int32 TextureID = hs(TexUniform->Name);
			std::shared_ptr<Texture> Tex = TextureUniformID_DataPtrMap.find(TextureID)->second->Tex;
			Data->append(PropertyToString(TexUniform->Name, std::shared_ptr<PropertyData>(new PropertyData(GPU_CPU_TypeMap[TexUniform->DataType], &Tex->GetPath()))));
;		}
		break;
	case MaterialInstance::DataGroup::UNIFORMBUFFER:
		for (std::set<std::shared_ptr<UniformItem_Block>>::iterator it = ParentMaterial->ProgramGPU->Uniforms_Block.begin(); it != ParentMaterial->ProgramGPU->Uniforms_Block.end(); it++)
		{
			std::shared_ptr<UniformItem_Block> Block = *it;
			String BlockName = Block->Name;
			std::map<uint32, UniformItem_WithinBlock> Uniforms;
			for(std::map<uint32, std::shared_ptr<UniformItem_WithinBlock>>::iterator UniformsIt = Block->Uniforms.begin(); UniformsIt != Block->Uniforms.end(); UniformsIt++)
			{
				String UniformName = UniformsIt->second->Name;
				PropertyBase::AddTab(Data);
				String PropertyName = BlockName + "," + UniformName;
				Data->append(PropertyToString(PropertyName, std::shared_ptr<PropertyData>(new PropertyData(GPU_CPU_TypeMap[UniformsIt->second->DataType], (void*) ((Address)Block->DataPtr + UniformsIt->second->Offset_Byte)))));
			}
		}
		break;
	default:
		break;
	}
}

