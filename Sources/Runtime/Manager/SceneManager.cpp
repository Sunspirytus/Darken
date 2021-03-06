#include "SceneManager.h"
#include "StaticMesh.h"
#include "FileIO.h"

SceneManager::SceneManager()
{
	CurrentEditMode = ModeState::EditMode::CameraRotation;
	CurrentViewMode = ModeState::ViewMode::LightingMode;
}

SceneManager::~SceneManager()
{
	
}

void SceneManager::Start()
{
	for (std::multimap<int32, std::shared_ptr<Object>>::iterator ObjectIterator = SceneObjects.begin(); ObjectIterator != SceneObjects.end(); ObjectIterator++)
	{
		ObjectIterator->second->Start();
	}
}

void SceneManager::InternalUpdate()
{
	for (std::multimap<int32, std::shared_ptr<Object>>::iterator ObjectIterator = SceneObjects.begin(); ObjectIterator != SceneObjects.end(); ObjectIterator++)
	{
		ObjectIterator->second->InternalUpdate();;
	}
}
void SceneManager::Update()
{
	for (std::multimap<int32, std::shared_ptr<Object>>::iterator ObjectIterator = SceneObjects.begin(); ObjectIterator != SceneObjects.end(); ObjectIterator++)
	{
		ObjectIterator->second->Update();
	}
}

void SceneManager::Render(std::shared_ptr<Camera> camera, int32 typeFlags)
{
	for (std::multimap<int32, std::shared_ptr<Object>>::iterator ObjectIterator = SceneObjects.begin(); ObjectIterator != SceneObjects.end(); ObjectIterator++)
	{
		if(typeFlags != (int32)ObjectType::Default && !(ObjectIterator->first & typeFlags))
		{
			continue;
		}
		if (ObjectIterator->second->bNeedCheckClip)
		{
			ObjectIterator->second->CheckWhetherNeedClip();
		}
		if (ObjectIterator->second->bNeedClip)
		{
			continue;
		}
		ObjectIterator->second->Draw();
	}
}

void SceneManager::AddObj(ObjectType type, std::shared_ptr<Object> obj)
{
	SceneObjects.insert(std::pair<uint32, std::shared_ptr<Object>>((uint32)type, obj));
}

void SceneManager::RemoveObj(std::shared_ptr<Object> obj)
{
	for (std::multimap<int32, std::shared_ptr<Object>>::iterator it = SceneObjects.begin(); it != SceneObjects.end(); it++)
	{
		if (it->second == obj)
		{
			SceneObjects.erase(it);
			break;
		}
	}
}

void SceneManager::AddCamera(uint32 index, std::shared_ptr<Camera> camera)
{
	SceneCameras.insert(std::pair<uint32, std::shared_ptr<Camera>>(index, camera));
}

void SceneManager::AddLight(std::shared_ptr<Light> light)
{
	SceneLights.push_back(light);
}

std::shared_ptr<Camera> SceneManager::GetCamera(uint32 index)
{
	return SceneCameras.find(index)->second;
}

std::vector<std::shared_ptr<Light>> SceneManager::GetAllLights()
{
	std::vector<std::shared_ptr<Light>> Lights;
	for (std::vector<std::shared_ptr<Light>>::iterator it = SceneLights.begin(); it != SceneLights.end(); it++)
	{
		Lights.push_back(*it);
	}
	return Lights;
}

std::vector<std::shared_ptr<Object>> SceneManager::GetObjects(uint32 typeFlag)
{
	std::vector<std::shared_ptr<Object>> Objs;
	for (std::multimap<int32, std::shared_ptr<Object>>::iterator it = SceneObjects.begin(); it != SceneObjects.end(); it++)
	{
		if (it->first & typeFlag)
		{
			Objs.push_back(it->second);
		}
	}

	return Objs;
}

void SceneManager::SetCurrentEditMode(ModeState::EditMode editMode)
{
	CurrentEditMode = editMode;
}

void SceneManager::SetCurrentViewMode(ModeState::ViewMode viewMode)
{
	CurrentViewMode = viewMode;
}

ModeState::EditMode SceneManager::GetCurrentEditMode()
{
	return CurrentEditMode;
}

ModeState::ViewMode SceneManager::GetCurrentViewMode()
{
	return CurrentViewMode;
}

void SceneManager::UpdatePreFrameData()
{
	for (std::multimap<int32, std::shared_ptr<Object>>::iterator ObjectIterator = SceneObjects.begin(); ObjectIterator != SceneObjects.end(); ObjectIterator++)
	{
		if (ObjectIterator->first != (int32)ObjectType::StaticMeshActor && ObjectIterator->first != (uint32)ObjectType::DynamicMeshActor) continue;
		StaticMesh* M = dynamic_cast<StaticMesh*>(ObjectIterator->second.get());
		M->UpdatePreFrameModelMatrix();
	}
	GetCamera((uint32)CameraIndex::MainCamera)->SetViewMatrix_PreFrame(GetCamera((uint32)CameraIndex::MainCamera)->GetViewMatrix());
	GetCamera((uint32)CameraIndex::MainCamera)->SetProjectMatrix_PreFrame(GetCamera((uint32)CameraIndex::MainCamera)->GetProjectMatrix());
}

void SceneManager::PrepareShadowDepthMaterial()
{
	for(std::multimap<int32, std::shared_ptr<Object>>::iterator ObjectIterator = SceneObjects.begin(); ObjectIterator != SceneObjects.end(); ObjectIterator++)
	{
		if (ObjectIterator->first != (int32)ObjectType::StaticMeshActor && ObjectIterator->first != (uint32)ObjectType::DynamicMeshActor) continue;
		StaticMesh* M = dynamic_cast<StaticMesh*>(ObjectIterator->second.get());
		M->BindShadowDepthMaterial();
	}
}

void SceneManager::PrepareLightingMaterial()
{
	for (std::multimap<int32, std::shared_ptr<Object>>::iterator ObjectIterator = SceneObjects.begin(); ObjectIterator != SceneObjects.end(); ObjectIterator++)
	{
		if (ObjectIterator->first != (int32)ObjectType::StaticMeshActor && ObjectIterator->first != (uint32)ObjectType::DynamicMeshActor) continue;
		StaticMesh* M = dynamic_cast<StaticMesh*>(ObjectIterator->second.get());
		M->BindLightingMaterial();
	}
}
