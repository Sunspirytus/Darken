#include "CommonFunctions.h"
#include <map>



String GetNameFromPath(const String& Path)
{
	int32 Pos = Path.rfind('/');
	String Name = Path.substr(Pos + 1, Path.length() - Pos);
	return Name;
}

String GetNameFromPathExceptSuffix(const String& Path)
{
	int32 Pos = Path.rfind('.');
	String Name = Path.substr(0, Pos);
	return Name;
}

String GetNameExceptSuffix(const String& FullName)
{
	int32 Pos = FullName.find('.');
	String Name = FullName.substr(0, Pos);
	assert(Name.find('.') == -1);
	return Name;
}
