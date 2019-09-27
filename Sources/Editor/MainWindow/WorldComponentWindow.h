#pragma once
#include <QWidget>
#include <QLayout>
#include <QSplitter>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>
#include <QDockWidget>
#include <QScrollArea>
#include <qevent.h>
#include <QPushButton>
#include "TypeDefine.h"
#include "SceneManager.h"

class WD_WorldComponentsSearch : public QWidget
{
	Q_OBJECT
public:
	WD_WorldComponentsSearch(QWidget* parent = Q_NULLPTR);
	~WD_WorldComponentsSearch();

private:
	QVBoxLayout* Layout_Main;
	QLineEdit* LE_Search;
	QHBoxLayout* Layout_InSearch;
	QPushButton* PB_Search;
};


class WD_WorldComponentsTable : public QWidget
{
	Q_OBJECT
public:
	WD_WorldComponentsTable(QWidget* parent = Q_NULLPTR);
	~WD_WorldComponentsTable();
	
	void UpdateComponent(std::shared_ptr<SceneManager> Scene);
private:
	QTableWidget* Table_Components;
	QVBoxLayout* Layout_Main;
	
protected:
	virtual void resizeEvent(QResizeEvent* event);
private slots:

};


class DOCK_WorldComponentWindow : public QDockWidget
{
	Q_OBJECT
public:
	DOCK_WorldComponentWindow(QWidget* parent = Q_NULLPTR);
	~DOCK_WorldComponentWindow();

	WD_WorldComponentsTable* GetTable();
private:
	QWidget* WD_WorldComponentWindow;
	QVBoxLayout* Layout_WorldComponent;

	WD_WorldComponentsSearch* WD_ComponentsSearch;
	WD_WorldComponentsTable* WD_ComponentsTable;

	int32				MinWidth;
	int32				MinHeight;
};

