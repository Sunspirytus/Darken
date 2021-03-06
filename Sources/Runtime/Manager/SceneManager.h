#pragma once

#include "Camera.h"
#include "Light.h"
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

class Object;
class StaticMesh;

namespace ModeState{
	enum class EditMode
	{
		CameraRotation,
		CameraTranslation,
		LandscapeSculpt,
		LandscapeFlatten,
		LandscapePaint,
		NumMode
	};

	enum class ViewMode
	{
		LightingMode,
		WireframeMode
	};
}

enum class CameraIndex
{
	MainCamera = 0,
	ShadowDepthCamera = 1,
	ReflectionCaptureCamera = 100
};

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	
	void Start();
	void InternalUpdate();
	void Update();
	void Render(std::shared_ptr<Camera> camera, int32 typeFlags = (int32)ObjectType::Default);
	void AddObj(ObjectType type, std::shared_ptr<Object> obj);
	void RemoveObj(std::shared_ptr<Object> obj);
	void AddCamera(uint32 index, std::shared_ptr<Camera> camera);
	void AddLight(std::shared_ptr<Light> light);

	void PrepareShadowDepthMaterial();
	void PrepareLightingMaterial();

	std::shared_ptr<Camera> GetCamera(uint32 index);
	std::vector<std::shared_ptr<Light>> GetAllLights();
	std::vector<std::shared_ptr<Object>> GetObjects(uint32 typeFlag);

	ModeState::EditMode GetCurrentEditMode();
	ModeState::ViewMode GetCurrentViewMode();
	void SetCurrentEditMode(ModeState::EditMode editMode);
	void SetCurrentViewMode(ModeState::ViewMode viewMode);

	void UpdatePreFrameData();

protected:
	std::vector<std::shared_ptr<Light>> SceneLights;
	std::multimap<uint32, std::shared_ptr<Camera>> SceneCameras;
	std::multimap<int32, std::shared_ptr<Object>> SceneObjects;

	ModeState::EditMode CurrentEditMode;
	ModeState::ViewMode CurrentViewMode;
};
