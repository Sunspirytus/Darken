#include "WorldComponentWidget.h"

WWorldComponentWindow::WWorldComponentWindow(QWidget* parent)
	: QSplitter(parent)
	, Layout_WorldComponent(nullptr)
	, MinWidth(50)
	, MinHeight(100)
{
	this->setObjectName("Widget_WorldComponentWindow");
	this->setMinimumSize(QSize(MinWidth, MinHeight));

	Layout_WorldComponent = new QVBoxLayout(this);
	Layout_WorldComponent->setObjectName("Layout_WorldComponent");

	TextEdit_Test = new QTextEdit(QObject::tr("Left Widget"), this);
	TextEdit_Test->setAlignment(Qt::AlignCenter);

}

WWorldComponentWindow::~WWorldComponentWindow()
{
}