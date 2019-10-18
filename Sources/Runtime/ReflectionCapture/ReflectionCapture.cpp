#include "Util.h"
#include "ReflectionCapture.h"
#include "DeferRenderPipeline.h"


SphereReflectionCapture::SphereReflectionCapture(std::shared_ptr<World> Scene, const Vector3f &position, const float32 &radius, const float32& brightness)
	:	InfluenceRadius(radius)
	,	Brightness(brightness)
	,	CaptureTexSize(512)
{
	Transform->SetPosition(position);
	CreateCaptureResources();

	int32 Count = 0;
	std::vector<std::shared_ptr<Object>> AbstractActors = Scene->GetObjects((int32)ObjectType::AbstractActor);
	for(int32 Index = 0; Index < AbstractActors.size(); Index++)
	{
		if(dynamic_cast<SphereReflectionCapture*>(AbstractActors[Index].get()) != nullptr)
		{
			Count++;
		}
	}
	Create6FacesCameraList();
	Scene->AddCamera((uint32)CameraIndex::ReflectionCaptureCamera + Count, CaptureCamera);
}

SphereReflectionCapture::~SphereReflectionCapture()
{
}

void SphereReflectionCapture::CreateCaptureResources()
{
	glGenTextures(1, &CaptureTexCube);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CaptureTexCube);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA16F, CaptureTexSize, CaptureTexSize, 0, GL_RGBA, GL_HALF_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA16F, CaptureTexSize, CaptureTexSize, 0, GL_RGBA, GL_HALF_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA16F, CaptureTexSize, CaptureTexSize, 0, GL_RGBA, GL_HALF_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA16F, CaptureTexSize, CaptureTexSize, 0, GL_RGBA, GL_HALF_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA16F, CaptureTexSize, CaptureTexSize, 0, GL_RGBA, GL_HALF_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA16F, CaptureTexSize, CaptureTexSize, 0, GL_RGBA, GL_HALF_FLOAT, 0);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_LOD, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LOD, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glGenTextures(1, &CaptureTex2D);
	glBindTexture(GL_TEXTURE_2D, CaptureTex2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, CaptureTexSize, CaptureTexSize, 0, GL_RGBA, GL_HALF_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SphereReflectionCapture::Create6FacesCameraList()
{
	std::shared_ptr<Camera> LCamera0 = std::shared_ptr<Camera>(new Camera(Transform->GetPosition(), Vector3f(-90.0,  0.0,    0.0), Math::Radians(90.0f), 1.0f, 0.1f, InfluenceRadius, Vector2i(CaptureTexSize, CaptureTexSize)));
	std::shared_ptr<Camera> LCamera1 = std::shared_ptr<Camera>(new Camera(Transform->GetPosition(), Vector3f(90.0,  0.0,  180.0), Math::Radians(90.0f), 1.0f, 0.1f, InfluenceRadius, Vector2i(CaptureTexSize, CaptureTexSize)));
	std::shared_ptr<Camera> LCamera2 = std::shared_ptr<Camera>(new Camera(Transform->GetPosition(), Vector3f(180.0,  0.0,   90.0), Math::Radians(90.0f), 1.0f, 0.1f, InfluenceRadius, Vector2i(CaptureTexSize, CaptureTexSize)));
	std::shared_ptr<Camera> LCamera3 = std::shared_ptr<Camera>(new Camera(Transform->GetPosition(), Vector3f(0.0,  0.0,  -90.0), Math::Radians(90.0f), 1.0f, 0.1f, InfluenceRadius, Vector2i(CaptureTexSize, CaptureTexSize)));
	std::shared_ptr<Camera> LCamera4 = std::shared_ptr<Camera>(new Camera(Transform->GetPosition(), Vector3f(0.0,  90.0,  -90.0), Math::Radians(90.0f), 1.0f, 0.1f, InfluenceRadius, Vector2i(CaptureTexSize, CaptureTexSize)));
	std::shared_ptr<Camera> LCamera5 = std::shared_ptr<Camera>(new Camera(Transform->GetPosition(), Vector3f(0.0,  -90.0,  90.0), Math::Radians(90.0f), 1.0f, 0.1f, InfluenceRadius, Vector2i(CaptureTexSize, CaptureTexSize)));

	LCamera0->SetNextCamera(LCamera1);
	LCamera1->SetNextCamera(LCamera2);
	LCamera2->SetNextCamera(LCamera3);
	LCamera3->SetNextCamera(LCamera4);
	LCamera4->SetNextCamera(LCamera5);
	LCamera5->SetNextCamera(LCamera0);

	CaptureCamera = LCamera0;
}

void SphereReflectionCapture::CaptureWithPipeLine(DeferRenderPipeline* Pipeline)
{
	for(int32 FaceIndex = 0; FaceIndex < 6; FaceIndex++)
	{
		DKEngine::GetInstance().GetGPUBufferManager()->UpdateViewBuffer(CaptureCamera.get());
		DKEngine::GetInstance().GetGPUBufferManager()->UpdateCustomBufferData();

		Pipeline->SceneWaitRender->PrepareShadowDepthMaterial();
		Pipeline->RenderShadowDepthPass((int32)ObjectType::StaticMeshActor);

		Pipeline->SceneWaitRender->PrepareLightingMaterial();
		Pipeline->RenderLightingPass(CaptureCamera, (int32)ObjectType::StaticMeshActor);
		Pipeline->RenderSSSPass();

		Pipeline->ExecuteTemporalAA();
		
		Util::CopyTex2D_Once(Pipeline->TAAPass->TAAToScreenFrame_Tex, CaptureTex2D, CaptureTexSize, CaptureTexSize);

		Util::FlipTex2DY_Once(CaptureTex2D, CaptureTexSize, CaptureTexSize, GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT);

		Util::CopyTex2DToCubeFace_Once(CaptureTex2D, CaptureTexCube, GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT, CaptureTexSize, CaptureTexSize, FaceIndex);
				
		CaptureCamera = CaptureCamera->GetNextCamera();
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, CaptureTexCube);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);	
}

