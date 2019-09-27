#include "MaterialManager.h"

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
}

std::shared_ptr<Material> MaterialManager::CreateMaterial(const String& name, std::vector<String> shaderNames, MaterialBelong belong)
{
	std::shared_ptr<Material> Mat = nullptr;
	std::map<String, std::shared_ptr<Material>>::iterator it;

	switch (belong)
	{
	case Internal:
		if(Materials_Internal.find(name) != Materials_Internal.end())
		{
			Mat = it->second;
		}
		break;
	case UserDefine:
		if (Materials_User.find(name) != Materials_User.end())
		{
			Mat = it->second;
		}
		break;
	default:
		break;
	}

	if(!Mat)
	{
		Mat = std::shared_ptr<Material>(new Material(name, shaderNames));
		switch (belong)
		{
		case Internal:
			Materials_Internal.insert(std::pair<String, std::shared_ptr<Material>>(name, Mat));
			break;
		case UserDefine:
			Materials_User.insert(std::pair<String, std::shared_ptr<Material>>(name, Mat));
			break;
		default:
			break;
		}
	}
	return Mat;
}

std::shared_ptr<MaterialInstance> MaterialManager::CreateMaterialInstance(const String& name, std::shared_ptr<Material> parentMaterial, MaterialBelong belong)
{
	std::shared_ptr<MaterialInstance> MatInst = nullptr;
	std::map<String, std::shared_ptr<MaterialInstance>>::iterator it;

	switch (belong)
	{
	case Internal:
		if (MaterialInsts_Internal.find(name) != MaterialInsts_Internal.end())
		{
			MatInst = it->second;
		}
		break;
	case UserDefine:
		if (MaterialInsts_User.find(name) != MaterialInsts_User.end())
		{
			MatInst = it->second;
		}
		break;
	default:
		break;
	}

	if(!MatInst)
	{
		MatInst = std::shared_ptr<MaterialInstance>(new MaterialInstance(name, parentMaterial));
		switch (belong)
		{
		case Internal:
			MaterialInsts_Internal.insert(std::pair<String, std::shared_ptr<MaterialInstance>>(name, MatInst));
			break;
		case UserDefine:
			MaterialInsts_User.insert(std::pair<String, std::shared_ptr<MaterialInstance>>(name, MatInst));
			break;
		default:
			break;
		}
	}
	return MatInst;
}