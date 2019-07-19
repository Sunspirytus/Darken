#pragma once

#include <float.h>
#include "TypeDefine.h"

enum HitState
{
	NoHit,
	Cross,
	Inside,
	OutSurround
};

//2018.11.8
class SphereBox
{
public:
	SphereBox();
	~SphereBox();
 
	void Reset();
	bool bOutOfCamera(Mat4f ModelMatrix, Mat4f VPMatrix, float32 scale);

	SphereBox& operator+= (const Vector3f& newVertex);

	Vector3f GetCenterPointLocal();
	float32 GetRadius();

private:
	Vector4f ClipPlanesLocal[6];

	Vector4f PointMin;
	Vector4f PointMax;
	Vector4f CenterLocal;
	float32 Radius;
	float32 Height;
};




//2018.9
//            2                                3
//              ------------------------------
//             /|                           /|
//            / |                          / |				ÓÒÊÖ×ø±êÏµ
//           /  |                         /  |
//          /   |                        /   |
//         /    |                       /    |				+Y
//        /     |                      /     |				|
//       /      |                     /      |				|
//      /       |                    /       |				|
//     /        |                   /        |				|
//  6 /         |                7 /         |				|
//   /----------------------------/          |				/-------------- +X
//   |          |                 |          |			   /	
//   |          |                 |          |      	  /
//   |        0 |                 |          |			 /
//   |          |-----------------|----------|			/+Z
//   |         /                  |         /  1    
//   |        /                   |        /               
//   |       /                    |       /         
//   |      /                     |      /              
//   |     /                      |     /               
//   |    /                       |    /               
//   |   /                        |   /           
//   |  /                         |  /            
//   | /                          | /             
//   |/                           |/              
//   ------------------------------               
//  4                              5
//

class RectBox
{
public:
	RectBox();
	~RectBox();

	void UpdateBoxModel(uint32 vertexNum, uint8 * pointPtr, uint8 stride);
	//void UpdateBoxWorld(Mat4f & ModelMatrix);
	//void CreateBoxFromCornerWorld(float32 Xmin, float32 Xmax, float32 Ymin, float32 Ymax, float32 Zmin, float32 Zmax);
	bool NeedClipFromObjSpace(Mat4f & MVP_Matrix);
	//bool NeedClipFromWorldSpace(Mat4f & VP_Matrix);
	//int32 HitBox(RectBox & Box);
	//bool CenterInsideBoxWorldSpace(RectBox & Box);

	RectBox& operator+= (const Vector3f& newVertex);

	Vector3f GetCenterPointLocal();
	Vector3f GetBoundMin();
	Vector3f GetBoundMax();
	float32 GetLength();
	float32 GetWidth();
	float32 GetHeight();

private:
	Vector4f CenterLocal;
	Vector3f BoxSize;

	//Vector4f CenterWorld;
	Vector4f pointCornersModel[8];
	Vector4f clipPlaneModel[6];

	Vector4f PointMin = Vector4f(FLT_MAX, FLT_MAX, FLT_MAX, 1.0);
	Vector4f PointMax = Vector4f(-FLT_MAX, -FLT_MAX, -FLT_MAX, 1.0);

	//Vector4f pointCornersWorld[8];
	//Vector4f clipPlaneWorld[6];
	void createCornerPointsModel();
	//void createCornerPointsWorld();
};

