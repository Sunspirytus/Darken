#pragma once

#include "TypeDefine.h"
#include "PropertyBase.h"

enum ComponentType
{
	CPT_Transform
};

class ComponentBase : public PropertyBase
{
public:
	ComponentBase(ComponentType type);
	~ComponentBase();

	ComponentType Type;
};
