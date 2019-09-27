#include "Transform.h"
#include "Quaternion.h"
#include "CommonFunctions.h"

TransformComponentBase::TransformComponentBase(const Vector3f& position,
	const Vector3f& eulerAngle,
	const Vector3f& scale)
	:	ComponentBase(CPT_Transform)
	,	Position(position)
	,	EulerAngle(eulerAngle)
	,	Scale(scale)
{
	AddProperty("Position", VECTOR3_F, &Position);
	AddProperty("EulerAngle", VECTOR3_F, &EulerAngle);
	AddProperty("Scale", VECTOR3_F, &Scale);
}

TransformComponentBase::~TransformComponentBase()
{
}

TransformComponent::TransformComponent(Vector3f position, Vector3f eulerAngle, Vector3f scale)
	: TransformComponentBase(position, eulerAngle, scale)
{
	Position = Vector3f(position);
	TranslationMatrix = Mat4f(Math::Translate(Mat4f(1.0), position));

	EulerAngle = Vector3f(eulerAngle);
	RotationMatrix = Math::Rotate(Mat4f(1.0), Math::Radians(eulerAngle.z), ORIGIN_UPWARD) * Math::Rotate(Mat4f(1.0), Math::Radians(eulerAngle.x), ORIGIN_FORWARD) * Math::Rotate(Mat4f(1.0), Math::Radians(eulerAngle.y), ORIGIN_LEFTWARD);
	
	Scale = Vector3f(scale);
	ScaleMatrix = Math::Scale(Mat4f(1.0), scale);

	Forward = Mat3f(RotationMatrix) * ORIGIN_FORWARD;
	
	Upward = Mat3f(RotationMatrix) * ORIGIN_UPWARD;
	
	Leftward = Mat3f(RotationMatrix) * ORIGIN_LEFTWARD;

	CalculateModelMatrix();
	ModelMatrix_PreFrame = ModelMatrix;
}

TransformComponent::TransformComponent()
	: TransformComponentBase(Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, 0.0, 0.0), Vector3f(1.0, 1.0, 1.0))
{
	Position = Vector3f(0.0, 0.0, 0.0);
	TranslationMatrix = Mat4f(1.0);

	EulerAngle = Vector3f(0.0, 0.0, 0.0);
	RotationMatrix = Mat4f(1.0);

	Scale = Vector3f(1.0, 1.0, 1.0);
	ScaleMatrix = Mat4f(1.0);
	
	Forward = ORIGIN_FORWARD;

	Upward = ORIGIN_UPWARD;

	Leftward = ORIGIN_LEFTWARD;

	CalculateModelMatrix();
	ModelMatrix_PreFrame = ModelMatrix;
}

