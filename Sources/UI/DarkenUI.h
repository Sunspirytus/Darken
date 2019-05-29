#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DarkenUI.h"
#include "OpenGLViewWidget.h"

class DarkenUI : public QMainWindow
{
	Q_OBJECT

public:
	DarkenUI(QWidget *parent = Q_NULLPTR);

private:
	Ui::DarkenUIClass ui;

	std::shared_ptr<dkQOpenGLViewWidget> OpenGLViewPort;
};

