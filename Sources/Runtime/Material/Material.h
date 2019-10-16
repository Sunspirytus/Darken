#pragma once

#include "TypeDefine.h"
#include "PropertyBase.h"
#include "CommonFunctions.h"
#include "ShaderHelper.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <typeinfo>
#include <memory>
#include <map>
#include <set>


static String DefaultVertShaderName = "DefaultVertShader.vsh";
static String DefaultFragShaderName = "DefaultFragShader.fsh";

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

static std::unordered_map<uint32, UniformType> UniformTypeMap = 
{
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
	{ GL_SAMPLER_2D_ARRAY, UniformType::GLSL_TEXTURE_2D_ARRAY }
};

static std::unordered_map<UniformType, VariableType> GPU_CPU_TypeMap =
{
	{ UniformType::GLSL_INT, VariableType::INT_32 },
	{ UniformType::GLSL_UINT, VariableType::UINT_32 },
	{ UniformType::GLSL_FLOAT, VariableType::FLOAT_32 },
	{ UniformType::GLSL_VEC2, VariableType:: VECTOR2_F },
	{ UniformType::GLSL_VEC3, VariableType::VECTOR3_F },
	{ UniformType::GLSL_VEC4, VariableType::VECTOR4_F },
	{ UniformType::GLSL_IVEC2, VariableType::VECTOR2_I },
	{ UniformType::GLSL_IVEC3, VariableType::VECTOR3_I },
	{ UniformType::GLSL_IVEC4, VariableType::VECTOR4_I },
	{ UniformType::GLSL_MAT3, VariableType::MAT3_F },
	{ UniformType::GLSL_MAT4, VariableType::MAT4_F },
	{ UniformType::GLSL_TEXTURE2D, VariableType::STRING },
	{ UniformType::GLSL_TEXTURECUBE, VariableType::STRING },
	{ UniformType::GLSL_TEXTURE3D, VariableType::STRING },
	{ UniformType::GLSL_TEXTURE_2D_ARRAY, VariableType::STRING }
};

struct AttribItem
{
	String Name;
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
	String Name;
	int32 Id;
	uint32 DataSize_Byte;
	void * DataPtr;
	uint32 Index;
	std::map<uint32, std::shared_ptr<UniformItem_WithinBlock>> Uniforms;
};

struct UniformItem_Basic
{
	String Name;
	int32 Location;
	UniformType DataType;
	uint32 Size;
	void* DataPtr;
	UniformItem_Basic() {}
};

struct UniformItem_Texture
{
	String Name;
	int32 Location;
	uint32* IDPtr;
	UniformType DataType;
	UniformItem_Texture() : Location(-1) {}
};

class Shader
{
public:
	uint32 Id;
	ShaderType Type;
	Shader();
	~Shader();
};

struct Program
{
	uint32 Id;
	std::set<std::shared_ptr<Shader>> Shaders;
	std::set<std::shared_ptr<AttribItem>> Attribs;
	std::set<std::shared_ptr<UniformItem_Block>> Uniforms_Block;
	std::set<std::shared_ptr<UniformItem_Basic>> Uniforms_Basic;
	std::set<std::shared_ptr<UniformItem_Texture>> Uniforms_Texture;
};

class MaterialBase : public PropertyBase
{
public:
	MaterialBase();
	//MaterialBase(const String& path, const std::vector<String>& shaderNames = std::vector<String>{});
	~MaterialBase();

	String GetPath();

	virtual void Save(String* Data);

protected:
	String Path;
	std::vector<String> ShaderNames;
};

class Material : public MaterialBase
{
public:
	Material();
	Material(const String& name);
	Material(const String& name, const std::vector<String>& shaderNames);
	~Material();

	std::shared_ptr<Program> ProgramGPU;

	void Draw(uint32 VAO, int32 NumFaces, IndexSizeType indexSize, int32 Offset = 0, GLDrawType drawType = GLDrawType::OGL_ELEMENT);
	void BindProgram();
	void UnBindProgram();
	void BindUniforms();
	void BindSamplers();

	virtual void Save(String* Data);
	virtual void Load(const String& Data);

private:
	//void LoadAndCreateShaders(std::vector<String>& shaderNames);

	void SaveShaderSourceCode(String* OutData, ShaderType Type, const String& SourceCode);
	void LoadBaseInfo(const String& SourceCode);
	std::map<ShaderType, String> LoadShaderSourceCode(const String& SourceCode);


	void ReCreate();
	
	void InitShaderInfo();
	std::map<ShaderType, String> CreateShadersSourceCode();
	void CreateGPUShaders(std::map<ShaderType, String>& SrcCodes);
	uint32 CreateShaderGPUObjFromSrcCode(const String& Code, ShaderType type);
	void CreateGPUProgram();
	void FindAttibInfos();
	void FindUniformInfos();
	void LinkLocation();
};

