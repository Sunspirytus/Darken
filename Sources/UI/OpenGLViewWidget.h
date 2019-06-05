#pragma once
#include "MainViewPort.h"
#include <QtWidgets/QOpenGLWidget>
#include <qopenglcontext.h>
#include <qevent.h>

class dkQOpenGLViewWidget : public QOpenGLWidget
{
	Q_OBJECT
public:
	dkQOpenGLViewWidget(QWidget* parent, Qt::WindowFlags f = Qt::WindowFlags());
	~dkQOpenGLViewWidget();

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
