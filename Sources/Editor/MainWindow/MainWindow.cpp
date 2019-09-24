#include "MainWindow.h"
#include "SystemContext.h"
#include "FileIO.h"

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
		MenuBar = new MW_MenuBar(this);
		this->setMenuBar(MenuBar);
		QMetaObject::Connection Connection = connect(MenuBar, SIGNAL(saveScene()), this, SLOT(SaveScene()));
	}

	{
		DOCK_OpenGLView = new DOCK_OpenGLWindow(this);
	}

	{
		DOCK_WorldComponent = new DOCK_WorldComponentWindow(nullptr);
	}
	
	this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, DOCK_WorldComponent);
	this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, DOCK_OpenGLView);
	//this->setLayout(Layout_MainWindow);
	//Layout_MainWindow->addWidget(Widget_OpenGLWindow);
	//Layout_MainWindow->addWidget(Widget_WorldComponent);

	//DOCK_WorldComponent->GetTable()->UpdateComponent(DOCK_OpenGLView->GetView()->Scene);
}

void MW_MainWindow::SaveScene()
{
	std::string SaveData;
	DOCK_OpenGLView->GetView()->GetScene()->GetSaveInfo(&SaveData);
	//std::string a = PropertyDataToString(Name, 123);
	int b = 0;
}







