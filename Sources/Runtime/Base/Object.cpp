#include "Object.h"

ObjectBase::ObjectBase()
{
	AddProperty("Path", STRING, &Path);
	AddProperty("Type", ENUM, &Type, std::map<int32, String>{ {Default, TO_String(Default)}, { StaticMeshActor,TO_String(StaticMeshActor) }, { DynamicMeshActor,TO_String(DynamicMeshActor) }, { CameraActor,TO_String(CameraActor) }, { LandscapeMeshActor,TO_String(LandscapeMeshActor) }, { AbstractActor,TO_String(AbstractActor) }, { NavigationSystemActor,TO_String(NavigationSystemActor) }});
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

	PropertyBase::BeginWriteProperty(Data, BaseInfo);
	PropertyBase::Save(Data);
	PropertyBase::EndWriteProperty(Data, BaseInfo);

	PropertyBase::BeginWriteProperty(Data, ComponentInfo);
	Transform->Save(Data);
	PropertyBase::EndWriteProperty(Data, ComponentInfo);

	PropertyBase::FinishWrite(Data);
}

void Object::Load(const String& Data)
{
	String ReserveData = Data;
	String DataWaitProcess;
	BeginReadProperty(&DataWaitProcess, ReserveData, BaseInfo);
	PropertyBase::Load(DataWaitProcess);
	EndReadProperty(&ReserveData, BaseInfo);

	BeginReadProperty(&DataWaitProcess, ReserveData,ComponentInfo);
	Transform->Load(DataWaitProcess);
	EndReadProperty(&ReserveData, ComponentInfo);
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
		return Default;
	}else if(Type == TO_String(StaticMeshActor))
	{
		return StaticMeshActor;
	}
	else if (Type == TO_String(DynamicMeshActor))
	{
		return DynamicMeshActor;
	}
	else if (Type == TO_String(CameraActor))
	{
		return CameraActor;
	}
	else if (Type == TO_String(LandscapeMeshActor))
	{
		return LandscapeMeshActor;
	}
	else if (Type == TO_String(AbstractActor))
	{
		return AbstractActor;
	}
	else if (Type == TO_String(NavigationSystemActor))
	{
		return NavigationSystemActor;
	}

	return Default;
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