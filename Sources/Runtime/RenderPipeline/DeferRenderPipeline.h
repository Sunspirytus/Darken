#pragma once

#include "MaterialInstance.h"
#include "Light.h"
#include "Object.h"
#include "RectBufferObject.h"
#include "RenderPipelineBase.h"
#include <vector>

class ShadowDepth;
class Lighting;
class ReflectionEnvironment;
class SubSurfaceShading;
class UE4TemporalAA;
class ToneMapping;

class RenderPassBase
{
public:
	RenderPassBase() 
		: RenderTextureSize(Vector2i(100, 100))
	{
	
	};
	~RenderPassBase() {};

	virtual void RenderTextureSizeChange(Vector2i newSize) = 0;
protected:
	Vector2i RenderTextureSize;
};

class DeferRenderPipeline : public RenderPipelineBase
{
public:
	DeferRenderPipeline();
	~DeferRenderPipeline();
	void RenderShadowDepthPass(uint32 typeFlags);
	void RenderLightingPass(std::shared_ptr<Camera> camera, uint32 typeFlags);
	void RenderSSSPass();
	void ExecuteTemporalAA();
	void ExecuteToneMapping();

	//ShadowMapping
	std::shared_ptr<ShadowDepth> ShadowMappingPass;
	//LightingPass
	std::shared_ptr<Lighting> LightingPass;
	//ReflectionEnvironment
	std::shared_ptr<ReflectionEnvironment> ReflectionPass;
	//SubSurfaceShadingPass
	std::shared_ptr<SubSurfaceShading> SSSPass;
	//TemporalAA
	std::shared_ptr<UE4TemporalAA> TAAPass;
	//ToneMapping
	std::shared_ptr<ToneMapping> ToneMappingPass;

	virtual void Init(std::shared_ptr<SceneManager> Scene) final;
	virtual void Render(std::shared_ptr<Camera> camera) final;
	virtual void RenderTextureSizeChange(Vector2i newSize) final;

private:
	std::shared_ptr<RectBufferObject> PPObj;

	void SortSceneLights();
	std::vector<Light*> Lights;
};

struct ShadowDepthMaterialDataIDs
{
	int32 LightSpaceVPMatrixID;
	int32 LightCamera_ZBufferParamsID;
	int32 bDirectLightID;
	
	ShadowDepthMaterialDataIDs()
	{
		LightSpaceVPMatrixID = MaterialInstance::GetID("LightSpaceVPMatrix");
		LightCamera_ZBufferParamsID = MaterialInstance::GetID("LightCamera_ZBufferParams");
		bDirectLightID = MaterialInstance::GetID("bDirectLight");
	}
};

class ShadowDepth : public RenderPassBase
{
public:
	ShadowDepth(std::shared_ptr<SceneManager> scene, std::vector<Light*> lights);
	~ShadowDepth();
	std::vector<uint32> ShadowDepth_Texs2D;
	std::vector<uint32> ShadowDepth_TexsCube;
	std::vector<Light*> Lights;
	std::shared_ptr<MaterialInstance> ShadowDepthMaterialInst;
	int32 ShadowDepthTexWidth;
	int32 ShadowDepthTexHeight;

	void Render(unsigned typeFlags);
private:
	ShadowDepthMaterialDataIDs MaterialDataIDs;
	std::vector<uint32> ShadowDepthFrameBuffers;
	std::vector<uint32> ShadowDepthRenderBuffers;

	void CreateShadowDepthResources();
	void CalculateLightsVPMatrix();
	void RenderDirectLightDepth(int32 LightIndex, const std::vector<std::shared_ptr<Object>> &Objects);
	void RenderPointLightDepth(int32 LightIndex, const std::vector<std::shared_ptr<Object>> &Objects);
	void RenderSpotLightDepth(int32 LightIndex, const std::vector<std::shared_ptr<Object>> &Objects);

	std::shared_ptr<SceneManager> Scene;
public:
	virtual void RenderTextureSizeChange(Vector2i newSize) final {};
};

struct LightingMaterialDataIDs
{
	int32 ShadowDepth_Tex2DID;
	int32 ShadowDepth_TexCubeID;
	int32 LightSpaceVPMatrixID;
	int32 PointLightSpaceVPMatricesID;
	int32 LightDirWorldID;
	int32 LightColorID;
	int32 LightCamera_ZBufferParamsID;
	int32 bDirectLightID;
	int32 ShadowBufferSizeID;
	int32 ShadowDepthSoftTransitionScaleID;
	int32 ProjectionDepthBiasParametersID;
	int32 bLastLightID;

