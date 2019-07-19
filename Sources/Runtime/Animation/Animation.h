#pragma once

#include "TypeDefine.h"
#include "Util.h"
#include <vector>


class Animation
{
public:
	Animation();
	~Animation();

	int32 GetNumFrames();
	void SetPlaySpeed(float32 speed);
	void Stop();
	void Play();
	void SetAttitudeMatrixes(const float32 * matrixData, int32 numFrames);
	Mat4f GetNextFrameAttitudeMatrix();
	
private:
	std::vector<Mat4f> AttitudeMatrixes;
	float32 CurrentFrame;
	int32 NumFrames;
	float32 Speed;
	bool isPlay;
};
