#pragma once

#include <QFileDialog>

class FileDialog : public QFileDialog
{
	Q_OBJECT
public:
	FileDialog(QWidget* parent = Q_NULLPTR);
	~FileDialog();

	//std::string OpenAndGetFileName(const std::string)
private:

};