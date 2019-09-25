#pragma once

#include "TypeDefine.h"
#include <fstream>
#include <map>

enum FileType
{
	Project,
	WorldScene
};

static std::map<FileType, std::string> FileTypeSuffixMap = 
{
	std::pair<FileType, std::string>(FileType::Project, ".dkProject"),
	std::pair<FileType, std::string>(FileType::WorldScene, ".dkScene")
};

class FileIO
{
public:
	FileIO();
	~FileIO();

	void SaveFile(const std::string& path, const std::string& name, FileType type, const std::string& content);

private:

};


