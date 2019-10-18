#include "Object.h"

ObjectBase::ObjectBase()
{
	AddProperty("Path", VariableType::STRING, &Path);
	AddProperty("Type", VariableType::ENUM, &Type, std::map<int32, String>{ {(int32)ObjectType::Default, TO_String(Default)}, { (int32)ObjectType::StaticMeshActor,TO_String(StaticMeshActor) }, { (int32)ObjectType::DynamicMeshActor,TO_String(DynamicMeshActor) }, { (int32)ObjectType::CameraActor,TO_String(CameraActor) }, { (int32)ObjectType::LandscapeMeshActor,TO_String(LandscapeMeshActor) }, { (int32)ObjectType::AbstractActor,TO_String(AbstractActor) }, { (int32)ObjectType::NavigationSystemActor,TO_String(NavigationSystemActor) }});
}

ObjectBase::~ObjectBase()
{
}

Object::Object()
	: ObjectBase()
	, bNeedCheckClip(true)
	, bNeedClip(false)
{
	Transform = std::shared_ptr<TransformComponent>(new TransformComponent());
}

Object::~Object()
{
}

void Object::Save(String* Data)
{
	PropertyBase::PrepareToWrite(Data);

	PropertyBase::BeginWriteProperty(Data, PropertyType::BaseInfo);
	PropertyBase::Save(Data);
	PropertyBase::EndWriteProperty(Data, PropertyType::BaseInfo);

	PropertyBase::BeginWriteProperty(Data, PropertyType::ComponentInfo);
	Transform->Save(Data);
	PropertyBase::EndWriteProperty(Data, PropertyType::ComponentInfo);

	PropertyBase::FinishWrite(Data);
}

void Object::Load(const String& Data)
{
	String ReserveData = Data;
	String DataWaitProcess;
	BeginReadProperty(&DataWaitProcess, ReserveData, PropertyType::BaseInfo);
	PropertyBase::Load(DataWaitProcess);
	EndReadProperty(&ReserveData, PropertyType::BaseInfo);

	BeginReadProperty(&DataWaitProcess, ReserveData, PropertyType::ComponentInfo);
	Transform->Load(DataWaitProcess);
	EndReadProperty(&ReserveData, PropertyType::ComponentInfo);
}

ObjectType Object::GetType(const String& ObjectInfo)
{
	String BeginTag = "Type:";
	String EndTag = ";";
	int32 Begin = ObjectInfo.find(BeginTag);
	int32 End = ObjectInfo.find(EndTag, Begin);
	String Type = ObjectInfo.substr(Begin + BeginTag.length(), End - Begin - BeginTag.length());
	if(Type == TO_String(Default))
	{
		return ObjectType::Default;
	}else if(Type == TO_String(StaticMeshActor))
	{
		return ObjectType::StaticMeshActor;
	}
	else if (Type == TO_String(DynamicMeshActor))
	{
		return ObjectType::DynamicMeshActor;
	}
	else if (Type == TO_String(CameraActor))
	{
		return ObjectType::CameraActor;
	}
	else if (Type == TO_String(LandscapeMeshActor))
	{
		return ObjectType::LandscapeMeshActor;
	}
	else if (Type == TO_String(AbstractActor))
	{
		return ObjectType::AbstractActor;
	}
	else if (Type == TO_String(NavigationSystemActor))
	{
		return ObjectType::NavigationSystemActor;
	}

	return ObjectType::Default;
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