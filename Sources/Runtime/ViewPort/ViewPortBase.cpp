#include "ViewPortBase.h"

ViewPortBase::ViewPortBase()
	: ViewPortSize(Vector2i(50, 50))
{
	Scene = std::shared_ptr<SceneManager>(new SceneManager());
}

ViewPortBase::~ViewPortBase()
{
}
void ViewPortBase::SizeChange(Vector2i newSize)
{
	ViewPortSize = newSize;
	OnSizeChange();
}