#pragma once
#include "SceneManager.h"

class World : public SceneManager
{
public:
	World();
	~World();

	void Save();
	void Load(const String& Path);

private:
	std::vector<String> SeperateObject(const String& SceneInfo);

	String Name;
};

