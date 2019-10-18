#include "BufferManager.h"
#include <iostream>

BufferManager::BufferManager()
{
	std::hash<String>hs;
	MODEL_UNIFORM_BLOCK_NAME_ID = (int32) hs(MODEL_UNIFORM_BLOCK_NAME);
	VIEW_UNIFORM_BLOCK_NAME_ID = (int32) hs(VIEW_UNIFORM_BLOCK_NAME);
	SHADOWMAPPING_UNIFORM_BLOCK_NAME_ID = (int32)hs(SHADOWMAPPING_UNIFORM_BLOCK_NAME);
	LIGHT_UNIFORM_BLOCK_NAME_ID = (int32)hs(LIGHT_UNIFORM_BLOCK_NAME);
}

BufferManager::~BufferManager()
{
}

int32 BufferManager::GetUniformBlockBindingPoint(const String & BlockName)
{
	/*if (BlockName == VIEW_UNIFORM_BLOCK_NAME) return VIEW_UNIFORM_BLOCK_BINDING_POINT;
	else if (BlockName == LIGHT_UNIFORM_BLOCK_NAME) return LIGHT_UNIFORM_BLOCK_BINDING_POINT;
	else if (BlockName == MODEL_UNIFORM_BLOCK_NAME) return MODEL_UNIFORM_BLOCK_BINDING_POINT;
	else if (BlockName == SHADOWMAPPING_UNIFORM_BLOCK_NAME) return SHADOWMAPPING_UNIFORM_BLOCK_BIDING_POINT;
	else return UNIFORM_BLOCK_BINGDING_POINT_COMMEN_BEGIN + (int32)UniformBufferNameID_InfoPtrMap.size();*/
	return 0;
}

int32 BufferManager::CreateUniformBuffer(std::shared_ptr<UniformItem_Block> UniformBlockInfo)
{
	/*if(UniformBlockInfo->Name == VIEW_UNIFORM_BLOCK_NAME && UniformBufferName_GPUIDMap.find(VIEW_UNIFORM_BLOCK_NAME) != UniformBufferName_GPUIDMap.end())
	{
		return UniformBufferName_GPUIDMap.find(VIEW_UNIFORM_BLOCK_NAME)->second;
	}else if(UniformBlockInfo->Name == LIGHT_UNIFORM_BLOCK_NAME && UniformBufferName_GPUIDMap.find(LIGHT_UNIFORM_BLOCK_NAME) != UniformBufferName_GPUIDMap.end())
	{
		return UniformBufferName_GPUIDMap.find(LIGHT_UNIFORM_BLOCK_NAME)->second;
	}
	else if (UniformBlockInfo->Name == MODEL_UNIFORM_BLOCK_NAME && UniformBufferName_GPUIDMap.find(MODEL_UNIFORM_BLOCK_NAME) != UniformBufferName_GPUIDMap.end())
	{
		return UniformBufferName_GPUIDMap.find(MODEL_UNIFORM_BLOCK_NAME)->second;
	}
	else if (UniformBlockInfo->Name == SHADOWMAPPING_UNIFORM_BLOCK_NAME && UniformBufferName_GPUIDMap.find(SHADOWMAPPING_UNIFORM_BLOCK_NAME) != UniformBufferName_GPUIDMap.end())
	{
		return UniformBufferName_GPUIDMap.find(SHADOWMAPPING_UNIFORM_BLOCK_NAME)->second;
	}

	if(UniformBufferName_GPUIDMap.find(UniformBlockInfo->Name) != UniformBufferName_GPUIDMap.end())
	{
		std::cout << "Warning: Several shader all have Uniform Buffer: " << UniformBlockInfo->Name << ". It will influence efficiency." << std::endl;
	}
	   
	uint32 UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, UniformBlockInfo->DataSize_Byte, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	UniformBufferName_GPUIDMap.insert(std::pair<String, int32>(UniformBlockInfo->Name, UBO));
	std::hash<String> hs;
	int32 BufferNameID = (int32)hs(UniformBlockInfo->Name);
	UniformBufferNameID_InfoPtrMap.insert(std::pair<int32, std::shared_ptr<UniformItem_Block>>(BufferNameID, UniformBlockInfo));
	UniformBufferNameID_DirtyMap.insert(std::pair<int32, bool>(BufferNameID, false));

	return UBO;*/
	return 0;
}

