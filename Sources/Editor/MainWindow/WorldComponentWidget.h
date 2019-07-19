#pragma once
#include <QWidget>
#include <QLayout>
#include "TypeDefine.h"


class WWorldComponentWindow : public QWidget
{
	Q_OBJECT
public:
	WWorldComponentWindow(QWidget* parent = Q_NULLPTR);
	~WWorldComponentWindow();

private:
	QVBoxLayout*		Layout_WorldComponent;
	int32				MinWidth;
	int32				MinHeight;
};

