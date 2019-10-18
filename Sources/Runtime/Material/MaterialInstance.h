#pragma once

#include "Material.h" 
#include "Texture.h" 
#include <map> 
#include <unordered_map>

class MaterialInstanceBase : public PropertyBase
{
public:
	MaterialInstanceBase();
	MaterialInstanceBase(const String& path, const String& parentName );
	~MaterialInstanceBase();

	String GetName();

protected:
	String Path;
	String ParentName;
};

struct BasicUniformData
{
	String Name;
	void* Data;
};

struct TextureUniformData
{
	String Name;
	std::shared_ptr<Texture> Tex;
	uint32* IDPtr;
};

struct UniformDataInBlock
{
	String BlockName;
	String UniformName;
	void* Data;
};

class MaterialInstance : public MaterialInstanceBase
{
public:
	MaterialInstance(const String& name, std::shared_ptr<Material> parentMaterial);
	~MaterialInstance();

	void SetParent(std::shared_ptr<Material> parentMaterial);
	//void ChangedParentDynamic(std::shared_ptr<Material> parentMaterial);
	std::shared_ptr<Material> GetParent();
	static int32 GetID(const String &UniformName);

	template<class T>
	void SetUniform(int32 UniformID, const T &data)
	{
		if (BasicUniformID_DataPtrMap.find(UniformID) == BasicUniformID_DataPtrMap.end()) return;
		memcpy(BasicUniformID_DataPtrMap[UniformID]->Data, &data, sizeof(T));
	}

	template<class T>
	void SetUniform(const String &UniformName, const T &data)
	{
		std::hash<String> hs;
		int32 ID = (int32) hs(UniformName);
		if (BasicUniformID_DataPtrMap.find(ID) == BasicUniformID_DataPtrMap.end()) return;
		memcpy(BasicUniformID_DataPtrMap[ID]->Data, &data, sizeof(T));
	}

	template<class T>
	void SetUniformArray(int32 UniformID, T * data, int32 count)
	{
		if (BasicUniformID_DataPtrMap.find(UniformID) == BasicUniformID_DataPtrMap.end()) return;
		memcpy(BasicUniformID_DataPtrMap[UniformID]->Data, data, sizeof(T) * count);
	}

	template<class T>
	void SetUniformArray(const String &UniformName, T * data, int32 count)
	{
		std::hash<String> hs;
		int32 ID = (int32)hs(UniformName);
		if (BasicUniformID_DataPtrMap.find(ID) == BasicUniformID_DataPtrMap.end()) return;
		memcpy(BasicUniformID_DataPtrMap[ID]->Data, data, sizeof(T) * count);
	}

	template<class T>
	void SetBlockUniform(int32 BlockID, int32 UniformID, const T &data)
	{
		if (BlockID_UniformID_DataPtrMap.find(BlockID) == BlockID_UniformID_DataPtrMap.end()) return;
		if (BlockID_UniformID_DataPtrMap[BlockID].find(UniformID) == BlockID_UniformID_DataPtrMap[BlockID].end()) return;
		memcpy(BlockID_UniformID_DataPtrMap[BlockID][UniformID]->Data, &data, sizeof(T));
		MarkDirty(BlockID_BufferPtrMap.find(BlockID)->second->UniformPtr);
	}

	template<class T>
	void SetBlockUniform(const String &BlockName, const String &UniformName, const T &data)
	{
		std::hash<String> hs;
		int32 BlockID = (int32)hs(BlockName);
		if (BlockID_UniformID_DataPtrMap.find(BlockID) == BlockID_UniformID_DataPtrMap.end()) return;
		int32 UniformID = (int32)hs(UniformName);
		if (BlockID_UniformID_DataPtrMap[BlockID].find(UniformID) == BlockID_UniformID_DataPtrMap[BlockID].end()) return;
		memcpy(BlockID_UniformID_DataPtrMap[BlockID][UniformID]->Data, &data, sizeof(T));
		MarkDirty(BlockID_BufferPtrMap.find(BlockID)->second->UniformPtr);
	}