void SphereReflectionCapture::CalReflectionCubeTexAvgBrightness()
{
	//Create GPU Resources
	uint32 CalRenderTex;
	glGenTextures(1, &CalRenderTex);
	glBindTexture(GL_TEXTURE_2D, CalRenderTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	uint32 CalFrameBuffer;
	glGenFramebuffers(1, &CalFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, CalFrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CalRenderTex, 0);

	uint32 attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "CalReflectionCubeTexAvgBrightness FrameBufferError" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Create Material
	std::shared_ptr<Material> CalculateMaterial = DKEngine::GetInstance().GetMaterialManager()->CreateMaterial("CalculateReflectionBrightnessMaterial", std::vector<String> { "DrawRectVertShader.vsh", "ComputeAverageBrightnessFragShader.fsh" }, MaterialBelong::Engine);
	std::shared_ptr<MaterialInstance> ComputeMaterialInst = DKEngine::GetInstance().GetMaterialManager()->CreateMaterialInstance("CalculateReflectionBrightnessMaterialInst", CalculateMaterial, MaterialBelong::Engine);

	//Create Quad Geometry
	std::shared_ptr<RectBufferObject> QuadBufferObject = std::shared_ptr<RectBufferObject>(new RectBufferObject());

	//Opengl pc do not support texturelod in frag shader. So we remember and set min/max lod hear.
	int32 MinLod, MaxLod;
	glBindTexture(GL_TEXTURE_CUBE_MAP, CaptureTexCube);
	glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_LOD, &MinLod);
	glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LOD, &MaxLod);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	int32 NumMip = (int32)Math::Log2((float32)CaptureTexSize);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CaptureTexCube);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_LOD, NumMip);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_LOD, NumMip);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//Calculate
	glBindFramebuffer(GL_FRAMEBUFFER, CalFrameBuffer);
	ComputeMaterialInst->SetTextureID("ReflectionTex", CaptureTexCube);
	ComputeMaterialInst->GetParent()->Draw(QuadBufferObject->VAO, QuadBufferObject->NumFaces, QuadBufferObject->IndexType);
	
	//Get Result
	uint8 BrightnessRGBA[4];
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &BrightnessRGBA);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//Decode RGBA8 to float32
	AverageBrightness = ((float32)BrightnessRGBA[0] / 10.0f + (float32)BrightnessRGBA[1] / 100.0f + (float32)BrightnessRGBA[2] / 1000.0f + (float32)BrightnessRGBA[3] / 10000.0f) / 255.0f * 10.0f;

	//Release Buffer
	glDeleteFramebuffers(1, &CalFrameBuffer);
	glDeleteTextures(1, &CalRenderTex);
	
	//Don't forget reset min/max lod
	glBindTexture(GL_TEXTURE_CUBE_MAP, CaptureTexCube);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_LOD, MinLod);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LOD, MaxLod);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

uint32 SphereReflectionCapture::GetReflectionTextureID()
{
	return CaptureTexCube;
}

float32 SphereReflectionCapture::GetInfluenceRaidus()
{
	return InfluenceRadius;
}

float32 SphereReflectionCapture::GetBrightness()
{
	return Brightness;
}

float32 SphereReflectionCapture::GetAverageBrightness()
{
	return AverageBrightness;
}