void TransformComponent::SetTransform(Vector3f position, Vector3f eulerAngle, Vector3f scale)
{
	Position = Vector3f(position);
	TranslationMatrix = Mat4f(Math::Translate(Mat4f(1.0), position));

	EulerAngle = Vector3f(eulerAngle);
	RotationMatrix = Math::Rotate(Mat4f(1.0), Math::Radians(eulerAngle.z), ORIGIN_UPWARD) * Math::Rotate(Mat4f(1.0), Math::Radians(eulerAngle.x), ORIGIN_FORWARD) * Math::Rotate(Mat4f(1.0), Math::Radians(eulerAngle.y), ORIGIN_LEFTWARD);

	Scale = Vector3f(scale);
	ScaleMatrix = Math::Scale(Mat4f(1.0), scale);

	Forward = Mat3f(RotationMatrix) * ORIGIN_FORWARD;

	Upward = Mat3f(RotationMatrix) * ORIGIN_UPWARD;

	Leftward = Mat3f(RotationMatrix) * ORIGIN_LEFTWARD;

	CalculateModelMatrix();
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetPosition(Vector3f position)
{
	if (Position == position) return;
	Position = position;
	TranslationMatrix = Math::Translate(Mat4f(1.0), position);
	CalculateModelMatrix();
}

void TransformComponent::SetEulerAngle(Vector3f eulerAngle)
{
	if (EulerAngle == eulerAngle) return;
	EulerAngle = eulerAngle;
	RotationMatrix = Math::Rotate(Mat4f(1.0), Math::Radians(eulerAngle.z), ORIGIN_UPWARD) * Math::Rotate(Mat4f(1.0), Math::Radians(eulerAngle.x), ORIGIN_FORWARD) * Math::Rotate(Mat4f(1.0), Math::Radians(eulerAngle.y), ORIGIN_LEFTWARD);
	CalculateModelMatrix();
	Forward = Mat3f(RotationMatrix) * ORIGIN_FORWARD;
	Upward = Mat3f(RotationMatrix) * ORIGIN_UPWARD;
	Leftward = Mat3f(RotationMatrix) * ORIGIN_LEFTWARD;
}

void TransformComponent::SetScale(Vector3f scale)
{
	if (Scale == scale) return;
	Scale = scale;
	ScaleMatrix = Math::Scale(Mat4f(1.0), scale);
	CalculateModelMatrix();
}

void TransformComponent::SetForward(Vector3f forward)
{
	if (Math::IsNearlyEqual(Forward, forward, 0.1f)) return;
	Vector3f C = Math::Cross(Forward, forward);
	float32 angle = Math::Degrees(Math::aCos(Math::Dot(Forward, forward) / (Math::Length(Forward) * Math::Length(forward))));
	Mat4f newRotateMatrix = mQuaternion::AxisAngle(Math::Radians(angle), C);
	float32 EulerX = Math::Degrees(std::atan2(newRotateMatrix[2][1], Math::Sqrt(newRotateMatrix[2][0] * newRotateMatrix[2][0] + newRotateMatrix[2][2] * newRotateMatrix[2][2])));
	float32 EulerY = Math::Degrees(std::atan2(-newRotateMatrix[2][0] / Math::Cos(Math::Radians(EulerX)), newRotateMatrix[2][2] / Math::Cos(Math::Radians(EulerX))));
	float32 EulerZ = Math::Degrees(std::atan2(-newRotateMatrix[0][1] / Math::Cos(Math::Radians(EulerX)), newRotateMatrix[1][1] / Math::Cos(Math::Radians(EulerX))));

	SetEulerAngle(Vector3f(EulerX, -EulerY, EulerZ) + EulerAngle);
}

void TransformComponent::SetLeftward(Vector3f leftward)
{
	if (Math::IsNearlyEqual(Leftward, leftward, 0.1f)) return;
	Vector3f C = Math::Cross(Leftward, leftward);
	float32 angle = Math::Degrees(Math::aCos(Math::Dot(Leftward, leftward) / (Math::Length(Leftward) * Math::Length(leftward))));
	Mat4f newRotateMatrix = mQuaternion::AxisAngle(Math::Radians(angle), C);
	float32 EulerX = Math::Degrees(std::atan2(newRotateMatrix[2][1], Math::Sqrt(newRotateMatrix[2][0] * newRotateMatrix[2][0] + newRotateMatrix[2][2] * newRotateMatrix[2][2])));
	float32 EulerY = Math::Degrees(std::atan2(-newRotateMatrix[2][0] / Math::Cos(Math::Radians(EulerX)), newRotateMatrix[2][2] / Math::Cos(Math::Radians(EulerX))));
	float32 EulerZ = Math::Degrees(std::atan2(-newRotateMatrix[0][1] / Math::Cos(Math::Radians(EulerX)), newRotateMatrix[1][1] / Math::Cos(Math::Radians(EulerX))));

	SetEulerAngle(Vector3f(EulerX, -EulerY, EulerZ) + EulerAngle);
}

void TransformComponent::SetUpward(Vector3f upward)
{
	if (Math::IsNearlyEqual(Upward, upward, 0.1f)) return;
	Vector3f C = Math::Cross(Upward, upward);
	float32 angle = Math::Degrees(Math::aCos(Math::Dot(Upward, upward) / (Math::Length(Upward) * Math::Length(upward))));
	Mat4f newRotateMatrix = mQuaternion::AxisAngle(Math::Radians(angle), C);
	float32 EulerX = Math::Degrees(std::atan2(newRotateMatrix[2][1], Math::Sqrt(newRotateMatrix[2][0] * newRotateMatrix[2][0] + newRotateMatrix[2][2] * newRotateMatrix[2][2])));
	float32 EulerY = Math::Degrees(std::atan2(-newRotateMatrix[2][0] / Math::Cos(Math::Radians(EulerX)), newRotateMatrix[2][2] / Math::Cos(Math::Radians(EulerX))));
	float32 EulerZ = Math::Degrees(std::atan2(-newRotateMatrix[0][1] / Math::Cos(Math::Radians(EulerX)), newRotateMatrix[1][1] / Math::Cos(Math::Radians(EulerX))));

	SetEulerAngle(Vector3f(EulerX, -EulerY, EulerZ) + EulerAngle);
}

void TransformComponent::SetModelMatrix_PreFrame(Mat4f newMatrix)
{
	ModelMatrix_PreFrame = newMatrix;
}

Vector3f TransformComponent::GetForward()
{
	return Forward;
}

Vector3f TransformComponent::GetUpward()
{
	return Upward;
}

Vector3f TransformComponent::GetLeftward()
{
	return Leftward;
}

Vector3f TransformComponent::GetPosition()
{
	return Position;
}

Vector3f TransformComponent::GetEulerAngle()
{
	return EulerAngle;
}

Vector3f TransformComponent::GetScale()
{
	return Scale;
}

Mat4f TransformComponent::GetTranslationMatrix()
{
	return TranslationMatrix;
}

Mat4f TransformComponent::GetRotationMatrix()
{
	return RotationMatrix;
}

Mat4f TransformComponent::GetScaleMatrix()
{
	return ScaleMatrix;
}

Mat4f TransformComponent::GetModelMatrix()
{
	return ModelMatrix;
}

Mat4f TransformComponent::GetModelMatrix_PreFrame()
{
	return ModelMatrix_PreFrame;
}

Mat4f TransformComponent::GetModelMatrix_IT()
{
	return ModelMatrix_IT;
}

void TransformComponent::CalculateModelMatrix()
{
	ModelMatrix = TranslationMatrix * ScaleMatrix * RotationMatrix;
	ModelMatrix_IT = Math::Inverse(Math::Transpose(ModelMatrix));
}
