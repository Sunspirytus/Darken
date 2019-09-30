#pragma once
#include "TypeDefine.h"
#include "World.h"
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

	std::shared_ptr<World> GetScene();
	
protected:
	Vector2i ViewPortSize;
	std::shared_ptr<RenderPipelineBase> RenderPipeline;
	std::shared_ptr<World> Scene;
	
	virtual void OnSizeChange() = 0;
};


