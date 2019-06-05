#include "DarkenUI.h"
#include "SystemContext.h"


DarkenUI::DarkenUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	OpenGLViewPort = std::shared_ptr<dkQOpenGLViewWidget>(new dkQOpenGLViewWidget(ui.CentralWidget));
	ui.verticalLayout->addWidget(OpenGLViewPort.get());

}