void BufferManager::MarkBufferDirty(std::shared_ptr<UniformItem_Block> UniformBlockInfo)
{
	//UniformBufferPtr_DirtyMap.find(UniformBlockInfo)->second = false;	
}

void BufferManager::UpdateCustomBufferData()
{
	for (std::map<std::shared_ptr<UniformItem_Block>, Bool>::iterator it = UniformBufferPtr_DirtyMap.begin(); it != UniformBufferPtr_DirtyMap.end(); it++)
	{
		if (it->second == true)
		{
			it->second = false;

			std::shared_ptr<UniformItem_Block> Block = it->first;
			glBindBuffer(GL_UNIFORM_BUFFER, Block->Id);
			glBufferData(GL_UNIFORM_BUFFER, Block->DataSize_Byte, Block->DataPtr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	}
}

void BufferManager::UpdateModelBuffer(const Mat4f &ModelMatrix,
	const Mat4f &Model_ITMatrix,
	const Mat4f &Model_PreMatrix,
	const Mat4f &Model_ITPreMatrix)
{
	/*Address BlockDataPtr = (Address)UniformBufferNameID_InfoPtrMap.find(MODEL_UNIFORM_BLOCK_NAME_ID)->second->DataPtr;
	std::map<uint32, std::shared_ptr<UniformItem_WithinBlock>>::iterator it = UniformBufferNameID_InfoPtrMap.find(MODEL_UNIFORM_BLOCK_NAME_ID)->second->Uniforms.begin();

	memcpy((void*)(BlockDataPtr + it->first), &ModelMatrix, sizeof(Mat4f));				it++;
	memcpy((void*)(BlockDataPtr + it->first), &Model_ITMatrix, sizeof(Mat4f));			it++;
	memcpy((void*)(BlockDataPtr + it->first), &Model_PreMatrix, sizeof(Mat4f));			it++;
	memcpy((void*)(BlockDataPtr + it->first), &Model_ITPreMatrix, sizeof(Mat4f));		it++;

	UniformBufferNameID_DirtyMap.find(MODEL_UNIFORM_BLOCK_NAME_ID)->second = false;
	UniformItem_Block* Block = UniformBufferNameID_InfoPtrMap.find(MODEL_UNIFORM_BLOCK_NAME_ID)->second.get();
	glBindBuffer(GL_UNIFORM_BUFFER, Block->Id);
	glBufferData(GL_UNIFORM_BUFFER, Block->DataSize_Byte, Block->DataPtr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);*/
}

void BufferManager::UpdateViewBuffer(Camera * camera)
{
	/*if (UniformBufferNameID_InfoPtrMap.empty()) return;
	Mat4f ViewMatrix = camera->GetViewMatrix();
	Mat4f ProjectMatrix = camera->GetProjectMatrix();
	Mat4f ViewMatrix_PreFrame = camera->GetViewMatrix_PreFrame();
	Mat4f ProjectMatrix_PreFrame = camera->GetProjectMatrix_PreFrame();
	Vector3f ViewPosition = camera->GetPosition();
	Vector2f ScreenSize = Vector2f(camera->GetViewPortSize());

	Address BlockDataPtr = (Address) UniformBufferNameID_InfoPtrMap.find(VIEW_UNIFORM_BLOCK_NAME_ID)->second->DataPtr;
	std::map<uint32, std::shared_ptr<UniformItem_WithinBlock>>::iterator it = UniformBufferNameID_InfoPtrMap.find(VIEW_UNIFORM_BLOCK_NAME_ID)->second->Uniforms.begin();

	memcpy((void*)(BlockDataPtr + it->first), &ViewMatrix, sizeof(Mat4f));				it++;
	memcpy((void*)(BlockDataPtr + it->first), &ProjectMatrix, sizeof(Mat4f));			it++;
	memcpy((void*)(BlockDataPtr + it->first), &ViewMatrix_PreFrame, sizeof(Mat4f));		it++;
	memcpy((void*)(BlockDataPtr + it->first), &ProjectMatrix_PreFrame, sizeof(Mat4f));	it++;
	memcpy((void*)(BlockDataPtr + it->first), &ViewPosition, sizeof(Vector3f));			it++;
	memcpy((void*)(BlockDataPtr + it->first), &ScreenSize, sizeof(Vector2f));				it++;

	UniformBufferNameID_DirtyMap.find(VIEW_UNIFORM_BLOCK_NAME_ID)->second = false;
	UniformItem_Block* Block = UniformBufferNameID_InfoPtrMap.find(VIEW_UNIFORM_BLOCK_NAME_ID)->second.get();
	glBindBuffer(GL_UNIFORM_BUFFER, Block->Id);
	glBufferData(GL_UNIFORM_BUFFER, Block->DataSize_Byte, Block->DataPtr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);*/
}

void BufferManager::UpdateShadowBuffer(ShadowData &shadowBuffer)
{
	/*Address BlockDataPtr = (Address)UniformBufferNameID_InfoPtrMap.find(SHADOWMAPPING_UNIFORM_BLOCK_NAME_ID)->second->DataPtr;
	std::map<uint32, std::shared_ptr<UniformItem_WithinBlock>>::iterator it = UniformBufferNameID_InfoPtrMap.find(SHADOWMAPPING_UNIFORM_BLOCK_NAME_ID)->second->Uniforms.begin();

	memcpy((void*)(BlockDataPtr + it->first), &shadowBuffer.LightSpaceVPMatrix, sizeof(Mat4f));					it++;
	memcpy((void*)(BlockDataPtr + it->first), &shadowBuffer.ShadowBufferSize, sizeof(Vector4f));					it++;
	memcpy((void*)(BlockDataPtr + it->first), &shadowBuffer.SoftTransitionScale, sizeof(float32));						it++;
	memcpy((void*)(BlockDataPtr + it->first), &shadowBuffer.ProjectionDepthBiasParameters, sizeof(Vector2f));		it++;
	memcpy((void*)(BlockDataPtr + it->first), &shadowBuffer.LightCamera_ZBufferParams, sizeof(Vector4f));			it++;
	memcpy((void*)(BlockDataPtr + it->first), &shadowBuffer.bDirectLight, sizeof(int32));								it++;

	UniformBufferNameID_DirtyMap.find(SHADOWMAPPING_UNIFORM_BLOCK_NAME_ID)->second = false;
	UniformItem_Block* Block = UniformBufferNameID_InfoPtrMap.find(SHADOWMAPPING_UNIFORM_BLOCK_NAME_ID)->second.get();
	glBindBuffer(GL_UNIFORM_BUFFER, Block->Id);
	glBufferData(GL_UNIFORM_BUFFER, Block->DataSize_Byte, Block->DataPtr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);*/
}

void BufferManager::UpdateLightBuffer(LightData &lightBuffer)
{
	/*Address BlockDataPtr = (Address)UniformBufferNameID_InfoPtrMap.find(LIGHT_UNIFORM_BLOCK_NAME_ID)->second->DataPtr;
	std::map<uint32, std::shared_ptr<UniformItem_WithinBlock>>::iterator it = UniformBufferNameID_InfoPtrMap.find(LIGHT_UNIFORM_BLOCK_NAME_ID)->second->Uniforms.begin();

	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightPosition, sizeof(Vector3f));				it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightInvRadius, sizeof(float32));					it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightColor, sizeof(Vector3f));					it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightFallofExponent, sizeof(float32));				it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.NormalizedLightDirection, sizeof(Vector3f));	it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.NormalizedLightTangent, sizeof(Vector3f));		it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightSpotAngles, sizeof(Vector2f));				it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightSourceRadius, sizeof(float32));				it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightSourceLength, sizeof(float32));				it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightSoftSourceRadius, sizeof(float32));			it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightSpecularScale, sizeof(float32));				it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightContactShadowLength, sizeof(float32));		it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightDistanceFadeMAD, sizeof(Vector2f));		it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightShadowMapChannelMask, sizeof(Vector4f));	it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightShadowedBits, sizeof(int32));					it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.LightType, sizeof(int32));							it++;
	memcpy((void*)(BlockDataPtr + it->first), &lightBuffer.bLastLight, sizeof(int32));						it++;

	UniformBufferNameID_DirtyMap.find(LIGHT_UNIFORM_BLOCK_NAME_ID)->second = false;
	UniformItem_Block* Block = UniformBufferNameID_InfoPtrMap.find(LIGHT_UNIFORM_BLOCK_NAME_ID)->second.get();
	glBindBuffer(GL_UNIFORM_BUFFER, Block->Id);
	glBufferData(GL_UNIFORM_BUFFER, Block->DataSize_Byte, Block->DataPtr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);*/
}