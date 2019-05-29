#pragma once
#include "MainViewPort.h"
#include <QtWidgets/QOpenGLWidget>
#include <qopenglcontext.h>
#include <qtimer.h>

class Test2 : public QObject
{
	Q_OBJECT
public:
	Test2(QObject* parent);
	~Test2();
	
	 
public slots:
	void show();
private:

};

class dkQOpenGLViewWidget : public QOpenGLWidget
{
	Q_OBJECT
public:
	dkQOpenGLViewWidget(QWidget* parent, Qt::WindowFlags f = Qt::WindowFlags());
	~dkQOpenGLViewWidget();

	virtual void initializeGL() final;
	virtual void resizeGL(int w, int h) final;
	virtual void paintGL() final;
public slots:
	void updateGL();
private:
	std::shared_ptr<MainViewPort> View;
	Test2* t;
	QTimer timer;
	//QOpenGLContext* Context;
	//QSurface* Surface;
	
};
