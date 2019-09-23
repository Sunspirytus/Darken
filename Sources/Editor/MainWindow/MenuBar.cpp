#include "MenuBar.h"

MenuBarItem::MenuBarItem(const std::string& title, ItemType type)
	:	Title(title)
	,	Type(type)
{
	
}

MenuBarItem::~MenuBarItem()
{
}

void MW_MenuBar::AddItem(const std::string& parentTitle, const std::string& title, MenuBarItem::ItemType type)
{
	Items.insert(std::pair<std::string, std::shared_ptr<MenuBarItem>>(parentTitle, std::shared_ptr<MenuBarItem>(new MenuBarItem(title, type))));
}


MW_MenuBar::MW_MenuBar()
{
	AddItem("", "File", MenuBarItem::ItemType::Menu);
	AddItem("", "Edit", MenuBarItem::ItemType::Menu);

	AddItem("File", "SaveScene", MenuBarItem::ItemType::Action);

	Setup();
}

MW_MenuBar::~MW_MenuBar()
{
}

void MW_MenuBar::Setup()
{
	for(std::map<std::string, std::shared_ptr<MenuBarItem>>::iterator it = Items.begin(); it != Items.end(); it++)
	{
		std::map<std::string, QMenu*> Menus;
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
				break; 
			}
			default:
				break;
			}
		}
		else
		{
			switch (Item->Type)
			{
			case MenuBarItem::ItemType::Menu:
			{
				QMenu* Menu = new QMenu(Item->Title.c_str(), this);
				Menus.insert(std::pair<std::string, QMenu*>(Item->Title, Menu));
				QMenu* ParentMenu = Menus.find(ParentTitle)->second;
				ParentMenu->addMenu(Menu);
				break;
			}
			case MenuBarItem::ItemType::Action:
			{
				QAction* Action = new QAction(Item->Title.c_str(), this);
				QMenu* ParentMenu = Menus.find(ParentTitle)->second;
				ParentMenu->addAction(it->second->Title.c_str());
				break; 
			}
			default:
				break;
			}
		}
	}
}