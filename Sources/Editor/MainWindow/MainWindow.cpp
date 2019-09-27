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
	String SaveData;
	DOCK_OpenGLView->GetView()->GetScene()->GetSaveInfo(&SaveData);
	FileIO FIO;
	FIO.SaveFile(ProjectDir, "SceneDemo", FileType::WorldScene, SaveData);
}

void MW_MainWindow::SaveMaterials()
{
	
}

void MW_MainWindow::SaveProject()
{
	String SaveData;
	SaveData =
	{
		"PrjDemo"
	};

	FileIO FIO;
	FIO.SaveFile(ProjectDir, "PrjDemo", FileType::Project, SaveData);

	SaveScene();
}

enum test
{
	test1,
	test2,
	test3
};

void MW_MainWindow::LoadProject()
{
	FileDialog FD;
	std::vector<String> ProjectFilePaths = FD.OpenAndGetFileName("Load Project", "ProjectFile(*.dkProject)", FileDialog::SelectMode::Single);
	if (ProjectFilePaths.size())
	{
		QFileInfo FileInfo(ProjectFilePaths[0].c_str());
		this->ProjectDir = QStringToString(FileInfo.absolutePath());
		String a = GetNameFromPath(ProjectFilePaths[0]);
		String b = GetNameExceptSuffix(a);
		String c = GetNameFromPathExceptSuffix(ProjectFilePaths[0]);
	}
	DOCK_WorldComponent->GetTable()->UpdateComponent(DOCK_OpenGLView->GetView()->GetScene());

	

	

	int b = 0;
}







