#include "Object.h"

extern int32 TabCount;

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
	Data->append("{\n");

	BeginWriteProperty(Data, ObjectInfo);
	PropertyBase::Save(Data);
	EndWriteProperty(Data, ObjectInfo);

	BeginWriteProperty(Data, ComponentInfo);
	Transform->Save(Data);
	EndWriteProperty(Data, ComponentInfo);

	Data->append("}\n");
}

void Object::Load(const String& Data)
{
	String PartData;
	BeginReadProperty(&PartData, Data, ObjectInfo);
	PropertyBase::Load(PartData);
}

void Object::BeginWriteProperty(String* Data, PropertyType type)
{
	switch (type)
	{
	case ObjectInfo:
		TabCount = ObjectInfo;
		break;
	case ComponentInfo:
	{
		TabCount = ComponentInfo - 1;
		AddTab(Data);
		Data->append("[\n");
		TabCount = ComponentInfo;
		break;
	}
	default:
		break;
	}
}

void Object::EndWriteProperty(String* Data, PropertyType type)
{
	switch (type)
	{
	case ObjectInfo:
		TabCount = 0;
		break;
	case ComponentInfo:
	{
		TabCount = ComponentInfo - 1;
		AddTab(Data);
		Data->append("]\n");
		TabCount = 0;
		break;
	}
	default:
		break;
	}
}


void Object::BeginReadProperty(String* PartData, const String& Data, PropertyType type)
{
	switch (type)
	{
	case ObjectInfo:
	{
		int32 BeginPos, EndPos = -1;
		String BeginTag = "{\n";
		String EndTag = "[\n";
		BeginPos = Data.find(BeginTag);
		EndPos = Data.find(EndTag);
		*PartData = Data.substr(BeginPos + BeginTag.length(), EndPos - BeginPos - BeginTag.length());
		break; 
	}
	case ComponentInfo:
		break;
	default:
		break;
	}
}

void Object::EndReadProperty(String* PartData, const String& Data, PropertyType type)
{

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