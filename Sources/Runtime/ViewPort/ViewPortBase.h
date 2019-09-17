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
	virtual void SizeChange(Vector2i newSize) final;
	std::shared_ptr<SceneManager> Scene;
protected:
	Vector2i ViewPortSize;
	std::shared_ptr<RenderPipelineBase> RenderPipeline;
	
	virtual void OnSizeChange() = 0;
};