	template<class T>
	void SetBlockUniformArray(int32 BlockID, int32 UniformID, T * data, int32 count)
	{
		if (BlockID_UniformID_DataPtrMap.find(BlockID) == BlockID_UniformID_DataPtrMap.end()) return;
		if (BlockID_UniformID_DataPtrMap[BlockID].find(UniformID) == BlockID_UniformID_DataPtrMap[BlockID].end()) return;
		memcpy(BlockID_UniformID_DataPtrMap[BlockID][UniformID]->Data, data, sizeof(T) * count);
		MarkDirty(BlockID_BufferPtrMap.find(BlockID)->second->UniformPtr);
	}

	template<class T>
	void SetBlockUniformArray(const String &BlockName, const String &UniformName, T * data, int32 count)
	{
		std::hash<String> hs;
		int32 BlockID = hs(BlockName);
		if (BlockID_UniformID_DataPtrMap.find(BlockID) == BlockID_UniformID_DataPtrMap.end()) return;
		int32 UniformID = hs(UniformName);
		if (BlockID_UniformID_DataPtrMap[BlockID].find(UniformID) == BlockID_UniformID_DataPtrMap[BlockID].end()) return;
		memcpy(BlockID_UniformID_DataPtrMap[BlockID][UniformID]->Data, data, sizeof(T) * count);
		MarkDirty(BlockID_BufferPtrMap.find(BlockID)->second->UniformPtr);
	}

	void SetTextureID(int32 UniformID, uint32 data)
	{
		if (TextureUniformID_DataPtrMap.find(UniformID) == TextureUniformID_DataPtrMap.end()) return;
		memcpy(TextureUniformID_DataPtrMap[UniformID]->IDPtr, &data, sizeof(uint32));
	}

	void SetTextureID(const String& UniformName, uint32 data)
	{
		std::hash<String> hs;
		int32 ID = (int32)hs(UniformName);
		if (TextureUniformID_DataPtrMap.find(ID) == TextureUniformID_DataPtrMap.end()) return;
		memcpy(TextureUniformID_DataPtrMap[ID]->IDPtr, &data, sizeof(uint32));
	}

	void SetTexture(int32 UniformID, std::shared_ptr<Texture> Tex)
	{
		if (TextureUniformID_DataPtrMap.find(UniformID) == TextureUniformID_DataPtrMap.end()) return;
		TextureUniformID_DataPtrMap[UniformID]->Tex = Tex;
		memcpy(TextureUniformID_DataPtrMap[UniformID]->IDPtr, &Tex->GPUId, sizeof(uint32));
	}

	void SetTexture(const String &UniformName, std::shared_ptr<Texture> Tex)
	{
		std::hash<String> hs;
		int32 ID = (int32)hs(UniformName);
		if (TextureUniformID_DataPtrMap.find(ID) == TextureUniformID_DataPtrMap.end()) return;
		TextureUniformID_DataPtrMap[ID]->Tex = Tex;
		memcpy(TextureUniformID_DataPtrMap[ID]->IDPtr, &Tex->GPUId, sizeof(uint32));
	}

	virtual void Save(String* Data);

private:
	std::map<int32, std::shared_ptr<BasicUniformData>> BasicUniformID_DataPtrMap;
	std::map<int32, std::shared_ptr<TextureUniformData>> TextureUniformID_DataPtrMap;
	std::map<int32, std::map<int32, std::shared_ptr<UniformDataInBlock>>> BlockID_UniformID_DataPtrMap;
	std::map<int32, std::shared_ptr<UniformProxyBlock>> BlockID_BufferPtrMap;
	std::shared_ptr<Material> ParentMaterial;
	void MarkDirty(std::shared_ptr<UniformItem_Block> BlockPtr);

	enum class DataGroup
	{
		UNIFORM,
		TEXTURE,
		UNIFORMBUFFER
	};

	void WriteInstanceData(String* Data, DataGroup type);
};

