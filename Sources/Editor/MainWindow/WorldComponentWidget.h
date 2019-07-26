#pragma once
#include <QWidget>
#include <QLayout>
#include <QSplitter>
#include <QTextEdit>
#include "TypeDefine.h"


class WWorldComponentWindow : public QSplitter
{
	Q_OBJECT
public:
	WWorldComponentWindow(QWidget* parent = Q_NULLPTR);
	~WWorldComponentWindow();

private:
	QVBoxLayout* Layout_WorldComponent;
	QTextEdit* TextEdit_Test;
	int32				MinWidth;
	int32				MinHeight;
};

