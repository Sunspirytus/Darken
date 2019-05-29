#include "ViewPortBase.h"

ViewPortBase::ViewPortBase()
{
	Scene = std::shared_ptr<SceneManager>(new SceneManager());
}

ViewPortBase::~ViewPortBase()
{
}