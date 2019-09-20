#include "FileIO.h"

FileIO::FileIO()
{
}

FileIO::~FileIO()
{
}

void FileIO::SaveFile(const std::string& path, const std::string& name, FileType type, const std::string& content)
{
	std::ofstream OutFile;
	std::string FilePath = path + name;
	switch (type)
	{
	case WorldScene:FilePath += FileTypeSuffixMap.find(type)->second; break;
		break;
	default:
		break;
	}
	OutFile.open(FilePath, std::ostream::out);
	OutFile << content;
	OutFile.close();
}