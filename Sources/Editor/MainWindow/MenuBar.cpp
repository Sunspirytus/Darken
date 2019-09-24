#include "MenuBar.h"
#include "TypeDefine.h"

void MW_MenuBar::AddItem(const std::string& parentTitle, const std::string& title, MenuBarItem::ItemType type, void(MW_MenuBar::* signal)())
{
	Items.insert(std::pair<std::string, std::shared_ptr<MenuBarItem>>(parentTitle, std::shared_ptr<MenuBarItem>(new MenuBarItem(title, type, signal))));
}

#define SIGNAL_STR(A) #A
MW_MenuBar::MW_MenuBar(QWidget* parent)
	:	QMenuBar(parent)
{
	AddItem("", "File", MenuBarItem::ItemType::Menu, nullptr);

	AddItem("File", "SaveScene", MenuBarItem::ItemType::Action, &MW_MenuBar::saveScene);

	Setup();
}

MW_MenuBar::~MW_MenuBar()
{
}

void MW_MenuBar::Setup()
{
	std::map<std::string, QMenu*> Menus;
	for(std::map<std::string, std::shared_ptr<MenuBarItem>>::iterator it = Items.begin(); it != Items.end(); it++)
	{		
		const std::string ParentTitle = it->first;
		const std::shared_ptr<MenuBarItem> Item = it->second;
		if(ParentTitle == "")
		{
			switch (Item->Type)
			{
			case MenuBarItem::ItemType::Menu: 
			{
				QMenu* Menu = new QMenu(Item->Title.c_str(), this);
				Menus.insert(std::pair<std::string, QMenu*>(Item->Title, Menu));
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
				Menus.insert(std::pair<std::string, QMenu*>(Item->Title, Menu));
				ParentMenu->addMenu(Menu);
				break;
			}
			case MenuBarItem::ItemType::Action:
			{				
				QAction* Action = new QAction(Item->Title.c_str(), ParentMenu);
				ParentMenu->addAction(Action);
				ItemData* Signal = new ItemData(Item->Signal);
				Action->setUserData(0, Signal);
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