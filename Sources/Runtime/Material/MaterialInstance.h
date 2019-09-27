#pragma once

#include "Material.h"
#include <map>
#include <unordered_map>

class MaterialInstanceBase : public PropertyBase
{
public:
	MaterialInstanceBase();
	MaterialInstanceBase(const String& path, const String& parentName );
	~MaterialInstanceBase();

private:
	String Path;
	String ParentName;
};

class MaterialInstance : public MaterialInstanceBase
{
public:
	MaterialInstance(const String& name, std::shared_ptr<Material> parentMaterial);
	~MaterialInstance();

	void SetParent(std::shared_ptr<Material> parentMaterial);
	void ChangedParentDynamic(std::shared_ptr<Material> parentMaterial);
	std::shared_ptr<Material> GetParent();
	static int32 GetID(const String &UniformName);

	template<class T>
	void SetUniform(int32 UniformID, const T &data)
	{
		if (BasicUniformID_PtrMap.find(UniformID) == BasicUniformID_PtrMap.end()) return;
		memcpy(BasicUniformID_PtrMap[UniformID], &data, sizeof(T));
	}

	template<class T>
	void SetUniform(const String &UniformName, const T &data)
	{
		std::hash<String> hs;
		int32 ID = (int32) hs(UniformName);
		if (BasicUniformID_PtrMap.find(ID) == BasicUniformID_PtrMap.end()) return;
		memcpy(BasicUniformID_PtrMap[ID], &data, sizeof(T));
	}

	template<class T>
	void SetUniformArray(int32 UniformID, T * data, int32 count)
	{
		if (BasicUniformID_PtrMap.find(UniformID) == BasicUniformID_PtrMap.end()) return;
		memcpy(BasicUniformID_PtrMap[UniformID], data, sizeof(T) * count);
	}

	template<class T>
	void SetUniformArray(const String &UniformName, T * data, int32 count)
	{
		std::hash<String> hs;
		int32 ID = (int32)hs(UniformName);
		if (BasicUniformID_PtrMap.find(ID) == BasicUniformID_PtrMap.end()) return;
		memcpy(BasicUniformID_PtrMap[ID], data, sizeof(T) * count);
	}

	template<class T>
	void SetBlockUniform(int32 BlockID, int32 UniformID, const T &data)
	{
		if (BlockID_UniformID_DataPtrMap.find(BlockID) == BlockID_UniformID_DataPtrMap.end()) return;
		if (BlockID_UniformID_DataPtrMap[BlockID].find(UniformID) == BlockID_UniformID_DataPtrMap[BlockID].end()) return;
		memcpy(BlockID_UniformID_DataPtrMap[BlockID][UniformID], &data, sizeof(T));
		MarkDirty(BlockID);
	}

	template<class T>
	void SetBlockUniform(const String &BlockName, const String &UniformName, const T &data)
	{
		std::hash<String> hs;
		int32 BlockID = (int32)hs(BlockName);
		if (BlockID_UniformID_DataPtrMap.find(BlockID) == BlockID_UniformID_DataPtrMap.end()) return;
		int32 UniformID = (int32)hs(UniformName);
		if (BlockID_UniformID_DataPtrMap[BlockID].find(UniformID) == BlockID_UniformID_DataPtrMap[BlockID].end()) return;
		memcpy(BlockID_UniformID_DataPtrMap[BlockID][UniformID], &data, sizeof(T));
		MarkDirty(BlockID);
	}

	template<class T>
	void SetBlockUniformArray(int32 BlockID, int32 UniformID, T * data, int32 count)
	{
		if (BlockID_UniformID_DataPtrMap.find(BlockID) == BlockID_UniformID_DataPtrMap.end()) return;
		if (BlockID_UniformID_DataPtrMap[BlockID].find(UniformID) == BlockID_UniformID_DataPtrMap[BlockID].end()) return;
		memcpy(BlockID_UniformID_DataPtrMap[BlockID][UniformID], data, sizeof(T) * count);
		MarkDirty(BlockID);
	}

	template<class T>
	void SetBlockUniformArray(const String &BlockName, const String &UniformName, T * data, int32 count)
	{
		std::hash<String> hs;
		int32 BlockID = hs(BlockName);
		if (BlockID_UniformID_DataPtrMap.find(BlockID) == BlockID_UniformID_DataPtrMap.end()) return;
		int32 UniformID = hs(UniformName);
		if (BlockID_UniformID_DataPtrMap[BlockID].find(UniformID) == BlockID_UniformID_DataPtrMap[BlockID].end()) return;
		memcpy(BlockID_UniformID_DataPtrMap[BlockID][UniformID], data, sizeof(T) * count);
		MarkDirty(BlockID);
	}

	void SetTextureID(int32 UniformID, uint32 data)
	{
		if (TextureUniformID_PtrMap.find(UniformID) == TextureUniformID_PtrMap.end()) return;
		memcpy(TextureUniformID_PtrMap[UniformID], &data, sizeof(uint32));
	}

	void SetTextureID(const String &UniformName, uint32 data)
	{
		std::hash<String> hs;
		int32 ID = (int32)hs(UniformName);
		if (TextureUniformID_PtrMap.find(ID) == TextureUniformID_PtrMap.end()) return;
		memcpy(TextureUniformID_PtrMap[ID], &data, sizeof(uint32));
	}

private:
	std::map<int32, void*> BasicUniformID_PtrMap;
	std::map<int32, uint32*> TextureUniformID_PtrMap;
	std::map<int32, std::map<int32, void*>> BlockID_UniformID_DataPtrMap;
	std::shared_ptr<Material> ParentMaterial;
	void MarkDirty(int32 BlockID);
};

