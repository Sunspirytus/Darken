#include "FileIO.h"
#include <string>

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

void FileIO::LoadFile(const String& path, String* outData)
{
	std::ifstream InFile;
	InFile.open(path, std::istream::in);

	String Line;
	while (!InFile.eof())
	{
		std::getline(InFile, Line);
		outData->append(Line + "\n");
	}
	InFile.close();
}