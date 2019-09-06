#pragma once

#include <QWidget>
#include <QBoxLayout>
#include <QSplitter>
#include <QMainWindow>
#include "OpenGLWindowWidget.h"
#include "WorldComponentWindow.h"

class MW_MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MW_MainWindow(QWidget *parent = Q_NULLPTR);
	
private:
	DOCK_OpenGLWindow*	DOCK_OpenGLView;
	QHBoxLayout*			Layout_MainWindow;
	//QSplitter*				Widget_MainWindow;

	DOCK_WorldComponentWindow* DOCK_WorldComponent;
	

	int32					MinWidth;
	int32					MinHeight;
};

