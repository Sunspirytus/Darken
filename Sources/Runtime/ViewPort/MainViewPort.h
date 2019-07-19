#pragma once
#include <ViewPortBase.h>
#include "ConstructScene.h"

class MainViewPort : public ViewPortBase
{
public:
	MainViewPort();
	~MainViewPort();

	virtual void InitScene() final;
	virtual void TickScene() final;
	virtual void RenderScene() final;

private:
};
