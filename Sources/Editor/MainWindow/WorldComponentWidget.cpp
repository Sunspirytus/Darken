#include "WorldComponentWidget.h"

WWorldComponentWindow::WWorldComponentWindow(QWidget* parent)
	: QWidget(parent)
	, Layout_WorldComponent(nullptr)
	, MinWidth(50)
	, MinHeight(100)
{
	this->setObjectName("Widget_WorldComponentWindow");
	this->setMinimumSize(QSize(MinWidth, MinHeight));

	Layout_WorldComponent = new QVBoxLayout(this);
	Layout_WorldComponent->setObjectName("Layout_WorldComponent");
}

WWorldComponentWindow::~WWorldComponentWindow()
{
}