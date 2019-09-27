#pragma once

#include "Transform.h"
#include "SurroundBox.h"
#include "CommonFunctions.h"
#include "PropertyBase.h"
#include <map>

enum ObjectType
{
	Default = 0xFFFFFFFF,
	StaticMeshActor = 1 << 1,
	DynamicMeshActor = 1 << 2,
	CameraActor = 1 << 3,
	LandscapeMeshActor = 1 << 4,
	AbstractActor = 1 << 5,
	NavigationSystemActor = 1 << 6,
};

class ObjectBase : public PropertyBase
{
public:
	ObjectBase();
	ObjectBase(const String& path, ObjectType type);
	~ObjectBase();

	String Path;
	ObjectType Type;
};

class Object : public ObjectBase
{
public:
	Object();
	Object(const String& name, ObjectType type);
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
};


