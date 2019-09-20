#include "MaterialInstance.h"
#include "BufferManager.h"

extern std::shared_ptr<BufferManager> _GPUBuffers;

MaterialInstance::MaterialInstance(std::shared_ptr<Material> parentMaterial)
{
	SetParent(parentMaterial);
}

MaterialInstance::~MaterialInstance()
{
	for (std::map<int32, void*>::iterator it = BasicUniformID_PtrMap.begin(); it != BasicUniformID_PtrMap.end(); it++)
	{
		free(it->second);
	}

	for (std::map<int32, uint32*>::iterator it = TextureUniformID_PtrMap.begin(); it != TextureUniformID_PtrMap.end(); it++)
	{
		delete it->second;
	}
	
	for (std::unordered_map<std::string, std::shared_ptr<UniformItem_Block>>::iterator ItBlock = ParentMaterial->MaterialProgram->Uniforms_Block.begin(); ItBlock != ParentMaterial->MaterialProgram->Uniforms_Block.end(); ItBlock++)
	{
		free(ItBlock->second->DataPtr);
	}
}

void MaterialInstance::SetParent(std::shared_ptr<Material> parentMaterial)
{
	ParentMaterial = parentMaterial;

	void* BlockData;
	for(std::unordered_map<std::string, std::shared_ptr<UniformItem_Block>>::iterator ItBlock = ParentMaterial->MaterialProgram->Uniforms_Block.begin(); ItBlock != ParentMaterial->MaterialProgram->Uniforms_Block.end(); ItBlock++)
	{
		BlockData = malloc(ItBlock->second->DataSize_Byte);
		ItBlock->second->DataPtr = BlockData;
		int32 BlockID = (int32) hs(ItBlock->first);
		std::map<int32, void*> UniformID_PtrMap;
		for(std::map<uint32, UniformItem_WithinBlock>::iterator ItUniform = ItBlock->second->Uniforms.begin(); ItUniform != ItBlock->second->Uniforms.end(); ItUniform++)
		{
			int32 UniformID = (int32) hs(ItUniform->second.Name);
			void* UniformPtr = (void*) ((Address)BlockData + ItUniform->second.Offset_Byte);
			UniformID_PtrMap.insert(std::pair<int32, void*>(UniformID, UniformPtr));
		}
		BlockID_UniformID_DataPtrMap.insert(std::pair<int32, std::map<int32, void*>>(BlockID, UniformID_PtrMap));
	}

	void * Data;
	for (std::unordered_map<std::string, UniformItem_Basic>::iterator it = ParentMaterial->MaterialProgram->Uniforms_Basic.begin(); it != ParentMaterial->MaterialProgram->Uniforms_Basic.end(); it++)
	{
		int32 ID = (int32) hs(it->first);
		switch (it->second.DataType)
		{
		case GLSL_INT: Data = malloc(sizeof(int32) * it->second.Size); break;
		case GLSL_FLOAT: Data = malloc(sizeof(float32) * it->second.Size); break;
		case GLSL_VEC2: Data = malloc(sizeof(Vector2f) * it->second.Size);	break;
		case GLSL_VEC3:	Data = malloc(sizeof(Vector3f) * it->second.Size);	break;
		case GLSL_VEC4:	Data = malloc(sizeof(Vector4f) * it->second.Size);	break;
		case GLSL_IVEC2: Data = malloc(sizeof(Vector2i) * it->second.Size); break;
		case GLSL_IVEC3: Data = malloc(sizeof(Vector3i) * it->second.Size); break;
		case GLSL_IVEC4: Data = malloc(sizeof(Vector4i) * it->second.Size); break;
		case GLSL_MAT3:	Data = malloc(sizeof(Mat3f) * it->second.Size);	break;
		case GLSL_MAT4: Data = malloc(sizeof(Mat4f) * it->second.Size); break;
		default:
			Data = nullptr;
			break;
		}
		it->second.DataPtr = (void*)Data;
		BasicUniformID_PtrMap.insert(std::pair<int32, void*>(ID, it->second.DataPtr));
	}
	for (std::unordered_map<std::string, UniformItem_Texture>::iterator it = ParentMaterial->MaterialProgram->Uniforms_Texture.begin(); it != ParentMaterial->MaterialProgram->Uniforms_Texture.end(); it++)
	{
		int32 ID = (int32)hs(it->first);
		uint32 * textureID = new uint32;
		it->second.IDPtr = textureID;
		TextureUniformID_PtrMap.insert(std::pair<int32, uint32 *>(ID, it->second.IDPtr));
	}
}

void MaterialInstance::ChangedParentDynamic(std::shared_ptr<Material> newParentMaterial)
{	 
	for(std::unordered_map<std::string, std::shared_ptr<UniformItem_Block>>::iterator it = newParentMaterial->MaterialProgram->Uniforms_Block.begin(); it != newParentMaterial->MaterialProgram->Uniforms_Block.end(); it++)
	{
		std::unordered_map<std::string, std::shared_ptr<UniformItem_Block>>::iterator it2 = ParentMaterial->MaterialProgram->Uniforms_Block.find(it->first);
		if(it2 == ParentMaterial->MaterialProgram->Uniforms_Block.end()) std::cout << "No " << it->first << " in new Shader." << std::endl;
		it->second->DataPtr = it2->second->DataPtr;
	}


	for (std::unordered_map<std::string, UniformItem_Basic>::iterator it = newParentMaterial->MaterialProgram->Uniforms_Basic.begin(); it != newParentMaterial->MaterialProgram->Uniforms_Basic.end(); it++)
	{
		std::unordered_map<std::string, UniformItem_Basic>::iterator it2 = ParentMaterial->MaterialProgram->Uniforms_Basic.find(it->first);
		if (it2 == ParentMaterial->MaterialProgram->Uniforms_Basic.end()) std::cout << "No " << it->first << " in new Shader." << std::endl;
		it->second.DataPtr = it2->second.DataPtr;
	}

	for (std::unordered_map<std::string,UniformItem_Texture>::iterator it = newParentMaterial->MaterialProgram->Uniforms_Texture.begin(); it != newParentMaterial->MaterialProgram->Uniforms_Texture.end(); it++)
	{
		std::unordered_map<std::string, UniformItem_Texture>::iterator it2 = ParentMaterial->MaterialProgram->Uniforms_Texture.find(it->first);
		if (it2 == ParentMaterial->MaterialProgram->Uniforms_Texture.end()) std::cout << "No " << it->first << " in new Shader." << std::endl;
		it->second.IDPtr = it2->second.IDPtr;
	}
	ParentMaterial = newParentMaterial;
}

std::shared_ptr<Material> MaterialInstance::GetParent()
{
	return ParentMaterial;
}

int32 MaterialInstance::GetID(const std::string& ParameterName)
{
	std::hash<std::string> hs;
	return (int32) hs(ParameterName);
}

void MaterialInstance::MarkDirty(int32 BlockID)
{
	_GPUBuffers->MarkBufferDirty(BlockID);
}

