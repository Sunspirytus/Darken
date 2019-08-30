#pragma once
#include "MainViewPort.h"
#include "TypeDefine.h"
#include <QtWidgets/QOpenGLWidget>
#include <QSplitter>
#include <QBoxLayout>
#include <qopenglcontext.h>
#include <qevent.h>

class WOpenGLWindowCore : public QOpenGLWidget
{
	Q_OBJECT
public:
	WOpenGLWindowCore(QWidget* parent, Qt::WindowFlags f = Qt::WindowFlags());
	~WOpenGLWindowCore();

	virtual void initializeGL() final;
	virtual void resizeGL(int w, int h) final;
	virtual void paintGL() final;

private:
	std::shared_ptr<MainViewPort> View;
	void SetOpenGLContext();

protected:
	virtual void keyPressEvent(QKeyEvent* ev);
	virtual void keyReleaseEvent(QKeyEvent* ev);
	
};

class WOpenGLWindowSplitter : public QSplitter
{
	Q_OBJECT
public:
	WOpenGLWindowSplitter(QWidget* parent, Qt::WindowFlags f = Qt::WindowFlags());
	~WOpenGLWindowSplitter();

private:
	WOpenGLWindowCore* Widget_OpenGLWindow;
	QHBoxLayout*		Layout_OpenGLWindow;
};


