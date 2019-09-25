#pragma once

#include "Transform.h"
#include "SurroundBox.h"
#include "CommonFunctions.h"
#include "ObjectProperty.h"

enum ObjectType
{
	Default = 0xFFFFFFFF,
	StaticMesh = 1 << 1,
	DynamicMesh = 1 << 2,
	CameraActor = 1 << 3,
	LandscapeMesh = 1 << 4,
	AbstractActor = 1 << 5,
	NavigationSystem = 1 << 6,
};

class Object : public PropertyBase
{
public:
	Object();
	~Object();

	std::shared_ptr<TransformComponent> Transform;
	
	bool bNeedCheckClip;
	bool bNeedClip;

	virtual void CheckWhetherNeedClip() = 0;
	virtual void Start() = 0;
	virtual void InternalUpdate() = 0;
	virtual void Update() = 0;
	virtual void FixUpdate() = 0;
	virtual void Draw() = 0;
	virtual void Save(String* Data);

private:

	String Name;
	ObjectType Type;
	//2
};


