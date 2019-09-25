#pragma once

#include "TypeDefine.h"
#include <QFileDialog>

class FileDialog : public QFileDialog
{
	Q_OBJECT
public:
	FileDialog(QWidget* parent = Q_NULLPTR);
	~FileDialog();

	String OpenAndGetFileName(const String& Title, const String& Filter);
private:

};