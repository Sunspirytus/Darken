#include "World.h"
#include "EngineRoot.h"
#include "FileIO.h"
#include "CommonFunctions.h"
#include "StaticMesh.h"

World::World()
{
}

World::~World()
{
}


void World::Save()
{
	String FolderPath = DKEngine::GetInstance().GetAssetFolderPath();
	String SaveData;
	for (std::multimap<uint32, std::shared_ptr<Object>>::iterator ObjectIterator = SceneObjects.begin(); ObjectIterator != SceneObjects.end(); ObjectIterator++)
	{
		ObjectIterator->second->Save(&SaveData);
	}
	FileIO File;
	File.SaveFile(FolderPath, Name, FileType::F_WorldScene, SaveData);
}

void World::Load(const String& Path)
{
	Name = GetNameFromPathExceptSuffix(Path);
	String Data;
	FileIO File;
	File.LoadFile(Path, &Data);
	std::vector<String> ObjectsInfo = SeperateObject(Data);
	for(int Index = 0; Index < ObjectsInfo.size(); Index++)
	{
		const String& Data = ObjectsInfo[Index];
		std::shared_ptr<Object> NewObject;
		ObjectType Type = Object::GetType(Data);
		switch (Type)
		{
		case Default:
			break;
		case StaticMeshActor:
		{
			NewObject = std::shared_ptr<StaticMesh>(new StaticMesh());
			NewObject->Load(Data);
			break; 
		}
		case DynamicMeshActor:
			break;
		case CameraActor:
			break;
		case LandscapeMeshActor:
			break;
		case AbstractActor:
			break;
		case NavigationSystemActor:
			break;
		default:
			break;
		}
		
	}
}

std::vector<String> World::SeperateObject(const String& SceneInfo)
{
	std::vector<String> Objects;
	int32 BeginPos, EndPos = -1;
	String SearchData = SceneInfo;
	while (true)
	{
		BeginPos = SearchData.find("{");
		EndPos = SearchData.find("}");
		if(BeginPos == -1 && EndPos == -1)
		{
			break;
		}
		else
		{
			Objects.push_back(SearchData.substr(BeginPos, EndPos - BeginPos + 1));
			SearchData = SceneInfo.substr(EndPos + 1, SearchData.length() - EndPos - 1);
		}
	}
	return Objects;
}