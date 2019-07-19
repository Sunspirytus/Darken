#pragma once
#include "TypeDefine.h"

class SystemTextureFactory
{
public:
	SystemTextureFactory();
	~SystemTextureFactory();

	void GeneratePreIntegratedGFTexture();
	uint32 GetPreIntegratedGF_Tex();

private:
	uint32 PreIntegratedGF_Tex;
};


