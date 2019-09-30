#include "CommonFunctions.h"
#include <map>

std::vector<String> split(const String& str, const String& delim) {
	std::vector<String> res;
	if ("" == str) return res;
	char* strs = new char[str.length() + 1];  
	strcpy_s(strs, strlen(strs), str.c_str());

	char* d = new char[delim.length() + 1];
	strcpy_s(d, strlen(d), delim.c_str());

	char* p = strtok_s(strs, d, nullptr);
	while (p) {
		String s = p; 
		res.push_back(s);
		p = strtok_s(nullptr, d, nullptr);
	}

	return res;
}

String GetNameFromPath(const String& Path)
{
	int32 Pos = Path.rfind('/');
	String Name = Path.substr(Pos + 1, Path.length() - Pos);
	return Name;
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
