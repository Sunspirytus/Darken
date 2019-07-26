#pragma once
#include "MainViewPort.h"
#include "TypeDefine.h"
#include <QtWidgets/QOpenGLWidget>
#include <QSplitter>
#include <qopenglcontext.h>
#include <qevent.h>

class WOpenGLWindow : public QOpenGLWidget
{
	Q_OBJECT
public:
	WOpenGLWindow(QWidget* parent, Qt::WindowFlags f = Qt::WindowFlags());
	~WOpenGLWindow();

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

class WOpenGLWindow : public QSplitter
{
public:
	WOpenGLWindowSplitter();
	~WOpenGLWindowSplitter();

private:

};

WOpenGLWindowSplitter::WOpenGLWindowSplitter()
{
}

WOpenGLWindowSplitter::~WOpenGLWindowSplitter()
{
}
