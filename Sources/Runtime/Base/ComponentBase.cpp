#include "ComponentBase.h"

ComponentBase::ComponentBase(ComponentType type)
	:	Type(type)
{
	AddProperty("Component", ENUM, &Type, std::map<int32, String>{ {CPT_Transform, TO_String(CPT_Transform)}});
}

ComponentBase::~ComponentBase()
{
	
}