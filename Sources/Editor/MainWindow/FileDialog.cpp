#include "FileDialog.h"
#include "CommonFunctions.h"

FileDialog::FileDialog(QWidget* parent)
{
	
}

FileDialog::~FileDialog()
{
}

std::vector<String> FileDialog::OpenAndGetFileName(const String& Title, const String& Filter, SelectMode FileCount)
{
	this->setWindowTitle(Title.c_str());
	this->setNameFilter(Filter.c_str());
	std::vector<String> OutFiles;
	
	if(this->exec())
	{
		QStringList SelectedFiles = this->selectedFiles();
		switch (FileCount)
		{
		case FileDialog::Single: OutFiles.push_back(QStringToString(SelectedFiles[0]));
			break;
		case FileDialog::Multi:
		{
			for (int32 Index = 0; Index < SelectedFiles.count(); Index++)
			{
				OutFiles.push_back(QStringToString(SelectedFiles[Index]));
			}
			break;
		}
		default:
			break;
		}
		
	}
	return OutFiles;
}