	LightingMaterialDataIDs()
	{
		ShadowDepth_Tex2DID = MaterialInstance::GetID("ShadowDepth_Tex2D");
		ShadowDepth_TexCubeID = MaterialInstance::GetID("ShadowDepth_TexCube");
		LightSpaceVPMatrixID = MaterialInstance::GetID("LightSpaceVPMatrix");
		PointLightSpaceVPMatricesID = MaterialInstance::GetID("PointLightSpaceVPMatrices");
		LightDirWorldID = MaterialInstance::GetID("LightDir");
		LightColorID = MaterialInstance::GetID("LightColor");
		LightCamera_ZBufferParamsID = MaterialInstance::GetID("LightCamera_ZBufferParams");
		bDirectLightID = MaterialInstance::GetID("bDirectLight");
		ShadowBufferSizeID = MaterialInstance::GetID("ShadowBufferSize");
		ShadowDepthSoftTransitionScaleID = MaterialInstance::GetID("SoftTransitionScale");
		ProjectionDepthBiasParametersID = MaterialInstance::GetID("ProjectionDepthBiasParameters");
		bLastLightID = MaterialInstance::GetID("bLastLight");
	}
};

class Lighting : public RenderPassBase
{
public:
	Lighting(std::shared_ptr<SceneManager> scene, std::vector<Light*> lights);
	~Lighting();

	std::vector<Light*> Lights;
	void Render(std::shared_ptr<Camera> camera, uint32 typeFlags);
	std::shared_ptr<MaterialInstance> LightingPassMaterialInst;
	uint32 ScreenDepthZ_Tex;
	uint32 Lighting_Tex;
	uint32 Velocity_Tex;
	uint32 BaseColor_Tex;
	uint32 CustomData_Tex;
	
	std::shared_ptr<ShadowDepth> ShadowMappingPass;
	std::shared_ptr<ReflectionEnvironment> ReflectEnvPass; //Only active at begining

private:
	uint32 LightingPassFrameBuffer;
	uint32 LightingPass_RBO;
	//void CreateLightingPassMaterial();
	void CreateLightingPassResources();
	
	std::shared_ptr<LightingMaterialDataIDs> MaterialDataIDs;
	std::shared_ptr<SceneManager> Scene;
public:
	virtual void RenderTextureSizeChange(Vector2i newSize) final;
};


#define SSSS_KERNEL0_OFFSET						0
#define SSSS_KERNEL0_SIZE						13
#define SSSS_KERNEL1_OFFSET						(SSSS_KERNEL0_OFFSET + SSSS_KERNEL0_SIZE)
#define SSSS_KERNEL1_SIZE						9
#define SSSS_KERNEL2_OFFSET						(SSSS_KERNEL1_OFFSET + SSSS_KERNEL1_SIZE)
#define SSSS_KERNEL2_SIZE						6
#define SSSS_KERNEL_TOTAL_SIZE					(SSSS_KERNEL0_SIZE + SSSS_KERNEL1_SIZE + SSSS_KERNEL2_SIZE)
#define SSSS_KERNEL_TOTAL_SIZE_WITH_PADDING		32
#define SSSS_TRANSMISSION_PROFILE_SIZE			32
#define	SSSS_MAX_TRANSMISSION_PROFILE_DISTANCE	5.0f // See MaxTransmissionProfileDistance in ComputeTransmissionProfile(), SeparableSSS.cpp
#define	SSSS_MAX_DUAL_SPECULAR_ROUGHNESS		2.0f
#define eye_blend_index  4
#define skin_profile_index  3
#define eye_refractive_index  2

class SubSurfaceShading : public RenderPassBase
{
public:
	SubSurfaceShading(std::shared_ptr<Camera> camera);
	~SubSurfaceShading();

	void ComputeTransmissionProfile(Vector4f* TargetBuffer, uint32 TargetBufferSize, Vector4f SubsurfaceColor, Vector4f FalloffColor, float32 ExtinctionScale);
	void ComputeTransmissionProfile(int32 index, Vector4f* TargetBuffer, uint32 TargetBufferSize);

	Vector4f GetSSSS_DUAL_SPECULAR_Params(int32 index);
	Vector4f GetSSSS_TRANSMISSION_Params(int32 index);

	std::shared_ptr<MaterialInstance> SSSSetupMaterialInst;
	std::shared_ptr<MaterialInstance> SSSScateringMaterialInst;
	std::shared_ptr<MaterialInstance> SSSRecombineMaterialInst;

