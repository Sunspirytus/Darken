#include "CommonFunctions.h"
#include <map>

std::vector<String> split(const String& str, const String& delim) {
	std::vector<String> res;
	if ("" == str) return res;
	int8* strs = new int8[str.length() + 1];
	strcpy_s(strs, str.length() + 1, str.c_str());

	int8* d = new int8[delim.length() + 1];
	strcpy_s(d, delim.length() + 1, delim.c_str());

	char* p = strtok_s(strs, d, &strs);
	while (p) {
		String s = p; 
		res.push_back(s);
		p = strtok_s(nullptr, d, &strs);
	}

	strs = nullptr;
	delete [] d;
	d = nullptr;

	return res;
}

String GetNameFromPath(const String& Path)
{
	int32 Pos = Path.rfind('/');
	String Name = Path.substr((int)(Pos + 1), Path.length() - Pos);
	return Name;
}

String GetSuperPathFromPath(const String& Path)
{
	int32 Pos = Path.rfind('/');
	String SuperPath = Path.substr(0, Pos);
	return SuperPath;
}

String GetNameFromPathExceptSuffix(const String& Path)
{
	String Name = GetNameFromPath(Path);
	int32 Pos = Name.rfind('.');
	String OutName = Name.substr(0, Pos);
	return OutName;
}

String GetNameExceptSuffix(const String& FullName)
{
	int32 Pos = FullName.find('.');
	String Name = FullName.substr(0, Pos);
	assert(Name.find('.') == -1);
	return Name;
}

String GetSuffixFromPath(const String& Path)
{
	int32 Pos = Path.rfind('.');
	return Path.substr(Pos, Path.length() - Pos);
}

void DeleteStringSpaces(String* Path)
{
	while (true)
	{
		int32 Pos = Path->find(" ");
		if (Pos == -1) return;;
		Path->erase(Pos, 1);
	}
}

