#pragma once

#include "TypeDefine.h"
#include <QFileDialog>



class FileDialog : public QFileDialog
{
	Q_OBJECT
public:
	enum class SelectMode
	{
		Single,
		Multi
	};

	FileDialog(QWidget* parent = Q_NULLPTR);
	~FileDialog();

	std::vector<String> OpenAndGetFileName(const String& Title, const String& Filter, SelectMode FileCount);
private:

};