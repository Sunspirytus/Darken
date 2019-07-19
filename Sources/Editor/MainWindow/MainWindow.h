#pragma once

#include <QWidget>
#include <QBoxLayout>
#include "OpenGLWindowWidget.h"
#include "WorldComponentWidget.h"

class WMainWindow : public QWidget
{
	Q_OBJECT

public:
	WMainWindow(QWidget *parent = Q_NULLPTR);
	
private:

	WOpenGLWindow*			Widget_OpenGLWindow;
	QHBoxLayout*			Layout_MainWindow;

	WWorldComponentWindow*	Widget_WorldComponent;
	

	int32					MinWidth;
	int32					MinHeight;
};

