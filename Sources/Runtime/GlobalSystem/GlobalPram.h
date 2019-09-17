#pragma once
#include "TypeDefine.h"
#include "SceneManager.h"
#include "BufferManager.h"
#include "SystemTextures.h"
#include "SystemTextures.h"

using namespace dk;

//extern std::shared_ptr<SceneManager> _Scene;
extern std::shared_ptr<BufferManager> _GPUBuffers;
extern std::shared_ptr<SystemTextureFactory> GlobalTextures;
//extern const uint32 _ScreenWidth;
//extern const uint32 _ScreenHeight;

//extern std::unordered_map<pvr::Keys, bool> InputKeyPressed;
//extern std::vector<pvr::Keys> InputKeyRecord;
extern uint32 APP_DeltaTime;
extern unsigned long APP_FrameCount;
//extern pvr::IAssetProvider* App;

extern std::string AssetFolderPath;