#pragma once

#include <QMenuBar>
#include <map>

class MenuBarItem
{
public:

	enum ItemType
	{
		Menu,
		Action
	};

	MenuBarItem(const std::string& title, ItemType type);
	~MenuBarItem();

	std::string Title;
	ItemType Type;	
};

class MW_MenuBar : public QMenuBar
{
	Q_OBJECT

public:
	MW_MenuBar();
	~MW_MenuBar();

private:
	void Setup();
	void AddItem(const std::string& parentTitle, const std::string& title, MenuBarItem::ItemType type);
	std::multimap<std::string, std::shared_ptr<MenuBarItem>> Items;
};

