#include "Object.h"

Object::Object(ObjectProperty property) 
	: bNeedCheckClip(true)
	, bNeedClip(false)
{
	Property = std::shared_ptr<ObjectProperty>(new ObjectProperty(property));
	Transform = std::shared_ptr<TransformComponent>(new TransformComponent());
}

Object::~Object()
{
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