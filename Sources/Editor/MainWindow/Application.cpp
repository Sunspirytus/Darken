#include "Application.h"
#include "EngineRoot.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>

dkApplication::dkApplication(int& argc, char** argv) 
	: QApplication(argc, argv)
	, MainWindow(nullptr)
{
	ApplicaionDir = this->applicationDirPath();
	DKEngine::GetInstance().SetEngineRunningPath(QStringToString(ApplicaionDir));
	MainWindow = new MW_MainWindow(nullptr);	

	MainWindow->show();
}

dkApplication::~dkApplication()
{
	delete MainWindow;
}