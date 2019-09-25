#include "MenuBar.h"
#include "TypeDefine.h"

void MW_MenuBar::AddItem(const String& parentTitle, const String& title, const String& describe, const String& hotKey, MenuBarItem::ItemType type, void(MW_MenuBar::* signal)())
{
	Items.insert(std::pair<String, std::shared_ptr<MenuBarItem>>(parentTitle, std::shared_ptr<MenuBarItem>(new MenuBarItem(title, describe, hotKey, type, signal))));
}

#define SIGNAL_STR(A) #A
MW_MenuBar::MW_MenuBar(QWidget* parent)
	:	QMenuBar(parent)
{
	//ParentMenuName	Name				Describe					ShortCut		Type								Signal
	AddItem("",			"File",				"Open File Menu",			"",				MenuBarItem::ItemType::Menu,		nullptr);

	AddItem("File",		"New Scene",		"Create New Scene",			"Ctrl+N",		MenuBarItem::ItemType::Action,		&MW_MenuBar::newScene);
	AddItem("File",		"Open Scene",		"Load Scene File",			"Ctrl+O",		MenuBarItem::ItemType::Action,		&MW_MenuBar::openScene);
	AddItem("File",		"Save Scene",		"Save Scene File",			"Ctrl+S",		MenuBarItem::ItemType::Action,		&MW_MenuBar::saveScene);
	AddItem("File",		"",					"",							"",				MenuBarItem::ItemType::Separator,	nullptr);
	
	AddItem("File",		"New Project",		"Create New Project",		"",				MenuBarItem::ItemType::Action,		&MW_MenuBar::newProject);
	AddItem("File",		"Load Project",		"Load Project File",		"",				MenuBarItem::ItemType::Action,		&MW_MenuBar::loadProject);
	AddItem("File",		"Save Project",		"Save Project File",		"",				MenuBarItem::ItemType::Action,		&MW_MenuBar::saveProject);
	AddItem("File",		"",					"",							"",				MenuBarItem::ItemType::Separator,	nullptr);

	AddItem("File",		"Exit",				"Exit Application",			"Alt+F4",		MenuBarItem::ItemType::Action,		&MW_MenuBar::exitApplication);
	Setup();
}

MW_MenuBar::~MW_MenuBar()
{
}

void MW_MenuBar::Setup()
{
	std::map<String, QMenu*> Menus;
	for(std::map<String, std::shared_ptr<MenuBarItem>>::iterator it = Items.begin(); it != Items.end(); it++)
	{
		const String ParentTitle = it->first;
		const std::shared_ptr<MenuBarItem> Item = it->second;
		if(ParentTitle == "")
		{
			switch (Item->Type)
			{
			case MenuBarItem::ItemType::Menu: 
			{
				QMenu* Menu = new QMenu(Item->Title.c_str(), this);
				Menu->setToolTipsVisible(true);
				Menus.insert(std::pair<String, QMenu*>(Item->Title, Menu));
				this->addMenu(Menu);

				break; 
			}
			case MenuBarItem::ItemType::Action:
			{
				QAction* Action = new QAction(Item->Title.c_str(), this);
				this->addAction(Action);
				ItemData* Signal = new ItemData(Item->Signal);
				Action->setUserData(0, Signal);
				break; 
			}
			default:
				break;
			}
		}
		else
		{
			QMenu* ParentMenu = Menus.find(ParentTitle)->second;
			switch (Item->Type)
			{
			case MenuBarItem::ItemType::Menu:
			{
				QMenu* Menu = new QMenu(Item->Title.c_str(), ParentMenu);
				Menu->setToolTipsVisible(true);
				Menus.insert(std::pair<String, QMenu*>(Item->Title, Menu));
				ParentMenu->addMenu(Menu);
				break;
			}
			case MenuBarItem::ItemType::Action:
			{				
				QAction* Action = new QAction(Item->Title.c_str(), ParentMenu);
				Action->setToolTip(Item->Describe.c_str());
				Action->setShortcut(tr(Item->HotKey.c_str()));
				ParentMenu->addAction(Action);
				ItemData* Signal = new ItemData(Item->Signal);
				Action->setUserData(0, Signal);
				break; 
			}
			case MenuBarItem::ItemType::Separator:
			{
				ParentMenu->addSeparator();
				break;
			}
			default:
				break;
			}
		}
	}
	QMetaObject::Connection Connection = connect(this, SIGNAL(triggered(QAction*)), this, SLOT(triggerMenu(QAction*)));
}

void MW_MenuBar::triggerMenu(QAction * Action)
{
	ItemData* Signal = (ItemData*) Action->userData(0);
	emit (this->*Signal->ActionSignal)();
}