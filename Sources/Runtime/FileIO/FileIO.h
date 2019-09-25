#pragma once

#include "TypeDefine.h"
#include <fstream>
#include <map>

enum FileType
{
	Project,
	WorldScene
};

static std::map<FileType, String> FileTypeSuffixMap = 
{
	std::pair<FileType, String>(FileType::Project, ".dkProject"),
	std::pair<FileType, String>(FileType::WorldScene, ".dkScene")
};

class FileIO
{
public:
	FileIO();
	~FileIO();

	void SaveFile(const String& path, const String& name, FileType type, const String& content);

private:

};


