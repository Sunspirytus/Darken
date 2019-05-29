#pragma once
#include "TypeDefine.h"
#include "SceneManager.h"

class RenderPipelineBase
{
public:
	RenderPipelineBase();
	~RenderPipelineBase();

	virtual void Init(std::shared_ptr<SceneManager> Scene) = 0;
	virtual void Render() = 0;

public:
	std::shared_ptr<SceneManager> SceneWaitRender;
};