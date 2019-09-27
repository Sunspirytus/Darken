#pragma once

#include "TypeDefine.h"
#include "MaterialManager.h"
#include "BufferManager.h"
#include "SystemTextures.h"

class DKEngine
{
private:
	DKEngine();
	~DKEngine();
	DKEngine(const DKEngine&);
	DKEngine& operator=(const DKEngine& engine);

	String AssetFolderPath = "..\\Assets\\";

	std::shared_ptr<MaterialManager> _MaterialManager;
	std::shared_ptr<BufferManager> _GPUBuffers;
	std::shared_ptr<SystemTextureFactory> _GlobalTextures;

	uint32 APP_DeltaTime = 0;
	uint64 APP_FrameCount = 0;




public:
	static DKEngine& GetInstance()
	{
		static DKEngine instance;
		return instance;
	}

	std::shared_ptr<MaterialManager> GetMaterialManager();
	std::shared_ptr<BufferManager> GetGPUBufferManager();
	std::shared_ptr<SystemTextureFactory> GetGlobalTextures();
	uint64 GetFrameCount();
	String GetAssetFolderPath();
};

