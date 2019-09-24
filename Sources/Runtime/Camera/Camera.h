#pragma once

#include "Object.h"

class CameraProperty : public ObjectProperty
{
public:
	CameraProperty() {};
	~CameraProperty() {};

	float32 Fovy;
	float32 Aspect;
	float32 NearPlane;
	float32 FarPlane;
};

class Camera : public Object
{
public:
	Camera(std::shared_ptr<CameraProperty> property);
	~Camera();
	Camera(std::shared_ptr<CameraProperty> property, Vector3f position, Vector3f eulerAngle, float32 fovy, float32 aspect, float32 nearPlane, float32 farPlane, Vector2i viewPortSize);
	void Init(Vector3f position, Vector3f eulerAngle, float32 fovy, float32 aspect, float32 nearPlane, float32 farPlane, Vector2i viewPortSize);
	//void ModifyProjectionForClipping(Vector4f vClipPlane);

	Mat4f GetViewMatrix();
	Mat4f GetViewMatrix_PreFrame();
	Mat4f GetProjectMatrix_PreFrame();
	Mat4f GetProjectMatrix();
	Mat4f GetVPMatrix();
	Mat4f GetVPMatrix_I();
	Vector3f GetEulerAngle();
	Vector3f GetPosition();
	Vector3f GetForward();
	Vector3f GetUpward();
	Vector3f GetLeftward();
	float32 GetFOVinRadians();
	float32 GetAspect();
	float32 GetFarClipPlaneDis();
	float32 GetNearClipPlaneDis();
	Vector2i GetViewPortSize();
	void SetPosition(Vector3f position);
	void SetPosition(float32 x, float32 y, float32 z);
	void SetEulerAngle(Vector3f eulerAngle);
	void SetEulerAngle(float32 x, float32 y, float32 z);
	void SetFovy(float32 fovy);
	void SetAspect(float32 aspect);
	void SetNearPlaneDis(float32 nearDis);
	void SetFarPlaneDis(float32 farDis);
	void SetProjectMatrix(Mat4f newMatrix);
	void SetViewMatrix_PreFrame(Mat4f newMatrix);
	void SetProjectMatrix_PreFrame(Mat4f newMatrix);
	void SetViewPortSize(Vector2i viewPortSize);
	void ReCalculateProjectMatrix();
	void SetDirection(const Vector3f& forward, const Vector3f& up);

	void ActiveViewPort();

	void SetNextCamera(std::shared_ptr<Camera> camera);
	std::shared_ptr<Camera> GetNextCamera();

	virtual void CheckWhetherNeedClip() final {};
	virtual void Start() final {};
	virtual void InternalUpdate() final {};
	virtual void Update() final {};
	virtual void FixUpdate() final {};
	virtual void Draw() final {};
	virtual void Save(std::string* Data) final {};
private:

	float32 Fovy;
	float32 Aspect;
	float32 NearPlane;
	float32 FarPlane;

	Vector2i ViewPortSize;

	void CreateViewMatrix();
	void CreateProjectionMatrix();
	void CreateVPMatrix();

	Mat4f ViewMatrix;
	Mat4f ProjectMatrix;
	Mat4f VPMatrix;
	Mat4f VPMatrix_I;

	Mat4f ViewMatrix_PreFrame;
	Mat4f ProjectMatrix_PreFrame;

	//Transform Transform;

	float32 sgn(float32 a);

	std::shared_ptr<Camera> NextCamera;
};
