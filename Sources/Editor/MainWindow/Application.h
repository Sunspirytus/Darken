#pragma once
#include <QtWidgets/QApplication>
#include "MainWindow.h"

class dkApplication : public QApplication
{
	Q_OBJECT
private:
	dkApplication(const dkApplication&);
	dkApplication& operator=(const dkApplication& engine);
	dkApplication(int& argc, char** argv);
	~dkApplication();
	
	MW_MainWindow* MainWindow;
	QString ApplicaionDir;

public:
	static dkApplication& GetInstance();
};


