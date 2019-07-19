#pragma once
#include "MaterialInstance.h"

class RectBufferObject
{
public:
	RectBufferObject();
	~RectBufferObject();
	uint32 VAO;
	uint32 VBO;
	uint32 IBO;
	int32 NumFaces;
	IndexSizeType IndexType;
private:

};