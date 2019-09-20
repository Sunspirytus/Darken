#pragma once
#include "Object.h"
#include "Camera.h"
#include "DeferRenderPipeline.h"

class SphereReflectionCaptureProperty : public ObjectProperty
{
public:
	SphereReflectionCaptureProperty() {};
	~SphereReflectionCaptureProperty() {};
};

class SphereReflectionCapture : public Object
{
public:
	SphereReflectionCapture(SphereReflectionCaptureProperty property, std::shared_ptr<SceneManager> Scene, const Vector3f &position, const float32 &radius, const float32& brightness);
	~SphereReflectionCapture();

	virtual void Start() {};
	virtual void InternalUpdate() {};
	virtual void Update() {};
	virtual void FixUpdate() {};
	virtual void Draw() {};
	virtual void CheckWhetherNeedClip() {};

	void CaptureWithPipeLine(DeferRenderPipeline* Pipeline);
	void CalReflectionCubeTexAvgBrightness();

	void CreateCaptureResources();
	void Create6FacesCameraList();
	uint32 GetReflectionTextureID();
	float32 GetInfluenceRaidus();
	float32 GetBrightness();
	float32 GetAverageBrightness();

private:
	uint32 CaptureTexCube;
	uint32 CaptureTex2D;
	uint32 CaptureTexSize;
	std::shared_ptr<Camera> CaptureCamera;

	float32 InfluenceRadius;
	float32 Brightness;
	float32 AverageBrightness;
};
