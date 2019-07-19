#include "SystemContext.h"
#include "OpenGLWindowWidget.h"
#include <thread>
#include <iostream>
#include "GlobalPram.h"

void CameraRotateLeftPlus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera(CameraIndex::MainCamera);
	Vector3f angles = camera->GetEulerAngle();
	angles.z -= 1.0f;
	camera->SetEulerAngle(angles);
}

void CameraRotateLeftMinus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera(CameraIndex::MainCamera);
	Vector3f angles = camera->GetEulerAngle();
	angles.z += 1.0f;
	camera->SetEulerAngle(angles);
}
void CameraRotateUpPlus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera(CameraIndex::MainCamera);
	Vector3f angles = camera->GetEulerAngle();
	angles.y += 1.0f;
	camera->SetEulerAngle(angles);
}

void CameraRotateUpMinus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera(CameraIndex::MainCamera);
	Vector3f angles = camera->GetEulerAngle();
	angles.y -= 1.0f;
	camera->SetEulerAngle(angles);
}

void CameraTranslateLeftPlus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera(CameraIndex::MainCamera);
	Vector3f left = camera->GetLeftward();
	Vector3f position = camera->GetPosition();
	camera->SetPosition(position + left * 0.2f);
}
void CameraTranslateLeftMinus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera(CameraIndex::MainCamera);
	Vector3f left = camera->GetLeftward();
	Vector3f position = camera->GetPosition();
	camera->SetPosition(position - left * 0.2f);
}

void CameraTranslateForwardPlus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera(CameraIndex::MainCamera);
	Vector3f forward = camera->GetForward();
	//forward.z = 0.0f;
	Vector3f position = camera->GetPosition();
	camera->SetPosition(position + forward * 0.2f);
}

void CameraTranslateForwardMinus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera(CameraIndex::MainCamera);
	Vector3f forward = camera->GetForward();
	//forward.z = 0.0f;
	Vector3f position = camera->GetPosition();
	camera->SetPosition(position - forward * 0.2f);

}

WOpenGLWindow::WOpenGLWindow(QWidget * parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
	this->SetOpenGLContext();
	this->grabKeyboard();
	View = std::shared_ptr<MainViewPort>(new MainViewPort());
}

WOpenGLWindow::~WOpenGLWindow()
{
}

void WOpenGLWindow::SetOpenGLContext()
{
	QSurfaceFormat format;
	format.setSwapInterval(0);
	format.setVersion(4, 3);
	format.setSamples(4);
	format.setSwapBehavior(QSurfaceFormat::SwapBehavior::SingleBuffer);
	format.setProfile(QSurfaceFormat::CoreProfile);
	this->setFormat(format);
}

void WOpenGLWindow::initializeGL()
{
	OpenGLContext::Init();
	View->InitScene();
}

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) 

void WOpenGLWindow::paintGL()
{ 
	if (KEY_DOWN(0x01))
	{
		CameraTranslateForwardPlus(View->Scene);
	}

	View->TickScene();
	View->RenderScene();
	QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void WOpenGLWindow::resizeGL(int w, int h)
{

}

void WOpenGLWindow::keyPressEvent(QKeyEvent* ev)
{
	//if (ev->key() == Qt::Key_W)
	//{
	//	std::cout << "1" << std::endl;
	//	//CameraTranslateForwardPlus(View->Scene); 
	//}
	/*else if (ev->key() == Qt::Key_S && ev->isAutoRepeat())
		CameraTranslateForwardMinus(View->Scene);
	else if (ev->key() == Qt::Key_A && ev->isAutoRepeat())
		CameraTranslateLeftPlus(View->Scene);
	else if (ev->key() == Qt::Key_D && ev->isAutoRepeat())
		CameraTranslateLeftMinus(View->Scene);
	if (ev->key() == Qt::Key_Left && ev->isAutoRepeat())
		CameraRotateLeftPlus(View->Scene);
	else if (ev->key() == Qt::Key_Right && ev->isAutoRepeat())
		CameraRotateLeftMinus(View->Scene);
	else if (ev->key() == Qt::Key_Up && ev->isAutoRepeat())
		CameraRotateUpPlus(View->Scene);
	else if (ev->key() == Qt::Key_Down && ev->isAutoRepeat())
		CameraRotateUpMinus(View->Scene);*/
	update();
}
void WOpenGLWindow::keyReleaseEvent(QKeyEvent* ev)
{
	
}