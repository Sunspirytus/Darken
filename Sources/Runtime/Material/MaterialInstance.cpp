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
	AddProperty("Path", STRING, &Path);
	AddProperty("ParentPath", STRING, &ParentName);
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
	
	for (std::unordered_map<String, std::shared_ptr<UniformItem_Block>>::iterator ItBlock = ParentMaterial->MaterialProgram->Uniforms_Block.begin(); ItBlock != ParentMaterial->MaterialProgram->Uniforms_Block.end(); ItBlock++)
	{
		free(ItBlock->second->DataPtr);
	}
}

void MaterialInstance::SetParent(std::shared_ptr<Material> parentMaterial)
{
	ParentMaterial = parentMaterial;

	void* BlockData;
	for(std::unordered_map<String, std::shared_ptr<UniformItem_Block>>::iterator ItBlock = ParentMaterial->MaterialProgram->Uniforms_Block.begin(); ItBlock != ParentMaterial->MaterialProgram->Uniforms_Block.end(); ItBlock++)
	{
		std::hash<String> hs;
		BlockData = malloc(ItBlock->second->DataSize_Byte);
		ItBlock->second->DataPtr = BlockData;
		int32 BlockID = (int32) hs(ItBlock->first);
		std::map<int32, std::shared_ptr<BlockUniformData>> UniformID_PtrMap;
		for(std::map<uint32, UniformItem_WithinBlock>::iterator ItUniform = ItBlock->second->Uniforms.begin(); ItUniform != ItBlock->second->Uniforms.end(); ItUniform++)
		{
			int32 UniformID = (int32) hs(ItUniform->second.Name);
			std::shared_ptr<BlockUniformData> UniformPtr = std::shared_ptr<BlockUniformData>(new BlockUniformData());
			UniformPtr->Data = (void*)((Address)BlockData + ItUniform->second.Offset_Byte);
			UniformPtr->BlockName = ItBlock->first;
			UniformPtr->UniformName = ItUniform->second.Name;
			UniformID_PtrMap.insert(std::pair<int32, std::shared_ptr<BlockUniformData>>(UniformID, UniformPtr));
		}
		BlockID_UniformID_DataPtrMap.insert(std::pair<int32, std::map<int32, std::shared_ptr<BlockUniformData>>>(BlockID, UniformID_PtrMap));
	}

	for (std::unordered_map<String, UniformItem_Basic>::iterator it = ParentMaterial->MaterialProgram->Uniforms_Basic.begin(); it != ParentMaterial->MaterialProgram->Uniforms_Basic.end(); it++)
	{
		std::hash<String> hs;
		int32 ID = (int32) hs(it->first);
		std::shared_ptr<BasicUniformData> UniformData = std::shared_ptr<BasicUniformData>(new BasicUniformData());
		switch (it->second.DataType)
		{
		case GLSL_INT: UniformData->Data = malloc(sizeof(int32) * it->second.Size); break;
		case GLSL_FLOAT: UniformData->Data = malloc(sizeof(float32) * it->second.Size); break;
		case GLSL_VEC2: UniformData->Data = malloc(sizeof(Vector2f) * it->second.Size);	break;
		case GLSL_VEC3:	UniformData->Data = malloc(sizeof(Vector3f) * it->second.Size);	break;
		case GLSL_VEC4:	UniformData->Data = malloc(sizeof(Vector4f) * it->second.Size);	break;
		case GLSL_IVEC2: UniformData->Data = malloc(sizeof(Vector2i) * it->second.Size); break;
		case GLSL_IVEC3: UniformData->Data = malloc(sizeof(Vector3i) * it->second.Size); break;
		case GLSL_IVEC4: UniformData->Data = malloc(sizeof(Vector4i) * it->second.Size); break;
		case GLSL_MAT3:	UniformData->Data = malloc(sizeof(Mat3f) * it->second.Size);	break;
		case GLSL_MAT4: UniformData->Data = malloc(sizeof(Mat4f) * it->second.Size); break;
		default:
			UniformData->Data = nullptr;
			break;
		}
		UniformData->Name = it->first;
		it->second.DataPtr = UniformData->Data;
		BasicUniformID_DataPtrMap.insert(std::pair<int32, std::shared_ptr<BasicUniformData>>(ID, UniformData));
	}
	for (std::unordered_map<String, UniformItem_Texture>::iterator it = ParentMaterial->MaterialProgram->Uniforms_Texture.begin(); it != ParentMaterial->MaterialProgram->Uniforms_Texture.end(); it++)
	{
		std::hash<String> hs;
		int32 ID = (int32)hs(it->first);
		std::shared_ptr<TextureUniformData> TextureData = std::shared_ptr<TextureUniformData>(new TextureUniformData());
		TextureData->Name = it->first;
		TextureData->IDPtr = new uint32;
		it->second.IDPtr = TextureData->IDPtr;
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

void MaterialInstance::MarkDirty(int32 BlockID)
{
	DKEngine::GetInstance().GetGPUBufferManager()->MarkBufferDirty(BlockID);
}

void MaterialInstance::Save(String* Data)
{
	PropertyBase::PrepareToWrite(Data);

	PropertyBase::BeginWriteProperty(Data, BaseInfo);
	MaterialInstanceBase::Save(Data);
	PropertyBase::EndWriteProperty(Data, BaseInfo);

	PropertyBase::BeginWriteProperty(Data, ComponentInfo);
	WriteInstanceData(Data, UNIFORM);
	PropertyBase::EndWriteProperty(Data, ComponentInfo);

	PropertyBase::FinishWrite(Data);
}

void MaterialInstance::WriteInstanceData(String* Data, DataGroup type)
{
	switch (type)
	{
	case MaterialInstance::UNIFORM:
	{
		for (std::unordered_map<String, UniformItem_Basic>::iterator it = ParentMaterial->MaterialProgram->Uniforms_Basic.begin(); it != ParentMaterial->MaterialProgram->Uniforms_Basic.end(); it++)
		{
			PropertyBase::AddTab(Data);
			Data->append(PropertyToString(it->first, std::shared_ptr<PropertyData>(new PropertyData(GPU_CPU_TypeMap[it->second.DataType], it->second.DataPtr))));
		}
		break;
	}
	case MaterialInstance::TEXTURE:
		break;
	case MaterialInstance::UNIFORMBUFFER:
		break;
	default:
		break;
	}
	




	for(std::map<int32, std::shared_ptr<TextureUniformData>>::iterator it = TextureUniformID_DataPtrMap.begin(); it != TextureUniformID_DataPtrMap.end(); it++)
	{
	
	}


	
	for(std::map<int32, std::map<int32, std::shared_ptr<BlockUniformData>>>::iterator it = BlockID_UniformID_DataPtrMap.begin(); it != BlockID_UniformID_DataPtrMap.end(); it++)
	{
		
	}
}

