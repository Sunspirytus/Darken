#include "DarkenUI.h"


DarkenUI::DarkenUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	OpenGLViewPort = std::shared_ptr<dkQOpenGLViewWidget>(new dkQOpenGLViewWidget(ui.CentralWidget));
	QSurfaceFormat format;
	format.setSwapInterval(1);
	OpenGLViewPort->setFormat(format);
	ui.gridLayout_2->addWidget(OpenGLViewPort.get());
	
}





