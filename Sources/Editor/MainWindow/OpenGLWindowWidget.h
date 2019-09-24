#pragma once
#include "MainViewPort.h"
#include "TypeDefine.h"
#include <QtWidgets/QOpenGLWidget>
#include <QSplitter>
#include <QBoxLayout>
#include <QDockWidget>
#include <QOpenGLContext>
#include <QEvent>

class WD_OpenGLWindowCore : public QOpenGLWidget
{
	Q_OBJECT
public:
	WD_OpenGLWindowCore(QWidget* parent, Qt::WindowFlags f = Qt::WindowFlags());
	~WD_OpenGLWindowCore();

	virtual void initializeGL() final;
	virtual void resizeGL(int w, int h) final;
	virtual void paintGL() final;

	std::shared_ptr<MainViewPort> GetView();

private:
	std::shared_ptr<MainViewPort> View;
	void SetOpenGLContext();

protected:
	virtual void keyPressEvent(QKeyEvent* ev);
	virtual void keyReleaseEvent(QKeyEvent* ev);
	
};

class DOCK_OpenGLWindow : public QDockWidget
{
	Q_OBJECT
public:
	DOCK_OpenGLWindow(QWidget* parent);
	~DOCK_OpenGLWindow();

	std::shared_ptr<MainViewPort> GetView();

private:
	WD_OpenGLWindowCore* Widget_OpenGLWindow;
};


