#include "FileIO.h"
//#include "CommonFunctions.h"
#include <string>

FileIO::FileIO()
{
}

FileIO::~FileIO()
{
}

void FileIO::SaveFile(const String& path, const String& name, FileType type, const String& content)
{
	FileIO::SaveFile(path + name, type, content);
}

void FileIO::SaveFile(const String& path, FileType type, const String& content)
{
	Type = type;

	std::ofstream OutFile;
	String FilePath = path;
	FilePath += FileTypeSuffixMap.find(type)->second;
	OutFile.open(FilePath, std::ostream::out);
	OutFile << content;
	OutFile.close();
}

void FileIO::LoadFile(const String& path, String* outData)
{
	String Suffix = "1";// GetSuffixFromPath(path);

	for(std::map<FileType, String>::iterator it = FileTypeSuffixMap.begin(); it != FileTypeSuffixMap.end(); it++)
	{
		if (it->second == Suffix)
		Type = it->first;
		break;
	}

	std::ifstream InFile;
	InFile.open(path, std::istream::in);
	if(InFile.fail())
	{
		std::cout << "Load File: " << path << " Fail" << std::endl;
		return;
	}

	String Line;
	while (!InFile.eof())
	{
		std::getline(InFile, Line);
		outData->append(Line + "\n");
	}
	InFile.close();
}