#include "Object.h"

ObjectBase::ObjectBase()
{
}

ObjectBase::~ObjectBase()
{
}

ObjectBase::ObjectBase(const String& path, ObjectType type)
	: Path(path)
	, Type(type)
{
	AddProperty("Path", STRING, &Path);
	AddProperty("Type", ENUM, &Type, std::map<int32, String>{ {Default, TO_String(Default)}, { StaticMeshActor,TO_String(StaticMeshActor) }, { DynamicMeshActor,TO_String(DynamicMeshActor) },	{ CameraActor,TO_String(CameraActor) },	{ LandscapeMeshActor,TO_String(LandscapeMeshActor) },	{ AbstractActor,TO_String(AbstractActor) },	{ NavigationSystemActor,TO_String(NavigationSystemActor) }});
}

Object::Object()
	: bNeedCheckClip(true)
	, bNeedClip(false)
{
	Transform = std::shared_ptr<TransformComponent>(new TransformComponent());
}

Object::Object(const String& name, ObjectType type)
	:	ObjectBase(name, type)
	,	bNeedCheckClip(true)
	,	bNeedClip(false)
{
	Transform = std::shared_ptr<TransformComponent>(new TransformComponent());
}

Object::~Object()
{
}

void Object::Save(String* Data)
{
	PropertyBase::Save(Data);
	Transform->Save(Data);
}

//void Object::SetProperty(ObjectProperty property)
//{
//	Property = property;
//}

//void Object::Draw()
//{
//
//}

//void Object::CalculateLOD()
//{
//	Vector3f CameraPosWorld = _Scene->GetCamera(CameraIndex::MainCamera)->GetPosition();
//	Vector3f CenterWorld = Transform->GetPosition();
//	LODValue = glm::distance(Vector3f(CenterWorld), CameraPosWorld) / LODFactor - LODThreshold;
//	LODValue = glm::min(LODValue, static_cast<float32>(MaxLOD));
//	CurrentLOD = LODValue < 0 ? 0 : static_cast<int32>(glm::min(static_cast<float32>(MaxLOD), LODValue));
//	LODValue = glm::max(LODValue, 0.0f);
//}