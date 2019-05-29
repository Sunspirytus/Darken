#pragma once
#include "TypeDefine.h"
#include "SceneManager.h"
#include "RenderPipelineBase.h"

class ViewPortBase
{
public:
	ViewPortBase();
	~ViewPortBase();

	virtual void InitScene() = 0;
	virtual void TickScene() = 0;
	virtual void RenderScene() = 0;

protected:
	std::shared_ptr<SceneManager> Scene;
	std::shared_ptr<RenderPipelineBase> RenderPipeline;
};


