#include "MainWindow.h"
#include "SystemContext.h"
#include "FileIO.h"
#include "FileDialog.h"

MW_MainWindow::MW_MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, DOCK_OpenGLView(nullptr)
	, DOCK_WorldComponent(nullptr)
	, Layout_MainWindow(nullptr)
	, ProjectDir("..\\Assets\\")
{
	{
		this->setObjectName("MainWindow_MainWindow");
	}

	{
		MenuBar = new MW_MenuBar(this);
		this->setMenuBar(MenuBar);
		QMetaObject::Connection Connection;
		Connection = connect(MenuBar, SIGNAL(saveScene()), this, SLOT(SaveScene()));
		Connection = connect(MenuBar, SIGNAL(saveProject()), this, SLOT(SaveProject()));
		Connection = connect(MenuBar, SIGNAL(loadProject()), this, SLOT(LoadProject()));
	}

	{
		DOCK_OpenGLView = new DOCK_OpenGLWindow(this);
	}

	{
		DOCK_WorldComponent = new DOCK_WorldComponentWindow(nullptr);
	}
	
	this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, DOCK_WorldComponent);
	this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, DOCK_OpenGLView);
}

void MW_MainWindow::SaveScene()
{
	std::string SaveData;
	DOCK_OpenGLView->GetView()->GetScene()->GetSaveInfo(&SaveData);
	FileIO FIO;
	FIO.SaveFile(ProjectDir, "SceneDemo", FileType::WorldScene, SaveData);
}

void MW_MainWindow::SaveProject()
{
	std::string SaveData;
	SaveData =
	{
		"PrjDemo"
	};

	FileIO FIO;
	FIO.SaveFile(ProjectDir, "PrjDemo", FileType::Project, SaveData);

	SaveScene();
}

void MW_MainWindow::LoadProject()
{
	FileDialog FD;
	
	
}







