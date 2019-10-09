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

	String FolderPath = "../Assets/";
	String InternalShaderPath;
	String EngineRunningPath;

	std::shared_ptr<MaterialManager> _MaterialManager;
	std::shared_ptr<BufferManager> _GPUBuffers;
	std::shared_ptr<SystemTextureFactory> _GlobalTextures;

	uint32 APP_DeltaTime = 0;
	uint64 FrameCount = 0;

public:
	static DKEngine& GetInstance();
	static void Init();

	void SetProjectWorkingPath(const String& Path);
	void SetEngineRunningPath(const String& Path);
	

	std::shared_ptr<MaterialManager> GetMaterialManager();
	std::shared_ptr<BufferManager> GetGPUBufferManager();
	std::shared_ptr<SystemTextureFactory> GetGlobalTextures();
	uint64 GetFrameCount();

	String GetProjectWorkingPath();
	String GetEngineRunningPath();
	String GeInternalShaderPath();
};

