#include "Camera.h"
#include "Quaternion.h"

Camera::Camera()
{
	
}

Camera::~Camera()
{
}

Camera::Camera(Vector3f position, Vector3f eulerAngle, float32 fovy, float32 aspect, float32 nearPlane, float32 farPlane, Vector2i viewPortSize)
{
	Init(position, eulerAngle, fovy, aspect, nearPlane, farPlane, viewPortSize);
}

void Camera::Init(Vector3f position, Vector3f eulerAngle, float32 fovy, float32 aspect, float32 nearPlane, float32 farPlane, Vector2i viewPortSize)
{
	Transform->SetTransform(position, eulerAngle, Vector3f(1.0));
	ViewPortSize = viewPortSize;
	Fovy = fovy;
	Aspect = aspect;
	NearPlane = nearPlane;
	FarPlane = farPlane;
	CreateViewMatrix();
	ViewMatrix_PreFrame = ViewMatrix;
	CreateProjectionMatrix();
	CreateVPMatrix();
	NextCamera = nullptr;
}

void Camera::CreateViewMatrix()
{
	Mat4f viewMatrix = Mat4f(1.0);
	ViewMatrix = CameraUtil::LookAt(Transform->GetPosition(), Transform->GetPosition() + Transform->GetForward(), Transform->GetUpward());
}

void Camera::CreateProjectionMatrix()
{
	ProjectMatrix = CameraUtil::Perspective(Fovy,
		Aspect,
		NearPlane,
		FarPlane
		);
	ProjectMatrix_PreFrame = ProjectMatrix;
}

void Camera::CreateVPMatrix()
{
	VPMatrix = ProjectMatrix * ViewMatrix;
	VPMatrix_I = Math::Inverse(VPMatrix);
}

//void Camera::ModifyProjectionForClipping(Vector4f vClipPlane)
//{
//	Vector4f vClipPlaneView(vClipPlane * Math::Inverse(ViewMatrix));	// put clip plane into view coords
//	/*
//	Calculate the clip-space corner point opposite the clipping plane
//	and transform it into camera space by multiplying it by the inverse
//	projection matrix.
//	*/
//	Vector4f vClipSpaceCorner(sgn(vClipPlaneView.x), sgn(vClipPlaneView.y), 1.0f, 1.0f);
//	vClipSpaceCorner = vClipSpaceCorner * Math::Inverse(ProjectMatrix);
//
//	// Calculate the scaled plane vector
//	Vector4f vScaledPlane = vClipPlaneView * (2.0f / glm::dot(vClipSpaceCorner, vClipPlaneView));
//
//	// Replace the third row of the matrix
//	glm::value_ptr(ProjectMatrix)[2] = vScaledPlane.x;
//	glm::value_ptr(ProjectMatrix)[6] = vScaledPlane.y;
//	glm::value_ptr(ProjectMatrix)[10] = vScaledPlane.z + 1.0f;
//	glm::value_ptr(ProjectMatrix)[14] = vScaledPlane.w;
//	CreateVPMatrix();
//}

void Camera::SetPosition(Vector3f position)
{
	

	if (Transform->GetPosition() == position) return;
	Transform->SetPosition(position);
	CreateViewMatrix();
	CreateVPMatrix();
}

void Camera::SetPosition(float32 x, float32 y, float32 z)
{
	Vector3f newPosition(x, y, z);
	if (Transform->GetPosition() == newPosition) return;
	Transform->SetPosition(newPosition);
	CreateViewMatrix();
	CreateVPMatrix();	
}

void Camera::SetEulerAngle(Vector3f eulerAngle)
{
	if (Transform->GetEulerAngle() == eulerAngle) return;
	Transform->SetEulerAngle(eulerAngle);
	CreateViewMatrix();
	CreateVPMatrix();
}

void Camera::SetEulerAngle(float32 x, float32 y, float32 z)
{
	Vector3f newEulerAngle(x, y, z);
	if (Transform->GetEulerAngle() == newEulerAngle) return;
	Transform->SetEulerAngle(newEulerAngle);
	CreateViewMatrix();
	CreateVPMatrix();
}

void Camera::SetFovy(float32 fovy)
{
	Fovy = fovy;
	CreateProjectionMatrix();
	CreateVPMatrix();
}

void Camera::SetAspect(float32 aspect)
{
	Aspect = aspect;
	CreateProjectionMatrix();
	CreateVPMatrix();
}

void Camera::SetNearPlaneDis(float32 nearDis)
{
	NearPlane = nearDis;
	CreateProjectionMatrix();
	CreateVPMatrix();
}

void Camera::SetFarPlaneDis(float32 farDis)
{
	FarPlane = farDis;
	CreateProjectionMatrix();
	CreateVPMatrix();
}

void Camera::SetProjectMatrix(Mat4f newMatrix)
{
	ProjectMatrix = newMatrix;
	CreateVPMatrix();
}

void Camera::SetViewMatrix_PreFrame(Mat4f newMatrix)
{
	ViewMatrix_PreFrame = newMatrix;
}

void Camera::SetProjectMatrix_PreFrame(Mat4f newMatrix)
{
	ProjectMatrix_PreFrame = newMatrix;
}

void Camera::SetViewPortSize(Vector2i viewPortSize)
{
	ViewPortSize = viewPortSize;
}

void Camera::SetDirection(const Vector3f& forward, const Vector3f& up)
{
	Transform->SetForward(forward);
	Transform->SetUpward(up);
	CreateViewMatrix();
	CreateVPMatrix();
}

Mat4f Camera::GetViewMatrix()
{
	return ViewMatrix;
}

Mat4f Camera::GetViewMatrix_PreFrame()
{
	return ViewMatrix_PreFrame;
}

Mat4f Camera::GetProjectMatrix()
{
	return ProjectMatrix;
}

Mat4f Camera::GetProjectMatrix_PreFrame() 
{
	return ProjectMatrix_PreFrame;
}

Mat4f Camera::GetVPMatrix()
{
	return VPMatrix;
}

Mat4f Camera::GetVPMatrix_I()
{
	return VPMatrix_I;
}

Vector3f Camera::GetEulerAngle()
{
	return Transform->GetEulerAngle();
}

Vector3f Camera::GetPosition()
{
	return Transform->GetPosition();
}

Vector3f Camera::GetForward()
{
	return Transform->GetForward();
}

Vector3f Camera::GetUpward()
{
	return Transform->GetUpward();
}

Vector3f Camera::GetLeftward()
{
	return Transform->GetLeftward();
}

float32 Camera::GetFOVinRadians()
{
	return Fovy;
}

float32 Camera::GetAspect()
{
	return Aspect;
}

float32 Camera::GetNearClipPlaneDis()
{
	return NearPlane;
}

float32 Camera::GetFarClipPlaneDis()
{
	return FarPlane;
}

void Camera::ReCalculateProjectMatrix()
{
	CreateProjectionMatrix();
}

float32 Camera::sgn(float32 a)
{
	if (a > 0.0f) return(1.0f);
	if (a < 0.0f) return(-1.0f);
	return 0.0f;
}

Vector2i Camera::GetViewPortSize()
{
	return ViewPortSize;
}

void Camera::ActiveViewPort()
{
	glViewport(0, 0, ViewPortSize.x, ViewPortSize.y);
}

void Camera::SetNextCamera(std::shared_ptr<Camera> camera)
{
	NextCamera = camera;
}

std::shared_ptr<Camera> Camera::GetNextCamera()
{
	return NextCamera;
}