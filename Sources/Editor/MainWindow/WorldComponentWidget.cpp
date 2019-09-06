#include "WorldComponentWidget.h"

DOCK_WorldComponentWindow::DOCK_WorldComponentWindow(QWidget* parent)
	: QDockWidget(parent)
	, Layout_WorldComponent(nullptr)
	, MinWidth(50)
	, MinHeight(100)
{
	{
		this->setObjectName("DOCK_WorldComponentWindow");
		this->setMinimumSize(QSize(MinWidth, MinHeight)); 
		this->setAllowedAreas(Qt::DockWidgetArea::AllDockWidgetAreas);
		this->setFeatures(QDockWidget::DockWidgetMovable);
		this->setWindowTitle("World");
	}

	{
		WWorldComponent = new QWidget(this);
	}
	{
		Layout_WorldComponent = new QVBoxLayout(nullptr);
		Layout_WorldComponent->setObjectName("Layout_WorldComponent"); 
	}

	this->setWidget(WWorldComponent);

	WWorldComponent->setLayout(Layout_WorldComponent);
	
	TextEdit_Test = new QTextEdit(QObject::tr("Left Widget"), nullptr);
	//TextEdit_Test->setAlignment(Qt::AlignCenter);
	Layout_WorldComponent->addWidget(TextEdit_Test);
	TextEdit_Test2 = new QTextEdit(QObject::tr("Left Widget2"), nullptr);
	//TextEdit_Test2->setAlignment(Qt::AlignCenter);
	//Layout_WorldComponent->addWidget(TextEdit_Test2);
}

DOCK_WorldComponentWindow::~DOCK_WorldComponentWindow()
{
}