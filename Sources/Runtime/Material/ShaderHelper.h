#pragma once

#include "TypeDefine.h"

enum ShaderType
{
	VertexShader,
	FragmentShader,
	GeometryShader,
	TessEvaluationShader,
	TessControlShader,
	ComputeShader
};

static const String ReShaderTag_Internal = "#ReShader_Internal_";
static const String ReShaderTag_External = "#ReShader_External_";
class ShaderHelper
{
public:
	ShaderHelper();
	~ShaderHelper();

	static void InsertIncludeCode(String* SourceCode);
	static void GetIncludeFileName(String* SourceCode, std::vector<String>* ExternalShaders, std::vector<String>* InternalShaders);
	static ShaderType GetShaderType(const String& ShaderName);
	static void ExpandUniformProperty(String* SourceCode);
private:
	
	
};