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
	void SaveProject();
	void LoadProject();

private:
	DOCK_OpenGLWindow* DOCK_OpenGLView;
	QHBoxLayout* Layout_MainWindow;
	

	DOCK_WorldComponentWindow* DOCK_WorldComponent;
	MW_MenuBar* MenuBar;

	String ProjectDir;
};

