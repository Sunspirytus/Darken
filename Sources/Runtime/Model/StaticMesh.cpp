#include "StaticMesh.h"
#include "EngineRoot.h"

StaticMeshBase::StaticMeshBase()
{
}

StaticMeshBase::StaticMeshBase(const String& path)
	:	Object(path, StaticMeshActor)
{
}

StaticMeshBase::~StaticMeshBase()
{
}

StaticMesh::StaticMesh()
{
}

StaticMesh::StaticMesh(const String& fileName, Vector3f scale, bool bPackToOneMesh)
	:	StaticMeshBase(fileName)
{
	LoadModelFromAsset(fileName, scale, bPackToOneMesh);
}

StaticMesh::~StaticMesh()
{	
	for (uint32 Index = 0; Index < MeshGroup.size(); Index++)
	{
		//glDeleteBuffers(1, &MeshGroup[Index]->VAO);
		//glDeleteBuffers(1, &MeshGroup[Index]->VBO);
		//glDeleteBuffers(1, &MeshGroup[Index]->IBO);
	}
}

void StaticMesh::LoadModelFromAsset(String fileName, Vector3f scale, bool bPackToOneMesh)
{
	
	LoadFromAssetWithAssimp(DKEngine::GetInstance().GetAssetFolderPath(), fileName, scale, bPackToOneMesh);
	
}

void StaticMesh::LoadFromAssetWithAssimp(String folderPath, String fileName, Vector3f scale, bool bPackToOneMesh)
{
	String file = folderPath + fileName;
	const aiScene* scene = import.ReadFile(file, aiProcess_Triangulate);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Error Load" << fileName << std::endl;
		return;
	}
	
	AddNode(scene, scene->mRootNode, Mat4f(1.0));
	RenderGroup = NodeGroup;
}

