#pragma once
#include <QtWidgets/QApplication>
#include "MainWindow.h"

class dkApplication : public QApplication
{
	Q_OBJECT
public:
	dkApplication(int& argc, char** argv);
	~dkApplication();
	
private:
	MW_MainWindow* MainWindow;
	QString ApplicaionDir;
};


