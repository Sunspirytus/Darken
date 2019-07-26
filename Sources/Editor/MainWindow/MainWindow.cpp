#include "MainWindow.h"
#include "SystemContext.h"


WMainWindow::WMainWindow(QWidget *parent)
	: QWidget(parent)
	, Widget_OpenGLWindow(nullptr)
	, Layout_MainWindow(nullptr)
	, MinWidth(512)
	, MinHeight(512)
{
	{
		this->setObjectName("Widget_MainWindow");
		this->setMinimumSize(QSize(MinWidth, MinHeight)); 

		Layout_MainWindow = new QHBoxLayout(nullptr);
		Layout_MainWindow->setObjectName("Layout_MainWindow");
	}
	{
		Widget_WorldComponent = new WWorldComponentWindow(this);
	}

	{
		Widget_OpenGLWindow = new WOpenGLWindowCore(this); 
	}

	this->setLayout(Layout_MainWindow);
	Layout_MainWindow->addWidget(Widget_OpenGLWindow);
	Layout_MainWindow->addWidget(Widget_WorldComponent);
}





