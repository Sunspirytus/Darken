#pragma once
#include <QtWidgets/QApplication>

class dkApplication : public QApplication
{
	Q_OBJECT
public:
	dkApplication(int& argc, char** argv);
	~dkApplication();
	
private:
	int a = 0;
};


