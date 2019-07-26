#pragma once

#include <QWidget>
#include <QBoxLayout>
#include <QSplitter>
#include "OpenGLWindowWidget.h"
#include "WorldComponentWidget.h"

class WMainWindow : public QWidget
{
	Q_OBJECT

public:
	WMainWindow(QWidget *parent = Q_NULLPTR);
	
private:

	WOpenGLWindowCore*			Widget_OpenGLWindow;
	QHBoxLayout*			Layout_MainWindow;
	//QSplitter*				Widget_MainWindow;

	WWorldComponentWindow*	Widget_WorldComponent;
	

	int32					MinWidth;
	int32					MinHeight;
};

