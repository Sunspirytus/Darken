#pragma once

#include "TypeDefine.h"
#include <fstream>
#include <map>

enum class ENGINEAPI FileType
{
	F_Material,
	F_MaterialInstance,
	F_Project,
	F_WorldScene
};

static std::map<FileType, String> FileTypeSuffixMap = 
{
	std::pair<FileType, String>(FileType::F_Material, ".dkmat"),
	std::pair<FileType, String>(FileType::F_MaterialInstance, ".dkmatinst"),
	std::pair<FileType, String>(FileType::F_Project, ".dkproject"),
	std::pair<FileType, String>(FileType::F_WorldScene, ".dkscene")
};

class ENGINEAPI FileIO
{
public:
	FileIO();
	~FileIO();

	void SaveFile(const String& path, const String& name, FileType type, const String& content);
	void SaveFile(const String& path, FileType type, const String& content);
	void LoadFile(const String& path, String* outData);

	FileType Type;
};


