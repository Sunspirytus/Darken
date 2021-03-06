#pragma once
#include "TypeDefine.h"
#include "World.h"

class RenderPipelineBase
{
public:
	RenderPipelineBase();
	~RenderPipelineBase();

	virtual void Init(std::shared_ptr<World> Scene) = 0;
	virtual void Render(std::shared_ptr<Camera> camera) = 0;
	virtual void RenderTextureSizeChange(Vector2i newSize) = 0;
public:
	std::shared_ptr<World> SceneWaitRender;
};