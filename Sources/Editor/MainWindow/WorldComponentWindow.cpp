#include "WorldComponentWindow.h"

WD_WorldComponentsSearch::WD_WorldComponentsSearch(QWidget* parent)
	: QWidget(parent)
{
	
	Layout_Main = new QVBoxLayout(nullptr);
	Layout_Main->setMargin(0);

	this->setObjectName("Widget_WorldComponentsSearch");
	this->setLayout(Layout_Main);

	LE_Search = new QLineEdit(parent);
	LE_Search->setObjectName("LE_Search");
	LE_Search->setPlaceholderText("Search");
	LE_Search->setAlignment(Qt::AlignLeft);

	Layout_Main->addWidget(LE_Search);

	Layout_InSearch = new QHBoxLayout(nullptr);
	Layout_InSearch->setAlignment(Qt::AlignRight);
	Layout_InSearch->setMargin(3);
	
	LE_Search->setLayout(Layout_InSearch);
	PB_Search = new QPushButton("1", LE_Search);
	PB_Search->setMaximumWidth(20);
	Layout_InSearch->addWidget(PB_Search);
}

WD_WorldComponentsSearch::~WD_WorldComponentsSearch()
{
}

WD_WorldComponentsTable::WD_WorldComponentsTable(QWidget* parent)
	: QWidget(parent)
{
	Layout_Main = new QVBoxLayout(nullptr);
	
	Layout_Main->setObjectName("Layout_Components");
	this->setLayout(Layout_Main);

	Table_Components = new QTableWidget(parent);
	Table_Components->setObjectName("Table_Components");
	Table_Components->setColumnCount(2);
	Table_Components->setRowCount(2);
	Table_Components->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	Table_Components->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
	Table_Components->setShowGrid(false);
	Table_Components->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
	Table_Components->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	QStringList HorizenHeaders;
	HorizenHeaders << "Tag" << "Type";
	Table_Components->setHorizontalHeaderLabels(HorizenHeaders);

	Layout_Main->addWidget(Table_Components);
}

WD_WorldComponentsTable::~WD_WorldComponentsTable()
{
}

void WD_WorldComponentsTable::resizeEvent(QResizeEvent* event)
{
	const QSize ParentSize = this->parentWidget()->size();
	this->setBaseSize(ParentSize);
}
#define ToString(x) #x
void WD_WorldComponentsTable::UpdateComponent(std::shared_ptr<SceneManager> Scene)
{
	std::vector<std::shared_ptr<Object>> Objects = Scene->GetObjects(ObjectType::Default);
	int32 RowCount = Table_Components->rowCount();
	for(int32 Index = 0; Index < Objects.size(); Index++)
	{
		Table_Components->insertRow(RowCount + Index);
		//Table_Components->setItem(RowCount - 1, 0, new QTableWidgetItem(Objects[Index]->Properties[TO_String(Name)]->DatasInString[0].c_str()));
		//std::shared_ptr<PropertyData> Data = Objects[Index]->Properties[TO_String(Type)];
		//Table_Components->setItem(RowCount - 1, 1, new QTableWidgetItem(Data->DatasInString[Data->Data].c_str()));
	}
	
}


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
		WD_WorldComponentWindow = new QWidget(this);
		WD_WorldComponentWindow->setObjectName("Widget_WorldComponentWindow");
	}
	{
		Layout_WorldComponent = new QVBoxLayout(nullptr);
		Layout_WorldComponent->setObjectName("Layout_WorldComponent"); 
		Layout_WorldComponent->setAlignment(Qt::AlignTop);
	}

	this->setWidget(WD_WorldComponentWindow);

	WD_WorldComponentWindow->setLayout(Layout_WorldComponent);
	
	WD_ComponentsSearch = new WD_WorldComponentsSearch(WD_WorldComponentWindow);
	Layout_WorldComponent->addWidget(WD_ComponentsSearch);

	WD_ComponentsTable = new WD_WorldComponentsTable(WD_ComponentsSearch);
	Layout_WorldComponent->addWidget(WD_ComponentsTable);
	
}

DOCK_WorldComponentWindow::~DOCK_WorldComponentWindow()
{
}

WD_WorldComponentsTable* DOCK_WorldComponentWindow::GetTable()
{
	return WD_ComponentsTable;
}