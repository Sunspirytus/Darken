#include "MaterialManager.h"
#include "EngineRoot.h"
#include "FileIO.h"

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
		String FolderPath;
		switch (belong)
		{
		case Internal:
			FolderPath = DKEngine::GetInstance().GetInternalShaderPath();
			break;
		case UserDefine:
			FolderPath = DKEngine::GetInstance().GetAssetFolderPath();
			break;
		default:
			break;
		}

		for(int32 ShaderIndex = 0; ShaderIndex < (int32) shaderNames.size(); ShaderIndex++)
		{
			shaderNames[ShaderIndex] = FolderPath + shaderNames[ShaderIndex];
		}

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


void MaterialManager::Save()
{
	String FolderPath = DKEngine::GetInstance().GetAssetFolderPath();
	for(std::map<String, std::shared_ptr<Material>>::iterator it =  Materials_User.begin(); it != Materials_User.end(); it++)
	{
		String MaterialPath = it->first;
		std::shared_ptr<Material> Material = it->second;
		String Data;
		Material->Save(&Data);
		FileIO::SaveFile(FolderPath, MaterialPath, FileType::F_Material, Data);
	}
	for (std::map<String, std::shared_ptr<MaterialInstance>>::iterator it = MaterialInsts_User.begin(); it != MaterialInsts_User.end(); it++)
	{
		String MaterialInstPath = it->first;
		std::shared_ptr<MaterialInstance> MaterialInst = it->second;
		String Data;
		MaterialInst->Save(&Data);
		FileIO::SaveFile(FolderPath, MaterialInstPath, FileType::F_MaterialInstance, Data);
	}
}