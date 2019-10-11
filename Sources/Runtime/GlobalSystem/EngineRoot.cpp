#include "EngineRoot.h"

DKEngine::DKEngine()
{
}

DKEngine::~DKEngine()
{
}

DKEngine& DKEngine::GetInstance()
{
	static DKEngine instance;
	return instance;
}

void DKEngine::Init()
{
	DKEngine& Engine = DKEngine::GetInstance();
	Engine._GPUBuffers = std::shared_ptr<BufferManager>(new BufferManager());
	Engine._MaterialManager = std::shared_ptr<MaterialManager>(new MaterialManager());
	Engine._GlobalTextures = std::shared_ptr<SystemTextureFactory>(new SystemTextureFactory());
	Engine._GlobalTextures->GeneratePreIntegratedGFTexture();
}

std::shared_ptr<MaterialManager> DKEngine::GetMaterialManager()
{
	return _MaterialManager;
}

std::shared_ptr<BufferManager> DKEngine::GetGPUBufferManager()
{
	return _GPUBuffers;
}

std::shared_ptr<SystemTextureFactory> DKEngine::GetGlobalTextures()
{
	return _GlobalTextures;
}

uint64 DKEngine::GetFrameCount()
{
	return FrameCount;
}

void DKEngine::SetAssetFolderPath(const String& Path)
{
	FolderPath = Path;
}

void DKEngine::SetEngineRunningPath(const String& Path)
{
	EngineRunningPath = Path;
	InternalShaderPath = GetSuperPathFromPath(Path) + "/Sources/Shaders/";
}

String DKEngine::GetAssetFolderPath()
{
	return FolderPath;
}

String DKEngine::GetEngineRunningPath()
{
	return EngineRunningPath;
}

String DKEngine::GetInternalShaderPath()
{
	return InternalShaderPath;
}