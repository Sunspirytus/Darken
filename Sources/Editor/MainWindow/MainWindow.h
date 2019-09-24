#pragma once

#include <QWidget>
#include <QBoxLayout>
#include <QSplitter>
#include <QMainWindow>
#include "OpenGLWindowWidget.h"
#include "WorldComponentWindow.h"
#include "MenuBar.h"

class MW_MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MW_MainWindow(QWidget *parent = Q_NULLPTR);

private slots:
	void SaveScene();

private:
	DOCK_OpenGLWindow*	DOCK_OpenGLView;
	QHBoxLayout*			Layout_MainWindow;
	//QSplitter*				Widget_MainWindow;

	DOCK_WorldComponentWindow* DOCK_WorldComponent;
	MW_MenuBar*				MenuBar;

	int32					MinWidth;
	int32					MinHeight;
};

