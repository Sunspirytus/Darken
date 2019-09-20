#pragma once

#include "Transform.h"
#include "SurroundBox.h"

enum ObjectType
{
	Default					= 0xFFFFFFFF,
	StaticMesh				= 1 << 1,
	DynamicMesh				= 1 << 2,
	CameraActor				= 1 << 3,
	LandscapeMesh			= 1 << 4,
	AbstractActor			= 1 << 5,
	NavigationSystem		= 1 << 6,
};

class ObjectProperty
{
public:
	ObjectProperty() 
		:	Name("Object")
		,	Type(ObjectType::Default)
	{};
	~ObjectProperty() {};

	std::string Name;
	ObjectType Type;
};

class Object
{
public:
	Object(ObjectProperty property);
	~Object();

	std::shared_ptr<ObjectProperty> GetProperty() { return Property; }
	//void SetProperty(ObjectProperty property);
	std::shared_ptr<TransformComponent> Transform;
	
	bool bNeedCheckClip;
	bool bNeedClip;

	virtual void CheckWhetherNeedClip() = 0;
	virtual void Start() = 0;
	virtual void InternalUpdate() = 0;
	virtual void Update() = 0;
	virtual void FixUpdate() = 0;
	virtual void Draw() = 0;
private:
	std::shared_ptr<ObjectProperty> Property;
	//2
};


