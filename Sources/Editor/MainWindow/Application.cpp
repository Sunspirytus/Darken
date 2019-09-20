#include "Application.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>

dkApplication::dkApplication(int& argc, char** argv) 
	: QApplication(argc, argv)
	, MainWindow(nullptr)
{
	MainWindow = new MW_MainWindow(nullptr);
	MainWindow->show();
}

dkApplication::~dkApplication()
{
}