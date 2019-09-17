#include "MainWindow.h"
#include "SystemContext.h"


MW_MainWindow::MW_MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, DOCK_OpenGLView(nullptr)
	, DOCK_WorldComponent(nullptr)
	, Layout_MainWindow(nullptr)
	, MinWidth(100)
	, MinHeight(100)
{
	{
		this->setObjectName("MainWindow_MainWindow");
		this->setMinimumSize(QSize(MinWidth, MinHeight)); 

		//Layout_MainWindow = new QHBoxLayout(nullptr);
		//Layout_MainWindow->setObjectName("Layout_MainWindow");
	}
	{
		DOCK_WorldComponent = new DOCK_WorldComponentWindow(nullptr);
	}

	{
		DOCK_OpenGLView = new DOCK_OpenGLWindow(this);
	}
	this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, DOCK_WorldComponent);
	this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, DOCK_OpenGLView);
	//this->setLayout(Layout_MainWindow);
	//Layout_MainWindow->addWidget(Widget_OpenGLWindow);
	//Layout_MainWindow->addWidget(Widget_WorldComponent);
}





