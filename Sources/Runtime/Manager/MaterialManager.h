#pragma once
#include "MaterialInstance.h"

enum class MaterialBelong
{
	Engine,
	UserDefine
};

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	void Save(const String& Name);
	std::shared_ptr<Material> LoadMaterial(const String& Name);
	std::shared_ptr<MaterialInstance> LoadMaterialInstance(const String& Name);

	std::shared_ptr<Material> CreateMaterial(const String& name, std::vector<String> shaderNames, MaterialBelong belong);
	std::shared_ptr<MaterialInstance> CreateMaterialInstance(const String& name, std::shared_ptr<Material> parentMaterial, MaterialBelong belong);

private:
	std::map<String, std::shared_ptr<Material>> Materials_User;
	std::map<String, std::shared_ptr<Material>> Materials_Internal;
	std::map<String, std::shared_ptr<MaterialInstance>> MaterialInsts_User;
	std::map<String, std::shared_ptr<MaterialInstance>> MaterialInsts_Internal;
};

