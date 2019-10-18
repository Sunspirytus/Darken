#include "SystemContext.h"
#include "OpenGLWindowWidget.h"
#include <thread>
#include <iostream>

void CameraRotateLeftPlus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera((uint32)CameraIndex::MainCamera);
	Vector3f angles = camera->GetEulerAngle();
	angles.z -= 1.0f;
	camera->SetEulerAngle(angles);
}

void CameraRotateLeftMinus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera((uint32)CameraIndex::MainCamera);
	Vector3f angles = camera->GetEulerAngle();
	angles.z += 1.0f;
	camera->SetEulerAngle(angles);
}
void CameraRotateUpPlus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera((uint32)CameraIndex::MainCamera);
	Vector3f angles = camera->GetEulerAngle();
	angles.y += 1.0f;
	camera->SetEulerAngle(angles);
}

void CameraRotateUpMinus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera((uint32)CameraIndex::MainCamera);
	Vector3f angles = camera->GetEulerAngle();
	angles.y -= 1.0f;
	camera->SetEulerAngle(angles);
}

void CameraTranslateLeftPlus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera((uint32)CameraIndex::MainCamera);
	Vector3f left = camera->GetLeftward();
	Vector3f position = camera->GetPosition();
	camera->SetPosition(position + left * 0.2f);
}
void CameraTranslateLeftMinus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera((uint32)CameraIndex::MainCamera);
	Vector3f left = camera->GetLeftward();
	Vector3f position = camera->GetPosition();
	camera->SetPosition(position - left * 0.2f);
}

void CameraTranslateForwardPlus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera((uint32)CameraIndex::MainCamera);
	Vector3f forward = camera->GetForward();
	//forward.z = 0.0f;
	Vector3f position = camera->GetPosition();
	camera->SetPosition(position + forward * 0.2f);
}

void CameraTranslateForwardMinus(std::shared_ptr<SceneManager> _Scene)
{
	std::shared_ptr<Camera> camera = _Scene->GetCamera((uint32)CameraIndex::MainCamera);
	Vector3f forward = camera->GetForward();
	//forward.z = 0.0f;
	Vector3f position = camera->GetPosition();
	camera->SetPosition(position - forward * 0.2f);

}

WD_OpenGLWindowCore::WD_OpenGLWindowCore(QWidget * parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
	this->SetOpenGLContext();
	this->grabKeyboard();
	View = std::shared_ptr<MainViewPort>(new MainViewPort());
}

WD_OpenGLWindowCore::~WD_OpenGLWindowCore()
{
}

void WD_OpenGLWindowCore::SetOpenGLContext()
{
	QSurfaceFormat format;
	format.setSwapInterval(0);
	format.setVersion(4, 3);
	format.setSamples(4);
	format.setSwapBehavior(QSurfaceFormat::SwapBehavior::SingleBuffer);
	format.setProfile(QSurfaceFormat::CoreProfile);
	this->setFormat(format);
}

void WD_OpenGLWindowCore::initializeGL()
{
	OpenGLContext::Init();
	DKEngine::Init();
	View->InitScene();
}

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) 

void WD_OpenGLWindowCore::paintGL()
{ 
	/*if (KEY_DOWN(0x01))
	{
		CameraTranslateForwardPlus(View->Scene);
	}*/

	View->TickScene();
	View->RenderScene();
	QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void WD_OpenGLWindowCore::resizeGL(int w, int h)
{
	View->SizeChange(Vector2i(w, h));
}

void WD_OpenGLWindowCore::keyPressEvent(QKeyEvent* ev)
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
void WD_OpenGLWindowCore::keyReleaseEvent(QKeyEvent* ev)
{
	
}

std::shared_ptr<MainViewPort> WD_OpenGLWindowCore::GetView() 
{
	return View;
}

DOCK_OpenGLWindow::DOCK_OpenGLWindow(QWidget* parent)
	: QDockWidget(parent)
	, Widget_OpenGLWindow(nullptr)
{
	{
		this->setObjectName("DOCK_OpenGLWindow");
		this->setAllowedAreas(Qt::DockWidgetArea::AllDockWidgetAreas);
		this->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::AllDockWidgetFeatures);
		this->setWindowTitle("View");
	}
	
	{
		Widget_OpenGLWindow = new WD_OpenGLWindowCore(this);
	}
	this->setWidget(Widget_OpenGLWindow);
}

DOCK_OpenGLWindow::~DOCK_OpenGLWindow()
{
}

std::shared_ptr<MainViewPort> DOCK_OpenGLWindow::GetView()
{
	return Widget_OpenGLWindow->GetView();
}