void StaticMesh::AddNode(const aiScene* scene, aiNode* node, Mat4f parentTransformMatrix)
{
	if (node == nullptr) return;
	if (node->mMeshes == nullptr && node->mNumChildren != 0)
	{
		Mat4f TransformMatrix;
		memcpy(&TransformMatrix[0][0], &node->mTransformation.a1, sizeof(float32) * 16);
		TransformMatrix = Math::Transpose(TransformMatrix);
		TransformMatrix = TransformMatrix * parentTransformMatrix;

		for (uint32 ChildIndex = 0; ChildIndex < node->mNumChildren; ChildIndex++)
		{
			AddNode(scene, node->mChildren[ChildIndex], TransformMatrix);
		}
	}
	else
	{
		Mat4f TransformMatrix;
		memcpy(&TransformMatrix[0][0], &node->mTransformation.a1, sizeof(float32) * 16);
		TransformMatrix = Math::Transpose(TransformMatrix);
		TransformMatrix = TransformMatrix * parentTransformMatrix;

		for (uint32 MeshIndex = 0; MeshIndex < node->mNumMeshes; MeshIndex++)
		{
			aiMesh * AssimpMesh = scene->mMeshes[node->mMeshes[MeshIndex]];
			bool AlreadyHaveMesh = false;
			for (uint32 MeshIndex = 0; MeshIndex < MeshGroup.size(); MeshIndex++)
			{
				if (AssimpMesh == MeshGroup[MeshIndex]->AssimpMeshPtr)
				{
					AlreadyHaveMesh = true;
					break;
				}
			}

			if (AlreadyHaveMesh)
			{
				std::shared_ptr<ModelNode> mNode = std::shared_ptr<ModelNode>(new ModelNode());
				mNode->VAO = MeshGroup[MeshIndex]->VAO;
				mNode->VBO = MeshGroup[MeshIndex]->VBO;
				mNode->IBO = MeshGroup[MeshIndex]->IBO;
				mNode->IBOIndexSizeType = MeshGroup[MeshIndex]->IBOIndexSizeType;
				mNode->NumFaces = MeshGroup[MeshIndex]->NumFaces;
				mNode->SurroundSphereBox = MeshGroup[MeshIndex]->SurroundSphereBox;
				mNode->SurroundRectBox = MeshGroup[MeshIndex]->SurroundRectBox;
				mNode->ModelMatrix = TransformMatrix;
				NodeGroup.push_back(mNode);
			} 
			else
			{
				std::shared_ptr<ModelMesh> mMesh = std::shared_ptr<ModelMesh>(new ModelMesh());
				MeshGroup.push_back(mMesh);
					
				int32 NumFaces = AssimpMesh->mNumFaces;
				IndexSizeType IndexType = IndexSizeType::Index32Bits;
				uint32 * IndexData = new uint32[NumFaces * 3];

				for (int32 FaceIndex = 0; FaceIndex < NumFaces; FaceIndex++)
				{
					IndexData[FaceIndex * 3 + 0] = AssimpMesh->mFaces[FaceIndex].mIndices[0];
					IndexData[FaceIndex * 3 + 1] = AssimpMesh->mFaces[FaceIndex].mIndices[1];
					IndexData[FaceIndex * 3 + 2] = AssimpMesh->mFaces[FaceIndex].mIndices[2];
				}
				
				uint32 IBO;
				glGenBuffers(1, &IBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumFaces * 3 * sizeof(uint16) * (IndexType + 1), IndexData, GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				delete[] IndexData;

				int32 NumVertex = AssimpMesh->mNumVertices;

				uint32 VAO;
				glGenVertexArrays(1, &VAO);
				glBindVertexArray(VAO);

				int32 DataWidth = 0;
				if (AssimpMesh->HasPositions())
				{
					DataWidth += sizeof(float32) * 3;

					Vector3f * PositionData = new Vector3f[NumVertex];
					memcpy(PositionData, &AssimpMesh->mVertices[0].x, sizeof(Vector3f) * NumVertex);
					for (int32 VertexID = 0; VertexID < NumVertex; VertexID++)
					{
						Vector3f Position = Vector3f(AssimpMesh->mVertices[VertexID].x, AssimpMesh->mVertices[VertexID].y, AssimpMesh->mVertices[VertexID].z);
						Vector4f LocalPosition = TransformMatrix * Vector4f(Position, 1.0);
						LocalPosition.y = -LocalPosition.y;
						PositionData[VertexID] = Vector3f(LocalPosition);
						mMesh->SurroundSphereBox += Vector3f(LocalPosition);
						mMesh->SurroundRectBox += Vector3f(LocalPosition);
					}
					
					uint32 PositionBuffer;
					glGenBuffers(1, &PositionBuffer);
					glBindBuffer(GL_ARRAY_BUFFER, PositionBuffer);
					glBufferData(GL_ARRAY_BUFFER, NumVertex * 3 * sizeof(float32), PositionData, GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);	
					glBindVertexBuffer(0, PositionBuffer, 0, 3 * sizeof(float32));
					glEnableVertexAttribArray(0);
					glVertexAttribBinding(0, 0); 
					glVertexAttribFormat(0, 3, GL_FLOAT, false, 0);

					delete[] PositionData;
				}
				if (AssimpMesh->HasNormals())
				{
					Vector3f * NormalData = new Vector3f[NumVertex];
					memcpy(NormalData, &AssimpMesh->mNormals[0].x, sizeof(Vector3f) * NumVertex);

					Mat4f TransformMatrix_IT = Math::Inverse(Math::Transpose(TransformMatrix));
					for (int32 VertexID = 0; VertexID < NumVertex; VertexID++)
					{
						Vector3f Normal = Vector3f(AssimpMesh->mNormals[VertexID].x, -AssimpMesh->mNormals[VertexID].y, AssimpMesh->mNormals[VertexID].z);
						NormalData[VertexID] = Math::Normalize(Mat3f(TransformMatrix_IT) * Normal);
					}

					DataWidth += sizeof(float32) * 3;
					uint32 NormalBuffer;
					glGenBuffers(1, &NormalBuffer);
					glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
					glBufferData(GL_ARRAY_BUFFER, NumVertex * 3 * sizeof(float32), NormalData, GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexBuffer(1, NormalBuffer, 0, 3 * sizeof(float32));
					glEnableVertexAttribArray(1);
					glVertexAttribBinding(1, 1);
					glVertexAttribFormat(1, 3, GL_FLOAT, false, 0);

					delete[] NormalData;
				}
				if (AssimpMesh->HasTangentsAndBitangents())
				{
					DataWidth += sizeof(float32) * 6;
					
					Vector3f * TangentData = new Vector3f[NumVertex];
					memcpy(TangentData, &AssimpMesh->mTangents[0].x, sizeof(Vector3f) * NumVertex);

					Mat4f TransformMatrix_IT = Math::Inverse(Math::Transpose(TransformMatrix));
					for (int32 VertexID = 0; VertexID < NumVertex; VertexID++)
					{
						Vector3f Tangent = Vector3f(AssimpMesh->mTangents[VertexID].x, -AssimpMesh->mTangents[VertexID].y, AssimpMesh->mTangents[VertexID].z);
						TangentData[VertexID] = Math::Normalize(Mat3f(TransformMatrix_IT) * Tangent);
					}

					uint32 TangentsBuffer;
					glGenBuffers(1, &TangentsBuffer);
					glBindBuffer(GL_ARRAY_BUFFER, TangentsBuffer);
					glBufferData(GL_ARRAY_BUFFER, NumVertex * 3 * sizeof(float32), TangentData, GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexBuffer(2, TangentsBuffer, 0, 3 * sizeof(float32));
					glEnableVertexAttribArray(2);
					glVertexAttribBinding(2, 2);
					glVertexAttribFormat(2, 3, GL_FLOAT, false, 0);

					delete[] TangentData;

					Vector3f * BitangentData = new Vector3f[NumVertex];
					memcpy(BitangentData, &AssimpMesh->mBitangents[0].x, sizeof(Vector3f) * NumVertex);

					for (int32 VertexID = 0; VertexID < NumVertex; VertexID++)
					{
						Vector3f Bitangent = Vector3f(-AssimpMesh->mBitangents[VertexID].x, AssimpMesh->mBitangents[VertexID].y, -AssimpMesh->mBitangents[VertexID].z);
						BitangentData[VertexID] = Math::Normalize(Mat3f(TransformMatrix_IT) * Bitangent);
					}

					uint32 BitangentsBuffer;
					glGenBuffers(1, &BitangentsBuffer);
					glBindBuffer(GL_ARRAY_BUFFER, BitangentsBuffer);
					glBufferData(GL_ARRAY_BUFFER, NumVertex * 3 * sizeof(float32), BitangentData, GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexBuffer(3, BitangentsBuffer, 0, 3 * sizeof(float32));
					glEnableVertexAttribArray(3);
					glVertexAttribBinding(3, 3);
					glVertexAttribFormat(3, 3, GL_FLOAT, false, 0);

					delete[] BitangentData;
				}
				for (int32 UVindex = 0; UVindex < 3; UVindex++)
				{
					if (AssimpMesh->mNumUVComponents[UVindex] != 0)
					{
						if (AssimpMesh->mNumUVComponents[UVindex] != 2) assert("UVChnenel data size must be 2" == "0");
						DataWidth += sizeof(float32) * 2;

						float32* Data = new float32[NumVertex * 2];
						for (int32 VertexIndex = 0; VertexIndex < NumVertex; VertexIndex++)
						{
							Data[VertexIndex * 2] = AssimpMesh->mTextureCoords[UVindex][VertexIndex].x;
							Data[VertexIndex * 2 + 1] = AssimpMesh->mTextureCoords[UVindex][VertexIndex].y;
						}

						uint32 UVBuffer;
						glGenBuffers(1, &UVBuffer);
						glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
						glBufferData(GL_ARRAY_BUFFER, NumVertex * 2 * sizeof(float32), Data, GL_STATIC_DRAW);
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						glBindVertexBuffer(4 + UVindex, UVBuffer, 0, 2 * sizeof(float32));
						glEnableVertexAttribArray(4 + UVindex);
						glVertexAttribBinding(4 + UVindex, 4 + UVindex);
						glVertexAttribFormat(4 + UVindex, 2, GL_FLOAT, false, 0);

						delete[] Data;
					}
					else
					{
						float32* Data = new float32[NumVertex * 2];
						for (int32 VertexIndex = 0; VertexIndex < NumVertex; VertexIndex++)
						{
							Data[VertexIndex * 2] = 0.0f;
							Data[VertexIndex * 2 + 1] = 0.0f;
						}

						uint32 UVBuffer;
						glGenBuffers(1, &UVBuffer);
						glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
						glBufferData(GL_ARRAY_BUFFER, NumVertex * 2 * sizeof(float32), Data, GL_STATIC_DRAW);
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						glBindVertexBuffer(4 + UVindex, UVBuffer, 0, 2 * sizeof(float32));
						glEnableVertexAttribArray(4 + UVindex);
						glVertexAttribBinding(4 + UVindex, 4 + UVindex);
						glVertexAttribFormat(4 + UVindex, 2, GL_FLOAT, false, 0);

						delete[] Data;
					}
				}

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
				glBindVertexArray(0);

				mMesh->VAO = VAO;
				mMesh->VBO = 0;
				mMesh->IBO = IBO;
				mMesh->IBOIndexSizeType = IndexType;
				mMesh->NumFaces = NumFaces;
				mMesh->AssimpMeshPtr = AssimpMesh;
				
				std::shared_ptr<ModelNode> mNode = std::shared_ptr<ModelNode>(new ModelNode());
				NodeGroup.push_back(mNode);
				mNode->VAO = VAO;
				mNode->VBO = 0;
				mNode->IBO = IBO;
				mNode->IBOIndexSizeType = IndexType;
				mNode->ModelMatrix = Mat4f(1.0);
				mNode->NumFaces = NumFaces;
				mNode->SurroundRectBox = mMesh->SurroundRectBox;
				mNode->SurroundSphereBox = mMesh->SurroundSphereBox;
			}			
		}
		for (uint32 ChildIndex = 0; ChildIndex < node->mNumChildren; ChildIndex++)
		{
			AddNode(scene, node->mChildren[ChildIndex], TransformMatrix);
		}
	}
}


void StaticMesh::BindMaterial(std::shared_ptr<MaterialInstance> shadowDepthMaterial, std::shared_ptr<MaterialInstance> lightingMaterial)
{
	ShadowDepthMaterial = shadowDepthMaterial;
	LightingMaterial = lightingMaterial;
	MaterialInstData = std::shared_ptr<MaterialData>(new MaterialData());
}

void StaticMesh::BindShadowDepthMaterial()
{
	RenderMaterial = ShadowDepthMaterial;
}

void StaticMesh::BindLightingMaterial()
{
	RenderMaterial = LightingMaterial;
}

void StaticMesh::CheckWhetherNeedClip()
{
	for (uint32 Index = 0; Index < RenderGroup.size(); Index++)
	{
		//RenderGroup[Index]->bNeedClip = RenderGroup[Index]->SurroundSphereBox.bOutOfCamera(Transform->GetModelMatrix() * (RenderGroup[Index]->AnimationSys.GetNumFrames() == -1 ? RenderGroup[Index]->ModelMatrix : RenderGroup[Index]->AnimationSys.GetNextFrameAttitudeMatrix()), _Scene->GetCamera(CameraIndex::MainCamera)->GetVPMatrix());
		//Vector3f Scale = Transform->GetScale();
		//float32 MaxScale = Scale.x > Scale.y ? Scale.x : Scale.y;
		//MaxScale = MaxScale > Scale.z ? MaxScale : Scale.z;
		//RenderGroup[Index]->bNeedClip = RenderGroup[Index]->SurroundSphereBox.bOutOfCamera(Transform->GetModelMatrix() * RenderGroup[Index]->ModelMatrix, camera->GetVPMatrix(), MaxScale);
		RenderGroup[Index]->bNeedClip = false;
	}
}

void StaticMesh::Draw()
{
	for (uint32 Index = 0; Index < RenderGroup.size(); Index++)
	{
		if (RenderGroup[Index]->bNeedClip) continue;

		Mat4f ModelMatrix = Transform->GetModelMatrix() *  RenderGroup[Index]->ModelMatrix;
		Mat4f Model_ITMatrix = Math::Inverse(Math::Transpose(Transform->GetModelMatrix() * RenderGroup[Index]->ModelMatrix));
		Mat4f Model_PreMatrix = Transform->GetModelMatrix_PreFrame() * RenderGroup[Index]->ModelMatrix;
		Mat4f Model_ITPreMatrix = Math::Inverse(Math::Transpose(Transform->GetModelMatrix_PreFrame() * RenderGroup[Index]->ModelMatrix));
		DKEngine::GetInstance().GetGPUBufferManager()->UpdateModelBuffer(ModelMatrix, Model_ITMatrix, Model_PreMatrix, Model_ITPreMatrix);

		RenderMaterial->SetUniform<Mat4f>(MaterialInstData->ModelMatrixID, Transform->GetModelMatrix() *  RenderGroup[Index]->ModelMatrix);
		RenderMaterial->SetUniform<Mat4f>(MaterialInstData->ModelMatrix_PreID, Transform->GetModelMatrix_PreFrame() * RenderGroup[Index]->ModelMatrix);
		RenderMaterial->SetUniform<Mat4f>(MaterialInstData->ModelMatrix_ITID, Math::Inverse(Math::Transpose(Transform->GetModelMatrix() * RenderGroup[Index]->ModelMatrix)));

		RenderMaterial->GetParent()->Draw(RenderGroup[Index]->VAO, RenderGroup[Index]->NumFaces, RenderGroup[Index]->IBOIndexSizeType, 0, OGL_ELEMENT);
	}	
}

void StaticMesh::UpdatePreFrameModelMatrix()
{
	Transform->SetModelMatrix_PreFrame(Transform->GetModelMatrix());
}

void StaticMesh::Start()
{

}

void StaticMesh::Update()
{

}

void StaticMesh::InternalUpdate()
{
	/*for (int32 NodeIndex = 0; NodeIndex < NodeGroup.size(); NodeIndex++)
	{
		ModelNode* Node = NodeGroup[NodeIndex].get();
		if (Node->AnimationSys.GetNumFrames() == -1)
		{
			continue;
		}
		else
		{
			Node->ModelMatrix = Node->AnimationSys.GetNextFrameAttitudeMatrix();
		}
	}*/
}

void StaticMesh::FixUpdate()
{

}

std::vector<std::shared_ptr<ModelNode>>& StaticMesh::GetModelNodeGroup()
{
	return RenderGroup;
}

void StaticMesh::SetAnimationPlaySpeed(float32 speed)
{
	/*for (int32 NodeIndex = 0; NodeIndex < NodeGroup.size(); NodeIndex++)
	{
		NodeGroup[NodeIndex]->AnimationSys.SetPlaySpeed(speed);
	}*/
}

std::shared_ptr<MaterialInstance> StaticMesh::GetRenderMaterial()
{
	return RenderMaterial;
}

void StaticMesh::Save(String* Data)
{
	Object::Save(Data);
	Transform->Save(Data);
}