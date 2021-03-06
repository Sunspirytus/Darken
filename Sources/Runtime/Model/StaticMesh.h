#pragma once


#include "Object.h"
#include "MaterialInstance.h"
#include "Animation.h"
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#include <map>

struct ModelNode
{
	Mat4f ModelMatrix;
	int32 NumFaces;
	uint32 VBO;
	uint32 IBO;
	uint32 VAO;
	IndexSizeType IBOIndexSizeType;
	SphereBox SurroundSphereBox;
	RectBox SurroundRectBox;
	//Animation AnimationSys;
	bool bNeedClip;
	ModelNode(){};
};

struct ModelMesh
{
	int32 NumFaces;
	uint32 VAO;
	uint32 VBO;
	uint32 IBO;
	IndexSizeType IBOIndexSizeType;
	SphereBox SurroundSphereBox;
	RectBox SurroundRectBox;
	aiMesh* AssimpMeshPtr;
	ModelMesh(){};
};

class StaticMeshBase : public Object
{
public:
	StaticMeshBase();
	~StaticMeshBase();

	void SetMaterialName(const String& name);

	virtual void Save(String* Data);
	virtual void Load(const String& Data);

protected:
	String MeshPath;
	String MaterialName;
};

class StaticMesh : public StaticMeshBase
{
public:
	StaticMesh();
	StaticMesh(const String& fileName);
	~StaticMesh();

	void LoadModelFromAsset(String fileName);
	void BindMaterial(std::shared_ptr<MaterialInstance> shadowDepthMaterial, std::shared_ptr<MaterialInstance> lightingMaterial);
	void BindShadowDepthMaterial();
	void BindLightingMaterial();
	void SetAnimationPlaySpeed(float32 speed);

	virtual void CheckWhetherNeedClip() final;
	virtual void Draw() final;
	virtual void Start();
	virtual void InternalUpdate() final;
	virtual void Update();
	virtual void FixUpdate();
	virtual void Save(String* Data);
	virtual void Load(const String& Data);

	void UpdatePreFrameModelMatrix();

	std::shared_ptr<MaterialInstance> GetRenderMaterial();
	std::vector<std::shared_ptr<ModelNode>>& GetModelNodeGroup();

protected:

	struct MaterialData
	{
		int32 ModelBlockID;
		int32 ViewBlockID;
		int32 ModelMatrixID;
		int32 ModelMatrix_ITID;
		int32 ModelMatrix_IT_PreID;
		int32 ModelMatrix_PreID;
		int32 ViewMatrixID;
		int32 ViewMatrix_PreID;
		int32 ProjectMatrixID;
		int32 ProjectMatrix_PreID;
		int32 ViewPositionID;

		MaterialData()
		{
			ModelBlockID = MaterialInstance::GetID("Model");
			ViewBlockID = MaterialInstance::GetID("View");
			ModelMatrixID = MaterialInstance::GetID("ModelMatrix");
			ModelMatrix_PreID = MaterialInstance::GetID("ModelMatrix_PreFrame");
			ModelMatrix_ITID = MaterialInstance::GetID("ModelMatrix_IT");
			ModelMatrix_IT_PreID = MaterialInstance::GetID("ModelMatrix_IT_PreFrame");
			ViewMatrixID = MaterialInstance::GetID("ViewMatrix");
			ViewMatrix_PreID = MaterialInstance::GetID("ViewMatrix_PreFrame");
			ProjectMatrixID = MaterialInstance::GetID("ProjectMatrix");
			ProjectMatrix_PreID = MaterialInstance::GetID("ProjectMatrix_PreFrame");
			ViewPositionID = MaterialInstance::GetID("ViewPosition");
		}
	};

	std::shared_ptr<MaterialInstance> RenderMaterial;
	std::shared_ptr<MaterialInstance> ShadowDepthMaterial;
	std::shared_ptr<MaterialInstance> LightingMaterial;
	std::shared_ptr<MaterialData> MaterialInstData;
	std::vector<std::shared_ptr<ModelNode>> RenderGroup;
	std::vector<std::shared_ptr<ModelNode>> NodeGroup;
	std::vector<std::shared_ptr<ModelMesh>> MeshGroup;
	void LoadFromAssetWithAssimp(String folderPath, String fileName);
	void AddNode(const aiScene* scene, aiNode* node, Mat4f parentTransformMatrix);
	Assimp::Importer import;
};