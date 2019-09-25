#pragma once

#include <QMenuBar>
#include <QAction>
#include <QMenu>
#include <map>

class MW_MenuBar;

class ItemData : public QObjectUserData
{
public:
	ItemData(void(MW_MenuBar::* actionSignal)())
		: ActionSignal(actionSignal)
	{};
	~ItemData() {};

	void(MW_MenuBar::* ActionSignal)();
};

class MenuBarItem
{
public:

	enum ItemType
	{
		Menu,
		Action
	};

	MenuBarItem(const std::string& title, ItemType type, void(MW_MenuBar::* actionSignal)())
		: Title(title)
		, Type(type)
		, Signal(actionSignal)
	{
	};
	~MenuBarItem() {};

	std::string Title;
	ItemType Type;
	void(MW_MenuBar::* Signal)();
};



class MW_MenuBar : public QMenuBar
{
	Q_OBJECT

public:
	MW_MenuBar(QWidget* parent = Q_NULLPTR);
	~MW_MenuBar();

public slots:
	void triggerMenu(QAction* Action);

signals:
	void saveScene();
	void saveProject();
	void loadProject();

private:
	void Setup();
	void AddItem(const std::string& parentTitle, const std::string& title, MenuBarItem::ItemType type, void(MW_MenuBar::* signal)());
	std::multimap<std::string, std::shared_ptr<MenuBarItem>> Items;
};

