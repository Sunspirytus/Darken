#include "FileIO.h"

FileIO::FileIO()
{
}

FileIO::~FileIO()
{
}

void FileIO::SaveFile(const String& path, const String& name, FileType type, const String& content)
{
	std::ofstream OutFile;
	String FilePath = path + name;
	FilePath += FileTypeSuffixMap.find(type)->second;
	OutFile.open(FilePath, std::ostream::out);
	OutFile << content;
	OutFile.close();
}