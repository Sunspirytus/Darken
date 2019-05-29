#include "SystemContext.h"
#include "OpenGLViewWidget.h"

Test2::Test2(QObject* parent) : QObject(parent)
{

}

Test2::~Test2()
{
}

void Test2::show()
{
	std::cout << "TEST" << std::endl;
}


dkQOpenGLViewWidget::dkQOpenGLViewWidget(QWidget * parent, Qt::WindowFlags f)
{
	QOpenGLWidget(parent, f);
	View = std::shared_ptr<MainViewPort>(new MainViewPort());
	t = new Test2(this);
	connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
	if (format().swapInterval() == -1)
	{
		// V_blank synchronization not available (tearing likely to happen)
		qDebug("Swap Buffers at v_blank not available: refresh at approx 60fps.");
		timer.setInterval(17);
	}
	else
	{
		// V_blank synchronization available
		timer.setInterval(0);
	}
	timer.start();
}

dkQOpenGLViewWidget::~dkQOpenGLViewWidget()
{
}

void dkQOpenGLViewWidget::initializeGL()
{
	OpenGLContext::Init();
	View->InitScene();
}

int a = 0;
void dkQOpenGLViewWidget::paintGL()
{ 
	//View->TickScene();
	//View->RenderScene();

	//a++;
	///*if(a >= 0 && a <10)
	//{
	//	glClearColor(1.0, 0.0, 0.0, 1.0);
	//	glClear(GL_COLOR_BUFFER_BIT);
	//}
	//else if(a >= 10 && a< 20)
	//{
	//	glClearColor(1.0, 1.0, 0.0, 1.0);
	//	glClear(GL_COLOR_BUFFER_BIT);
	//}else if(a >= 20)
	//{
	//	a = 0;
	//}*/
	//int b = defaultFramebufferObject();
	//std::cout << a << std::endl;
	//doneCurrent();
}

void dkQOpenGLViewWidget::updateGL()
{
	//View->TickScene();
	//View->RenderScene();
	makeCurrent();
	glBindFramebuffer(GL_FRAMEBUFFER, 37);
	a++;
	if(a >= 0 && a <100)
	{
		glClearColor(1.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	else if(a >= 100 && a< 200)
	{
		glClearColor(1.0, 1.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}else if(a >= 200)
	{
		a = 0;
	}
	int b = defaultFramebufferObject();
	std::cout << a << std::endl;
	context()->swapBuffers(context()->surface());
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void dkQOpenGLViewWidget::resizeGL(int w, int h)
{

}