#pragma once

#include "TypeDefine.h"
#include <QMenuBar>
#include <QAction>
#include <QMenu>
#include <QToolTip>
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
		Action,
		Separator
	};

	MenuBarItem(const String& title,
		const String& describe,
		const String& hotKey,
		ItemType type,
		void(MW_MenuBar::* actionSignal)()
	)
		: Title(title)
		, Describe(describe)
		, HotKey(hotKey)
		, Type(type)
		, Signal(actionSignal)
	{
	};
	~MenuBarItem() {};

	String Title;
	String Describe;
	String HotKey;
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
	void newScene();
	void openScene();
	void saveScene();

	void newProject();
	void loadProject();
	void saveProject();	

	void exitApplication();

private:
	void Setup();
	void AddItem(const String& parentTitle, const String& title, const String& describe, const String& hotKey, MenuBarItem::ItemType type, void(MW_MenuBar::* signal)());
	std::multimap<String, std::shared_ptr<MenuBarItem>> Items;
};

