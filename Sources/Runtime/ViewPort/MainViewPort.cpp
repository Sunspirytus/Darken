#include "MainViewPort.h"
#include "DeferRenderPipeline.h"
#include "ReflectionCapture.h"
#include "SystemTextures.h"
#include "EngineRoot.h"

MainViewPort::MainViewPort()
{
}

MainViewPort::~MainViewPort()
{
}

void MainViewPort::InitScene()
{
	std::shared_ptr<BufferManager> _GPUBuffers = DKEngine::GetInstance().GetGPUBufferManager();
	std::shared_ptr<MaterialManager> _MaterialManager = DKEngine::GetInstance().GetMaterialManager();
	std::shared_ptr<SystemTextureFactory> GlobalTextures = DKEngine::GetInstance().GetGlobalTextures();

	std::shared_ptr<Material> SimpleObjectShadowDepthMaterial = _MaterialManager->CreateMaterial(DKEngine::GetInstance().GetAssetFolderPath() + "SimpleObjectShadowDepthMaterial", std::vector<String>{ "ShadowDepthVertShader.vsh", "ShadowDepthFragShader.fsh" }, UserDefine);
	std::shared_ptr<MaterialInstance> SimpleObjectShadowDepthMaterialInst = _MaterialManager->CreateMaterialInstance(DKEngine::GetInstance().GetAssetFolderPath() + "SimpleObjectShadowDepthMaterialInst", SimpleObjectShadowDepthMaterial, UserDefine);

	//std::shared_ptr<Material> SimpleObjectShadowDepthMaterial2 = std::shared_ptr<Material>(new Material(*SimpleObjectShadowDepthMaterial));
	{

		std::shared_ptr<Material> SimpleObjectMaterial00 = _MaterialManager->CreateMaterial(DKEngine::GetInstance().GetAssetFolderPath() + "SimpleObjectMaterial00", std::vector<String> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }, UserDefine);
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst00 = _MaterialManager->CreateMaterialInstance(DKEngine::GetInstance().GetAssetFolderPath() + "SimpleObjectMaterialInst00", SimpleObjectMaterial00, UserDefine);
		SimpleObjectMaterialInst00->SetUniform<Vector3f>("ColorTest", Vector3f(1.0, 0.0, 0.0));
		std::shared_ptr<SimpleObject> Cube00 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst00));
		Cube00->Transform->SetPosition(Vector3f(0.0, 0.0, 0.0));
		Scene->AddObj(ObjectType::StaticMeshActor, Cube00);
	}

	std::shared_ptr<Camera> ViewCamera = std::shared_ptr<Camera>(new Camera(Vector3f(0.0, 10.0, 0.0), Vector3f(0.0, 0.0, -90.0), Math::Radians(60.0), (float32)ViewPortSize.x / (float32)ViewPortSize.y, 0.1f, 100.0f, Vector2i(ViewPortSize.x, ViewPortSize.y)));

	Scene->AddCamera(CameraIndex::MainCamera, ViewCamera);

	std::shared_ptr<LightGroup> Lights = std::shared_ptr<LightGroup>(new LightGroup(Scene));

	RenderPipeline = std::shared_ptr<DeferRenderPipeline>(new DeferRenderPipeline());
	RenderPipeline->Init(Scene);	

	Scene->GetCamera(CameraIndex::MainCamera)->ActiveViewPort();
}

void MainViewPort::TickScene()
{
	Scene->InternalUpdate();
	Scene->Update();
}

void MainViewPort::RenderScene()
{
	Scene->GetCamera(CameraIndex::MainCamera)->ActiveViewPort();
	RenderPipeline->Render(Scene->GetCamera(CameraIndex::MainCamera));
	//glFlush();
}

void MainViewPort::OnSizeChange()
{
	std::shared_ptr<Camera> ViewCamera = Scene->GetCamera(MainCamera);
	ViewCamera->SetAspect((float32)ViewPortSize.x / (float32)ViewPortSize.y);
	ViewCamera->SetViewPortSize(ViewPortSize);
	RenderPipeline->RenderTextureSizeChange(ViewPortSize);
}