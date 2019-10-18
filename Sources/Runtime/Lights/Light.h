#pragma once

#include "Transform.h"
#include "MaterialInstance.h"
#include "BufferManager.h"

enum class LightType
{
	Direct,
	Point,
	Spot,
	Unknow
};

class Light
{
public:
	Light();
	~Light();

	LightType Type;
	void SetTransform(Vector3f position, Vector3f eulerAngle);
	void SetColor(Vector3f newColor);
	void SetShadowBias(float32 newBias);
	float32 GetShadowBias();

	TransformComponent* GetTransform();

	virtual void GetShaderData(LightData &lightBuffer) = 0;

protected:
	TransformComponent LightTransform;

	float32 ShadowBias;
	Vector3f Color;
	float32 Intensity;
};


class DirectLight : public Light
{
public:
	DirectLight();
	DirectLight(Vector3f eulerAngles);
	~DirectLight();

	Vector3f GetDirection();
	Vector3f GetUpDir();

	virtual void GetShaderData(LightData &lightBuffer) final {};
};

class PointLight : public Light
{
public:
	PointLight();
	~PointLight();

	void SetIntensity(float32 newIntensity);
	void SetAttenuationRadius(float32 newRadius);
	void SetSourceRadius(float32 Radius);
	void SetSoftSourceRadius(float32 newRadius);
	void SetSourceLength(float32 newLength);

	float32 GetAttenuationRadius();

	virtual void GetShaderData(LightData &lightBuffer);
protected:
	float32 AttenuationRadius;
	float32 InvRadius;
	float32 SourceRadius;
	float32 SoftSourceRadius;
	float32 SourceLength;

	float32 FallofExponent;
	float32 SpecularScale;
	float32 Brightness;
};

class SpotLight : public PointLight
{
public:
	SpotLight();
	~SpotLight();

	Vector3f GetDirection();
	Vector3f GetUpDir();
	
	void SetInnerConeAngle(float32 Degree);
	void SetOuterConeAngle(float32 Degree);
	
	float32 GetOutConeAngle();

	virtual void GetShaderData(LightData &lightBuffer) final;

private:
	float32 CosOuterCone;
	float32 CosInnerCone;
	float32 InvCosConeDifference;
	
	float32 InnerConeAngle;
	float32 OuterConeAngle;
};

