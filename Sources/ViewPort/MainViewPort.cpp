#include "MainViewPort.h"
#include "GlobalPram.h"
#include "DeferRenderPipeline.h"
#include "ReflectionCapture.h"

MainViewPort::MainViewPort()
{
}

MainViewPort::~MainViewPort()
{
}

void MainViewPort::InitScene()
{
	_GPUBuffers = std::shared_ptr<BufferManager>(new BufferManager());
	GlobalTextures = std::shared_ptr<SystemTextureFactory>(new SystemTextureFactory());

	std::shared_ptr<Material> SimpleObjectShadowDepthMaterial = std::shared_ptr<Material>(new Material(std::vector<std::string>{ "ShadowDepthVertShader.vsh", "ShadowDepthFragShader.fsh" }));
	std::shared_ptr<MaterialInstance> SimpleObjectShadowDepthMaterialInst = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectShadowDepthMaterial));

	Float32 Scale = 1.5f;
	{
		std::shared_ptr<Material> SimpleObjectMaterial00 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst00 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial00));
		SimpleObjectMaterialInst00->SetUniform<Vector3f>("ColorTest", Vector3f(0.25, 0.0, 0.0));
		std::shared_ptr<SimpleObject> Cube00 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst00));
		Cube00->ObjectTransform.SetPosition(Vector3f(-7.5, -7.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube00);

		std::shared_ptr<Material> SimpleObjectMaterial01 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst01 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial01));
		SimpleObjectMaterialInst01->SetUniform<Vector3f>("ColorTest", Vector3f(0.5, 0.0, 0.0));
		std::shared_ptr<SimpleObject> Cube01 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst01));
		Cube01->ObjectTransform.SetPosition(Vector3f(-2.5, -7.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube01);

		std::shared_ptr<Material> SimpleObjectMaterial02 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst02 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial02));
		SimpleObjectMaterialInst02->SetUniform<Vector3f>("ColorTest", Vector3f(0.75, 0.0, 0.0));
		std::shared_ptr<SimpleObject> Cube02 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst02));
		Cube02->ObjectTransform.SetPosition(Vector3f(2.5, -7.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube02);

		std::shared_ptr<Material> SimpleObjectMaterial03 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst03 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial03));
		SimpleObjectMaterialInst03->SetUniform<Vector3f>("ColorTest", Vector3f(1.0, 0.0, 0.0));
		std::shared_ptr<SimpleObject> Cube03 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst03));
		Cube03->ObjectTransform.SetPosition(Vector3f(7.5, -7.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube03);
	}

	{
		std::shared_ptr<Material> SimpleObjectMaterial10 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst10 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial10));
		SimpleObjectMaterialInst10->SetUniform<Vector3f>("ColorTest", Vector3f(0.0, 0.25, 0.0));
		std::shared_ptr<SimpleObject> Cube10 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst10));
		Cube10->ObjectTransform.SetPosition(Vector3f(-7.5, -2.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube10);

		std::shared_ptr<Material> SimpleObjectMaterial11 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst11 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial11));
		SimpleObjectMaterialInst11->SetUniform<Vector3f>("ColorTest", Vector3f(0.0, 0.50, 0.0));
		std::shared_ptr<SimpleObject> Cube11 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst11));
		Cube11->ObjectTransform.SetPosition(Vector3f(-2.5, -2.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube11);

		std::shared_ptr<Material> SimpleObjectMaterial12 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst12 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial12));
		SimpleObjectMaterialInst12->SetUniform<Vector3f>("ColorTest", Vector3f(0.0, 0.75, 0.0));
		std::shared_ptr<SimpleObject> Cube12 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst12));
		Cube12->ObjectTransform.SetPosition(Vector3f(2.5, -2.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube12);

		std::shared_ptr<Material> SimpleObjectMaterial13 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst13 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial13));
		SimpleObjectMaterialInst13->SetUniform<Vector3f>("ColorTest", Vector3f(0.0, 1.0, 0.0));
		std::shared_ptr<SimpleObject> Cube13 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst13));
		Cube13->ObjectTransform.SetPosition(Vector3f(7.5, -2.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube13);
	}

	{
		std::shared_ptr<Material> SimpleObjectMaterial20 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst20 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial20));
		SimpleObjectMaterialInst20->SetUniform<Vector3f>("ColorTest", Vector3f(0.0, 0.0, 0.25));
		std::shared_ptr<SimpleObject> Cube20 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst20));
		Cube20->ObjectTransform.SetPosition(Vector3f(-7.5, 2.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube20);

		std::shared_ptr<Material> SimpleObjectMaterial21 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst21 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial21));
		SimpleObjectMaterialInst21->SetUniform<Vector3f>("ColorTest", Vector3f(0.0, 0.0, 0.50));
		std::shared_ptr<SimpleObject> Cube21 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst21));
		Cube21->ObjectTransform.SetPosition(Vector3f(-2.5, 2.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube21);

		std::shared_ptr<Material> SimpleObjectMaterial22 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst22 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial22));
		SimpleObjectMaterialInst22->SetUniform<Vector3f>("ColorTest", Vector3f(0.0, 0.0, 0.75));
		std::shared_ptr<SimpleObject> Cube22 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst22));
		Cube22->ObjectTransform.SetPosition(Vector3f(2.5, 2.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube22);

		std::shared_ptr<Material> SimpleObjectMaterial23 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst23 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial23));
		SimpleObjectMaterialInst23->SetUniform<Vector3f>("ColorTest", Vector3f(0.0, 0.0, 1.0));
		std::shared_ptr<SimpleObject> Cube23 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst23));
		Cube23->ObjectTransform.SetPosition(Vector3f(7.5, 2.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube23);
	}

	{
		std::shared_ptr<Material> SimpleObjectMaterial30 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst30 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial30));
		SimpleObjectMaterialInst30->SetUniform<Vector3f>("ColorTest", Vector3f(0.25, 0.25, 0.0));
		std::shared_ptr<SimpleObject> Cube30 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst30));
		Cube30->ObjectTransform.SetPosition(Vector3f(-7.5, 7.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube30);

		std::shared_ptr<Material> SimpleObjectMaterial31 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst31 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial31));
		SimpleObjectMaterialInst31->SetUniform<Vector3f>("ColorTest", Vector3f(0.50, 0.50, 0.0));
		std::shared_ptr<SimpleObject> Cube31 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst31));
		Cube31->ObjectTransform.SetPosition(Vector3f(-2.5, 7.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube31);

		std::shared_ptr<Material> SimpleObjectMaterial32 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst32 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial32));
		SimpleObjectMaterialInst32->SetUniform<Vector3f>("ColorTest", Vector3f(0.75, 0.75, 0.0));
		std::shared_ptr<SimpleObject> Cube32 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst32));
		Cube32->ObjectTransform.SetPosition(Vector3f(2.5, 7.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube32);

		std::shared_ptr<Material> SimpleObjectMaterial33 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst33 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial33));
		SimpleObjectMaterialInst33->SetUniform<Vector3f>("ColorTest", Vector3f(1.0, 1.0, 0.0));
		std::shared_ptr<SimpleObject> Cube33 = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Cube.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst33));
		Cube33->ObjectTransform.SetPosition(Vector3f(7.5, 7.5, 0.0) * Scale);
		Scene->AddObj(ObjectType::StaticMesh, Cube33);
	}
	{
		std::shared_ptr<Material> SimpleObjectMaterial0 = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
		std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst7 = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial0));
		SimpleObjectMaterialInst7->SetUniform<Vector3f>("ColorTest", Vector3f(1.0, 0.0, 1.0));
		std::shared_ptr<SimpleObject> Plane = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Plane.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst7));
		Plane->ObjectTransform.SetPosition(Vector3f(0.0, 0.0, -1.0));
		Plane->ObjectTransform.SetScale(Vector3f(50.0, 50.0, 50.0));
		Scene->AddObj(ObjectType::StaticMesh, Plane);
	}

	std::shared_ptr<Material> SimpleObjectMaterial = std::shared_ptr<Material>(new Material(std::vector<std::string> { "SimpleVertShader.vsh", "SimpleFragShader.fsh" }));
	std::shared_ptr<MaterialInstance> SimpleObjectMaterialInst = std::shared_ptr<MaterialInstance>(new MaterialInstance(SimpleObjectMaterial));
	SimpleObjectMaterialInst->SetUniform<Vector3f>("ColorTest", Vector3f(1.0, 1.0, 1.0));
	std::shared_ptr<SimpleObject> Sphere = std::shared_ptr<SimpleObject>(new SimpleObject("BasicModel\\Sphere.FBX", SimpleObjectShadowDepthMaterialInst, SimpleObjectMaterialInst));
	Sphere->ObjectTransform.SetPosition(Vector3f(0.0, 0.0, 10.0));
	Sphere->ObjectTransform.SetScale(Vector3f(1.0, 1.0, 1.0));
	Scene->AddObj(ObjectType::DynamicMesh, Sphere);

	std::shared_ptr<SphereReflectionCapture> ReflectionActor = std::shared_ptr<SphereReflectionCapture>(new SphereReflectionCapture(Scene, Vector3f(0.0, 0.0, 5.0), 1000.0, 1.0));
	Scene->AddObj(ObjectType::AbstractActor, ReflectionActor);
	
	std::shared_ptr<Camera> ViewCamera = std::shared_ptr<Camera>(new Camera(Vector3f(0.0, 5.0, 10.0), Vector3f(0.0, 0.0, -90.0), Math::Radians(60.0), (Float32)_ScreenWidth / (Float32)_ScreenHeight, 0.1f, 100.0f, Vector2i(_ScreenWidth, _ScreenHeight)));

	Scene->AddCamera(CameraIndex::MainCamera, ViewCamera);

	std::shared_ptr<LightGroup> Lights = std::shared_ptr<LightGroup>(new LightGroup(Scene));

	RenderPipeline = std::shared_ptr<DeferRenderPipeline>(new DeferRenderPipeline());
	RenderPipeline->Init(Scene);

	std::vector<std::shared_ptr<Object>> AbstractActors = Scene->GetObjects(ObjectType::AbstractActor);
	for (Int32 Index = 0; Index < AbstractActors.size(); Index++)
	{
		SphereReflectionCapture* Capture = dynamic_cast<SphereReflectionCapture*>(AbstractActors[Index].get());
		if (Capture != nullptr)
		{
			Capture->CaptureWithPipeLine(dynamic_cast<DeferRenderPipeline*>(RenderPipeline.get()));
			Capture->CalReflectionCubeTexAvgBrightness();
		}
	}
	GlobalTextures = std::shared_ptr<SystemTextureFactory>(new SystemTextureFactory());
	GlobalTextures->GeneratePreIntegratedGFTexture();
	SimpleObjectMaterialInst->SetTextureID("ReflectionTex", ReflectionActor->GetReflectionTextureID());
	SimpleObjectMaterialInst->SetTextureID("PreIntegratedGF", GlobalTextures->GetPreIntegratedGF_Tex());
	SimpleObjectMaterialInst->SetUniform<Float32>("AverageBrightness", ReflectionActor->GetAverageBrightness());
	SimpleObjectMaterialInst->SetUniform<Float32>("Brightness", ReflectionActor->GetBrightness());
	SimpleObjectMaterialInst->SetUniform<Vector4f>("CapturePositionAndInfluenceRadius", Vector4f(ReflectionActor->ObjectTransform.GetPosition(), ReflectionActor->GetInfluenceRaidus()));
	SimpleObjectMaterialInst->SetUniform<Int32>("bReflect", 1);

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