	uint32 SSSRender_Tex;
	uint32 SSSSetup_TexOut;
	uint32 SSSScatering_TexOut1;
	uint32 SSSScatering_TexOut2;
	uint32 SSSRecombine_TexOut;
	void Render(uint32 VAO, int32 NumFaces, IndexSizeType indexType = IndexSizeType::Index16Bits);

private:
	struct FSubsurfaceProfileStruct
	{
		// defaults from SeparableSSS.h and the demo
		float32 ScatterRadius = 0.8f;
		Vector4f SubsurfaceColor = Vector4f(0.80f, 0.78f, 0.748f, 1.0);
		Vector4f FalloffColor = Vector4f(0.735f, 0.3083f, 0.30129f, 1.0);
		Vector4f BoundaryColorBleed = Vector4f(0.735f, 0.5279f, 0.5245f, 1.0);;
		float32 ExtinctionScale = 1.0f;
		float32 ScatteringDistribution = 0.93f;
		float32 NormalScale = 0.08f;
		float32 IOR = 1.55f;
		float32 Roughness0 = 0.75f;
		float32 Roughness1 = 1.30f;
		float32 LobeMix = 0.85f;
	};
	FSubsurfaceProfileStruct SubsurfaceProfileEntries[5];

	void InitSubsurfaceProfileEntries();
	void InitSSSSProfilekernel(int32 index);

	void ComputeMirroredSSSKernel(int32 index, uint32 startPos, uint32 TargetBufferSize, Vector4f SubsurfaceColor, Vector4f FalloffColor);

	static Vector3f SeparableSSS_Gaussian(float32 variance, float32 r, Vector4f FalloffColor);
	static Vector3f SeparableSSS_Profile(float32 r, Vector4f FalloffColor);

	//std::vector<Vector4f> SSSSMirroredProfilekernel;
	std::unordered_map<int32, std::vector<Vector4f> > SSSSMirroredProfilekernelMap;
	static const int32 QualityIndex = 3;
	static const int32 SUBSURFACE_KERNEL_SIZE = 3;

	void CreateResources();
	void InitSSSSProfilekernelParams(String const & _prefix, int32 index);
	uint32 SSSFrameBuffer;

	std::shared_ptr<Camera> ViewCamera;
public:
	virtual void RenderTextureSizeChange(Vector2i newSize) final;
};

struct TemporalAAPixelUniformData
{
	int32 JitterID;
	int32 PlusWeights_0ID;
	int32 PlusWeights_1ID;
	TemporalAAPixelUniformData()
	{
		JitterID = MaterialInstance::GetID("JitterUV"); 
		PlusWeights_0ID = MaterialInstance::GetID("PlusWeights_0");
		PlusWeights_1ID = MaterialInstance::GetID("PlusWeights_1");
	}
};

class UE4TemporalAA : public RenderPassBase
{
public:
	UE4TemporalAA(std::shared_ptr<Camera> camera);
	~UE4TemporalAA();
	void UpdateJitter();
	void RemoveJitter();

	std::shared_ptr<MaterialInstance> TAAPassMaterialInst;

	uint32 TAAToScreenFrame_Tex;
	uint32 TAAHistoryFrame_Tex;
	uint32 TAAFrameBuffer;

	void Execute(uint32 VAO, int32 NumFaces, IndexSizeType indexType = IndexSizeType::Index16Bits);
private:
	std::shared_ptr<TemporalAAPixelUniformData> TAAPixelUniformData;

	std::vector<Vector2f> FrustumJitter;
	int32 ActiveJitterIndex;
	Vector4f ActiveJitterSample;
	Mat4f JitterProjectMatrix;
	Mat4f JitterProjectMatrix_PreFrame;
	void InitUE4SampleType2();
	void InitUE4SampleType4();

	void HackUpdateCameraProjectMatrix(float32 sampleOffsetX, float32 sampleOffsetY);
	void HackRemoveCameraProjectMatrix(float32 sampleOffsetX, float32 sampleOffsetY);
	void UpdateCameraProjectMatrix(float32 sampleOffsetX, float32 sampleOffsetY);

private:
	void CreateTAAPassMaterial();
	void CreateTAAPassResources();
	std::shared_ptr<Camera> ViewCamera;
public:
	virtual void RenderTextureSizeChange(Vector2i newSize) final;
};

struct ToneMappingPixelShaderUniformData
{
	int32 GrainRandomFullID;
	ToneMappingPixelShaderUniformData()
	{
		GrainRandomFullID = MaterialInstance::GetID("GrainRandomFull");
	}
};

class ToneMapping : public RenderPassBase
{
public:
	ToneMapping();
	~ToneMapping();

	std::shared_ptr<MaterialInstance> ToneMappingMaterialInst;
	void Execute(uint32 VAO, int32 NumFaces, IndexSizeType indexType = IndexSizeType::Index16Bits);

	uint32 LUT_TEX;
	void GenerateLUTTexture(std::shared_ptr<RectBufferObject> pPObj);

private:
	void CreateToneMappingPassMaterial();
	std::shared_ptr<ToneMappingPixelShaderUniformData> ToneMappingUniformDatas;
	Vector2f GrainRandomFromFrame(int32 FrameCountMode8);
public:
	virtual void RenderTextureSizeChange(Vector2i newSize) final;
};




