#include "EngineRoot.h"

DKEngine::DKEngine()
{
}

DKEngine::~DKEngine()
{
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
	return APP_FrameCount;
}

String DKEngine::GetAssetFolderPath()
{
	return AssetFolderPath;
}