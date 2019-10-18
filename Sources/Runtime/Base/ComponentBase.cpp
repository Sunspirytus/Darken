#include "ComponentBase.h"
#include "CommonFunctions.h"

ComponentBase::ComponentBase(ComponentType type)
	:	Type(type)
{
	AddProperty("Component", VariableType::ENUM, &Type, std::map<int32, String>{ {(int32)ComponentType::CPT_Transform, TO_String(CPT_Transform)}});
}

ComponentBase::~ComponentBase()
{
	
}