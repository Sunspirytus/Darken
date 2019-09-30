#pragma once

#include "TypeDefine.h"
#include <fstream>
#include <map>

enum FileType
{
	F_Material,
	F_MaterialInstance,
	F_Project,
	F_WorldScene
};

static std::map<FileType, String> FileTypeSuffixMap = 
{
	std::pair<FileType, String>(FileType::F_Material, ".dkMat"),
	std::pair<FileType, String>(FileType::F_MaterialInstance, ".dkMatInst"),
	std::pair<FileType, String>(FileType::F_Project, ".dkProject"),
	std::pair<FileType, String>(FileType::F_WorldScene, ".dkScene")
};

class FileIO
{
public:
	FileIO();
	~FileIO();

	void SaveFile(const String& path, const String& name, FileType type, const String& content);
	void LoadFile(const String& path, String* outData);

private:

};


