#pragma once
#include "MaterialInstance.h"
#include "Camera.h"
#include <map>

#define MODEL_UNIFORM_BLOCK_NAME "Model"
#define MODEL_UNIFORM_BLOCK_BINDING_POINT 0

#define VIEW_UNIFORM_BLOCK_NAME "View"
#define VIEW_UNIFORM_BLOCK_BINDING_POINT (int32)1

#define SHADOWMAPPING_UNIFORM_BLOCK_NAME "Shadow"
#define SHADOWMAPPING_UNIFORM_BLOCK_BIDING_POINT (int32)2

#define LIGHT_UNIFORM_BLOCK_NAME "LightBuffer"
#define LIGHT_UNIFORM_BLOCK_BINDING_POINT (int32)3

#define UNIFORM_BLOCK_BINGDING_POINT_COMMEN_BEGIN (int32)4

struct ShadowData
{
	Mat4f LightSpaceVPMatrix;
	Vector4f ShadowBufferSize;
	float32 SoftTransitionScale;
	Vector2f ProjectionDepthBiasParameters;
	Vector4f LightCamera_ZBufferParams;
	int32 bDirectLight;
};

struct LightData
{
	Vector3f LightPosition;
	float32 LightInvRadius;
	Vector3f LightColor;
	float32 LightFallofExponent;
	Vector3f NormalizedLightDirection;
	Vector3f NormalizedLightTangent;
	Vector2f LightSpotAngles;
	float32 LightSourceRadius;
	float32 LightSourceLength;
	float32 LightSoftSourceRadius;
	float32 LightSpecularScale;
	float32 LightContactShadowLength;
	Vector2f LightDistanceFadeMAD;
	Vector4f LightShadowMapChannelMask;
	int32 LightShadowedBits;
	int32 LightType;
	int32 bLastLight;
};

class BufferManager
{
public:
	BufferManager();
	~BufferManager();
	
	int32 GetUniformBlockBindingPoint(const String & BlockName);
	int32 CreateUniformBuffer(const String& BufferName, std::shared_ptr<UniformItem_Block> UniformBlockInfo);
	void MarkBufferDirty(int32 BufferNameID);
	void UpdateViewBuffer(Camera * camera); //View uniform buffer need update before Render main view port
	void UpdateModelBuffer(const Mat4f &ModelMatrix,
							const Mat4f &Model_ITMatrix,
							const Mat4f &Model_PreMatrix,
							const Mat4f &Model_ITPreMatrix); //Model uniform buffer need update before per draw
	void UpdateShadowBuffer(ShadowData &shadowBuffer);  //Shadow uniform buffer need update before draw next light

	void UpdateLightBuffer(LightData &lightBuffer); //Light uniform buffer need update before draw next light
							
	
	void UpdateCustomBufferData();

private:
	std::multimap<int32, std::shared_ptr<UniformItem_Block>> UniformBufferNameID_InfoPtrMap;
	std::multimap<String, int32> UniformBufferName_GPUIDMap;
	std::map<int32, bool> UniformBufferNameID_DirtyMap;

	int32 MODEL_UNIFORM_BLOCK_NAME_ID;
	int32 VIEW_UNIFORM_BLOCK_NAME_ID;
	int32 SHADOWMAPPING_UNIFORM_BLOCK_NAME_ID;
	int32 LIGHT_UNIFORM_BLOCK_NAME_ID;
};

