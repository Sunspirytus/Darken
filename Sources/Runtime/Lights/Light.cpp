#include "Light.h"
#include "Util.h"

Light::Light()
{
	LightTransform = TransformComponent();
	Type = Unknow;
	ShadowBias = 0.0;
}

Light::~Light()
{
}

void Light::SetColor(Vector3f newColor)
{
	Color = newColor;
}

void Light::SetShadowBias(float32 newBias)
{
	ShadowBias = newBias;
}

float32 Light::GetShadowBias()
{
	return ShadowBias;
}

void Light::SetTransform(Vector3f position, Vector3f eulerAngle)
{
	LightTransform.SetTransform(position, eulerAngle, Vector3f(1.0));
}

TransformComponent* Light::GetTransform()
{
	return &LightTransform;
}


DirectLight::DirectLight()
{
	LightTransform.SetPosition(Vector3f(FLT_MAX));
	Type = LightType::Direct;
}

DirectLight::DirectLight(Vector3f eulerAngles) 
{
	LightTransform.SetEulerAngle(eulerAngles);
	Type = LightType::Direct;
}

Vector3f DirectLight::GetDirection()
{
	return -LightTransform.GetForward();
}

Vector3f DirectLight::GetUpDir()
{
	return LightTransform.GetUpward();
}

DirectLight::~DirectLight()
{
}

PointLight::PointLight()
{
	Type = LightType::Point;
	FallofExponent = 8.0f;
	SpecularScale = 1.0f;
}

PointLight::~PointLight()
{
}

void PointLight::SetIntensity(float32 newIntensity)
{
	Intensity = newIntensity;
	Brightness = Intensity * 16.0f;
}

void PointLight::SetAttenuationRadius(float32 newRadius)
{
	AttenuationRadius = newRadius;
	InvRadius = 1.0f / AttenuationRadius;
}

void PointLight::SetSourceRadius(float32 Radius)
{
	SourceRadius = Radius;
}

void PointLight::SetSoftSourceRadius(float32 newRadius)
{
	SoftSourceRadius = newRadius;
}

void PointLight::SetSourceLength(float32 newLength)
{
	SourceLength = newLength;
}

float32 PointLight::GetAttenuationRadius()
{
	return AttenuationRadius;
}

void PointLight::GetShaderData(LightData &lightBuffer)
{
	lightBuffer.LightPosition = LightTransform.GetPosition();
	lightBuffer.LightInvRadius = InvRadius;
	lightBuffer.LightColor = Color * Brightness;
	lightBuffer.LightFallofExponent = FallofExponent;
	lightBuffer.NormalizedLightDirection = Vector3f(-1.0, 0.0, 0.0);
	lightBuffer.NormalizedLightTangent = Vector3f(0.0, 0.0, 1.0);
	lightBuffer.LightSpotAngles = Vector2f(-2.0f, 1.0f);
	lightBuffer.LightSourceRadius = SourceRadius;
	lightBuffer.LightSourceLength = SourceLength;
	lightBuffer.LightSoftSourceRadius = SoftSourceRadius;
	lightBuffer.LightSpecularScale = SpecularScale;
	lightBuffer.LightContactShadowLength = 0.0;
	lightBuffer.LightDistanceFadeMAD = Vector2f(0.0f);
	lightBuffer.LightShadowMapChannelMask = Vector4f(0.0f);
	lightBuffer.LightShadowedBits = 3;
	lightBuffer.LightType = 2;
}

SpotLight::SpotLight()
{
	Type = LightType::Spot;
}

SpotLight::~SpotLight()
{
}

Vector3f SpotLight::GetDirection()
{
	return -LightTransform.GetForward();
}

Vector3f SpotLight::GetUpDir()
{
	return LightTransform.GetUpward();
}

void SpotLight::GetShaderData(LightData &lightBuffer)
{
	lightBuffer.LightPosition = LightTransform.GetPosition();
	lightBuffer.LightInvRadius = InvRadius;
	lightBuffer.LightColor = Color * Brightness;
	lightBuffer.LightFallofExponent = FallofExponent;
	lightBuffer.NormalizedLightDirection = GetDirection();
	lightBuffer.NormalizedLightTangent = LightTransform.GetUpward();
	lightBuffer.LightSpotAngles = Vector2f(CosOuterCone, InvCosConeDifference);
	lightBuffer.LightSourceRadius = SourceRadius;
	lightBuffer.LightSourceLength = SourceLength;
	lightBuffer.LightSoftSourceRadius = SoftSourceRadius;
	lightBuffer.LightSpecularScale = SpecularScale;
	lightBuffer.LightContactShadowLength = 0.0;
	lightBuffer.LightDistanceFadeMAD = Vector2f(0.0f);
	lightBuffer.LightShadowMapChannelMask = Vector4f(0.0f);
	lightBuffer.LightShadowedBits = 3;
	lightBuffer.LightType = 1;
}

void SpotLight::SetInnerConeAngle(float32 Degree)
{
	InnerConeAngle = Degree;
	CosInnerCone = Math::Cos(InnerConeAngle / 180.0f * 3.14159265358979323846264338327950288f);
	InvCosConeDifference = 1.0f / (CosInnerCone - CosOuterCone);
}

void SpotLight::SetOuterConeAngle(float32 Degree)
{
	OuterConeAngle = Degree;
	CosOuterCone = Math::Cos(OuterConeAngle / 180.0f * 3.14159265358979323846264338327950288f);
	InvCosConeDifference = 1.0f / (CosInnerCone - CosOuterCone);
}

float32 SpotLight::GetOutConeAngle()
{
	return OuterConeAngle;
}
