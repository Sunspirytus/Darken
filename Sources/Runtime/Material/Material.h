#pragma once

#include "TypeDefine.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <typeinfo>
#include <memory>
#include <map>


enum ShaderType
{
	VertexShader,
	FragmentShader,
	GeometryShader,
	TessEvaluationShader,
	TessControlShader,
	ComputeShader
};

enum IndexSizeType
{
	Index16Bits,
	Index32Bits
};

enum UniformType
{
	Unknown,
	GLSL_INT,
	GLSL_UINT,
	GLSL_FLOAT,
	GLSL_VEC2,
	GLSL_VEC4,
	GLSL_VEC3,
	GLSL_IVEC2,
	GLSL_IVEC3,
	GLSL_IVEC4,
	GLSL_MAT3,
	GLSL_MAT4,
	GLSL_TEXTURE2D,
	GLSL_TEXTURECUBE,
	GLSL_TEXTURE3D,
	GLSL_TEXTURE_2D_ARRAY
};

enum GLDrawType
{
	Unkown,
	OGL_ELEMENT,
	OGL_LINE_STRIP
};

static std::unordered_map<uint32, UniformType> UniformTypeMap = {
	{ GL_INT,   UniformType::GLSL_INT },
	{ GL_UNSIGNED_INT, UniformType::GLSL_UINT},
	{ GL_FLOAT, UniformType::GLSL_FLOAT},
	{ GL_FLOAT_VEC2,  UniformType::GLSL_VEC2 },
	{ GL_FLOAT_VEC3,  UniformType::GLSL_VEC3 },
	{ GL_FLOAT_VEC4,  UniformType::GLSL_VEC4 },
	{ GL_INT_VEC2, UniformType::GLSL_IVEC2 },
	{ GL_INT_VEC3, UniformType::GLSL_IVEC3 },
	{ GL_INT_VEC4, UniformType::GLSL_IVEC4 },
	{ GL_FLOAT_MAT3,  UniformType::GLSL_MAT3 },
	{ GL_FLOAT_MAT4,  UniformType::GLSL_MAT4 },
	{ GL_SAMPLER_2D, UniformType::GLSL_TEXTURE2D },
	{ GL_SAMPLER_CUBE, UniformType::GLSL_TEXTURECUBE },
	{ GL_SAMPLER_3D, UniformType::GLSL_TEXTURE3D },
	{ GL_SAMPLER_2D_ARRAY, UniformType::GLSL_TEXTURE_2D_ARRAY}
};

struct AttribItem
{
	int32 Location;
	AttribItem() {}
};

struct UniformItem_WithinBlock
{
	String Name;
	UniformType DataType;
	uint32 Size;
	int32 Offset_Byte;
	UniformItem_WithinBlock() {}
};

struct UniformItem_Block
{
	int32 Id;
	int32 HashCode;
	uint32 DataSize_Byte;
	void * DataPtr;
	uint32 Index;
	std::map<uint32, UniformItem_WithinBlock> Uniforms;
};

struct UniformItem_Basic
{
	int32 Location;
	UniformType DataType;
	uint32 Size;
	void* DataPtr;
	UniformItem_Basic() {}
};

struct UniformItem_Texture
{
	int32 Location;
	uint32* IDPtr;
	UniformType DataType;
	UniformItem_Texture() : Location(-1) {}
};

struct Shader
{
	uint32 Id;
	String Name;
	ShaderType type;
	Shader() {}
};

struct Program
{
	uint32 Id;
	std::vector<Shader> shaders;
	std::unordered_map<String, AttribItem> Attribs;
	std::unordered_map<String, std::shared_ptr<UniformItem_Block>> Uniforms_Block;
	std::unordered_map<String, UniformItem_Basic> Uniforms_Basic;
	std::unordered_map<String, UniformItem_Texture> Uniforms_Texture;
};

class Material
{
public:
	Material();
	~Material();

	std::shared_ptr<Program> MaterialProgram;

	void CreateMaterial(std::vector<String>& shaderNames);
	Material(std::vector<String> shaderNames);

	void Draw(uint32 VAO, int32 NumFaces, IndexSizeType indexSize, int32 Offset = 0, GLDrawType drawType = GLDrawType::OGL_ELEMENT);
	void BindProgram();
	void UnBindProgram();
	void BindUniforms();
	void BindSamplers();
private:
	void LoadAndCreateShaders(std::vector<String>& shaderNames);
	void CreateProgram();
	void FindShaderNames(std::vector<String>& shaderNames);
	void FindAttibInfos();
	void FindUniformInfos();
	void LinkLocation();
	uint32 CreateShaderGPUObjFromSrcCode(String & Code, ShaderType type);
};

