#include "EngineRoot.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader()
{
}

Shader::~Shader()
{
	glDeleteShader(Id);
}

MaterialBase::MaterialBase()
{
	AddProperty("Path", STRING, &Path);
	AddPropertyArray("Shaders", STRING, &ShaderNames);
};

//MaterialBase::MaterialBase(const String& path, const std::vector<String>& shaderNames)
//	: Path(path)
//	, ShaderNames(shaderNames)
//{
//	AddProperty("Path", STRING, &Path);
//	ShaderCount = (int32)ShaderNames.size();
//	AddProperty("ShaderCount", INT_32, &ShaderCount);
//	for(int32 Index = 0; Index < ShaderCount; Index++)
//	{
//		ShaderNames[Index] = ShaderNames[Index];
//		AddProperty("Shader" + DataToString(Index), STRING, &ShaderNames[Index]);
//	}
//};

MaterialBase::~MaterialBase() 
{
};

String MaterialBase::GetPath()
{
	return Path;
}

void MaterialBase::Save(String* Data)
{
	PropertyBase::Save(Data);
}

Material::Material()
	: MaterialBase()
{

}

Material::Material(const String& name)
	: MaterialBase()
{
	Path = name;

	ShaderNames.resize(2);
	ShaderNames[0] = DKEngine::GetInstance().GetInternalShaderPath() + DefaultVertShaderName;
	ShaderNames[1] = DKEngine::GetInstance().GetInternalShaderPath() + DefaultFragShaderName;
	
	ProgramGPU = std::shared_ptr<Program>(new Program);
}

Material::Material(const String& name, const std::vector<String>& shaderNames)
	: MaterialBase()
{
	Path = name;
	ShaderNames = shaderNames;

	ProgramGPU = std::shared_ptr<Program>(new Program);

	LoadAndCreateShaders(ShaderNames);
	CreateGPUProgram();
	FindAttibInfos();
	FindUniformInfos();
	LinkLocation();
}

Material::~Material()
{
}

void Material::ReCreate()
{
	std::vector<String> SourceCodes = CreateShadersSourceCode();

}

void Material::CreateGPUShaders(const std::vector<String>& SrcCodes)
{
	for(int32 Index = 0; Index < ProgramGPU->Shaders.size(); Index++)
	{
		ProgramGPU->Shaders[Index].Id = CreateShaderGPUObjFromSrcCode(SrcCodes[Index], ProgramGPU->Shaders[Index].Type);
	}
}

void Material::CreateFromShaders(const std::vector<String>& shaderNames)
{
	ShaderNames = shaderNames;

	BindNewShaders(ShaderNames);
	ReCreate();

	std::vector<String> SourceCodes = CreateShadersSourceCode();

	CreateGPUProgram();
	FindAttibInfos();
	FindUniformInfos();
	LinkLocation();
}

void Material::BindNewShaders(const std::vector<String>& shaderNames)
{
	ShaderNames = shaderNames;
	ProgramGPU->Shaders.clear();
	ProgramGPU->Shaders.resize(shaderNames.size());
	for (int32 Index = 0; Index < ShaderNames.size(); Index++)
	{
		ProgramGPU->Shaders[Index].Type = ShaderHelper::GetShaderType(ShaderNames[Index]);
	}
}

void Material::BindProgram()
{
	glUseProgram(ProgramGPU->Id);
}
void Material::UnBindProgram()
{
	glUseProgram(0);
}
void Material::BindUniforms()
{
	//for (std::unordered_map<String, UniformItem_Basic>::iterator it = MaterialProgram->Uniforms_Basic.begin(); it != MaterialProgram->Uniforms_Basic.end(); it++)
	for(auto& uniformitempair: ProgramGPU->Uniforms_Basic)
	{
		auto& uniformitem = uniformitempair.second;
		if (uniformitem.DataPtr)
		{
			int32 Location      = uniformitem.Location;
			uint32 Size = uniformitem.Size;
			switch (uniformitem.DataType)
			{
			case UniformType::GLSL_VEC2: glUniform2fv(Location, Size, reinterpret_cast<GLfloat*>(uniformitem.DataPtr)); break;
			case UniformType::GLSL_VEC3: glUniform3fv(Location, Size, reinterpret_cast<GLfloat*>(uniformitem.DataPtr)); break;
			case UniformType::GLSL_VEC4: glUniform4fv(Location, Size, reinterpret_cast<GLfloat*>(uniformitem.DataPtr)); break;
			case UniformType::GLSL_IVEC2: glUniform2iv(Location, Size, reinterpret_cast<GLint*>(uniformitem.DataPtr)); break;
			case UniformType::GLSL_IVEC3: glUniform3iv(Location, Size, reinterpret_cast<GLint*>(uniformitem.DataPtr)); break;
			case UniformType::GLSL_IVEC4: glUniform4iv(Location, Size, reinterpret_cast<GLint*>(uniformitem.DataPtr)); break;
			case UniformType::GLSL_MAT3: glUniformMatrix3fv(Location, Size, GL_FALSE, reinterpret_cast<GLfloat*>(uniformitem.DataPtr)); break;
			case UniformType::GLSL_MAT4: glUniformMatrix4fv(Location, Size, GL_FALSE, reinterpret_cast<GLfloat*>(uniformitem.DataPtr)); break;
			case UniformType::GLSL_FLOAT: glUniform1fv(Location, Size, reinterpret_cast<GLfloat*>(uniformitem.DataPtr)); break;
			case UniformType::GLSL_INT:	glUniform1iv(Location, Size, reinterpret_cast<GLint*>(uniformitem.DataPtr)); break;
			default:
				break;
			}
		}
	}
}
void Material::BindSamplers()
{
	//for (std::unordered_map<String, UniformItem_Texture>::iterator it = MaterialProgram->Uniforms_Texture.begin(); it != MaterialProgram->Uniforms_Texture.end(); it++)
	for (auto& uniformitempair : ProgramGPU->Uniforms_Texture)
	{
		auto& uniformitem = uniformitempair.second;
		int32 Location      = uniformitem.Location;
		void* IDPtr       = uniformitem.IDPtr;
		glActiveTexture(GL_TEXTURE0 + Location);
		switch (uniformitem.DataType)
		{
		case UniformType::GLSL_TEXTURE2D:
			glBindTexture(GL_TEXTURE_2D, *((uint32*)IDPtr));
			break;
		case UniformType::GLSL_TEXTURECUBE:
			glBindTexture(GL_TEXTURE_CUBE_MAP, *((uint32*)IDPtr));
			break;
		case UniformType::GLSL_TEXTURE3D:
			glBindTexture(GL_TEXTURE_3D, *((uint32*)IDPtr));
			break;
		case UniformType::GLSL_TEXTURE_2D_ARRAY:
			glBindTexture(GL_TEXTURE_2D_ARRAY, *((uint32*)IDPtr));
			break;
		default:
			break;
		}
	}
}
void Material::Draw(uint32 VAO, int32 NumFaces, IndexSizeType indexSize, int32 Offset, GLDrawType drawType)
{
	BindProgram();
	BindUniforms();
	BindSamplers();

	int32 IndexSize = 0;
	int32 GL_IndexType = 0;
	switch (indexSize)
	{
	case Index16Bits:
		IndexSize = 2;
		GL_IndexType = GL_UNSIGNED_SHORT;
		break;
	case Index32Bits:
		IndexSize = 4;
		GL_IndexType = GL_UNSIGNED_INT;
		break;
	default:
		break;
	}
	glBindVertexArray(VAO);
	//glPolygonOffset
	switch (drawType)
	{
	case Unkown:
		break;
	case OGL_ELEMENT:
		glDrawElements(GL_TRIANGLES, NumFaces * 3, GL_IndexType, reinterpret_cast<void*>((int64)Offset * IndexSize));
		break;
	case OGL_LINE_STRIP: glDrawElements(GL_LINE_STRIP, NumFaces * 3, GL_IndexType, reinterpret_cast<void*>((int64)Offset * IndexSize));
		break;
	default:
		break;
	}
	glBindVertexArray(0);
}

void Material::LoadAndCreateShaders(std::vector<String>& shaderNames)
{
	ProgramGPU->Shaders = std::vector<Shader>(shaderNames.size());

	for (uint32 i = 0; i < shaderNames.size(); i++)
	{
		std::ifstream ShaderFile;
		ShaderFile.open(shaderNames[i]);

		if (!ShaderFile)
		{
			std::cout << "Open Shader: " << shaderNames[i] << " Fail" << std::endl;
		}

		std::stringstream ShaderStream;
		ShaderStream << ShaderFile.rdbuf();
		String SourceCode = ShaderStream.str();

		int32 loc =(int32) shaderNames[i].rfind('.');
		const int8 ShaderType = shaderNames[i][loc + 1];

		switch (ShaderType)
		{
		case 'v': { ProgramGPU->Shaders[i].Id = CreateShaderGPUObjFromSrcCode(SourceCode, ShaderType::VertexShader);
			ProgramGPU->Shaders[i].Type = ShaderType::VertexShader;
			break; }
		case 'f': { ProgramGPU->Shaders[i].Id = CreateShaderGPUObjFromSrcCode(SourceCode, ShaderType::FragmentShader);
			ProgramGPU->Shaders[i].Type = ShaderType::FragmentShader;
			break; }
		case 'g': { ProgramGPU->Shaders[i].Id = CreateShaderGPUObjFromSrcCode(SourceCode, ShaderType::GeometryShader);
			ProgramGPU->Shaders[i].Type = ShaderType::GeometryShader;
			break; }
		case 'e': { ProgramGPU->Shaders[i].Id = CreateShaderGPUObjFromSrcCode(SourceCode, ShaderType::TessEvaluationShader);
			ProgramGPU->Shaders[i].Type = ShaderType::TessEvaluationShader;
			break; }
		case 'c': {
			if (shaderNames[i][loc - 1] == 't') {
				ProgramGPU->Shaders[i].Id = CreateShaderGPUObjFromSrcCode(SourceCode, ShaderType::TessControlShader);
				ProgramGPU->Shaders[i].Type = ShaderType::TessControlShader;
			}
			else {
				ProgramGPU->Shaders[i].Id = CreateShaderGPUObjFromSrcCode(SourceCode, ShaderType::ComputeShader);
				ProgramGPU->Shaders[i].Type = ShaderType::ComputeShader;
			}
			break;
		}
		default:
			break;
		}
	}
}

std::vector<String> Material::CreateShadersSourceCode()
{
	std::vector<String> ShadersCode;
	for (uint32 i = 0; i < ProgramGPU->Shaders.size(); i++)
	{
		String ShaderName = ShaderNames[i];
		std::ifstream ShaderFile;
		ShaderFile.open(ShaderName);

		if (!ShaderFile)
		{
			std::cout << "Open Shader: " << ShaderName << " Fail" << std::endl;
		}

		std::stringstream ShaderStream;
		ShaderStream << ShaderFile.rdbuf();
		String SourceCode = ShaderStream.str();

		ShaderHelper::InsertIncludeCode(&SourceCode);
		
		ShadersCode.push_back(SourceCode);
	}
	return ShadersCode;
}

void Material::CreateShadersFromSrcCode(const std::vector<String>& SourceCodes)
{
	for(int32 Index = 0 ; Index < ShaderNames.size(); Index++)
	{
		ShaderType Type = ShaderHelper::GetShaderType(ShaderNames[Index]);
		ProgramGPU->Shaders[Index].Type = Type;
		ProgramGPU->Shaders[Index].Id = CreateShaderGPUObjFromSrcCode(SourceCodes[Index], Type);
	}
}

void Material::CreateGPUProgram()
{
	ProgramGPU->Id = glCreateProgram();
	for (uint32 i = 0; i < ProgramGPU->Shaders.size(); i++)
	{
		glAttachShader(ProgramGPU->Id, ProgramGPU->Shaders[i].Id);
	}
	glLinkProgram(ProgramGPU->Id);

	GLint glStatus;

	glGetProgramiv(ProgramGPU->Id, GL_LINK_STATUS, &glStatus);
	String default_infolog;
	String& infolog = default_infolog;
	if (!glStatus)
	{
		int32_t infoLogLength, charWriten;
		glGetProgramiv(ProgramGPU->Id, GL_INFO_LOG_LENGTH, &infoLogLength);
		infolog.resize(infoLogLength);
		if (infoLogLength)
		{
			glGetProgramInfoLog(ProgramGPU->Id, infoLogLength, &charWriten, &infolog[0]);
			std::cout << "Failed to link program with infolog" << infolog << std::endl;
		}
		std::cout << "Failed to link shader" << std::endl;
	}
}

void Material::FindAttibInfos()
{
	int32 AttribCount;
	int32 AttribName_MaxLength;
	glGetProgramiv(ProgramGPU->Id, GL_ACTIVE_ATTRIBUTES, &AttribCount);
	glGetProgramiv(ProgramGPU->Id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &AttribName_MaxLength);
	int8 * AttribName = new int8[AttribName_MaxLength];
	for (int32 AttribIndex = 0; AttribIndex < AttribCount; AttribIndex++)
	{
		AttribItem Attrib;
		int32 NameLength;
		int32 AttribSize;
		uint32 AttribType;
		glGetActiveAttrib(ProgramGPU->Id, AttribIndex, AttribName_MaxLength, &NameLength, &AttribSize, &AttribType, AttribName);
		ProgramGPU->Attribs.insert(std::pair<String, AttribItem>(String(AttribName), Attrib));
	}
	delete[] AttribName;
}

void Material::FindUniformInfos()
{
	int32 UniformBlockCount;
	glGetProgramiv(ProgramGPU->Id, GL_ACTIVE_UNIFORM_BLOCKS, &UniformBlockCount);
	if (UniformBlockCount > 0) {
		int32 UniformBlockName_MaxLength;
		glGetProgramiv(ProgramGPU->Id, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &UniformBlockName_MaxLength);
		int8* UniformBlockName = new int8[UniformBlockName_MaxLength];
		for (int32 UniformBlockIndex = 0; UniformBlockIndex < UniformBlockCount; UniformBlockIndex++)
		{
			int32 NameLength;
			glGetActiveUniformBlockName(ProgramGPU->Id, UniformBlockIndex, UniformBlockName_MaxLength, &NameLength, UniformBlockName);

			int32 UniformCountInBlock;
			glGetActiveUniformBlockiv(ProgramGPU->Id, UniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &UniformCountInBlock);
			int32* InUniformIndices = new int32[UniformCountInBlock];
			glGetActiveUniformBlockiv(ProgramGPU->Id, UniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, InUniformIndices);
			int32 UniformInBlockName_MaxLength;
			glGetProgramiv(ProgramGPU->Id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &UniformInBlockName_MaxLength);

			int32 DataSize;
			glGetActiveUniformBlockiv(ProgramGPU->Id, UniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &DataSize);

			std::hash<String> hs;
			std::shared_ptr<UniformItem_Block> Block = std::shared_ptr<UniformItem_Block>(new UniformItem_Block());
			Block->DataSize_Byte = DataSize;
			Block->Index = UniformBlockIndex;
			ProgramGPU->Uniforms_Block.insert(std::pair<String, std::shared_ptr<UniformItem_Block>>(UniformBlockName, Block));

			int8* UniformInBlockName = new int8[UniformInBlockName_MaxLength];
			for (int32 Index = 0; Index < UniformCountInBlock; Index++)
			{
				uint32 InUniformIndex = InUniformIndices[Index];

				int32 NameLength;
				int32 Size;
				uint32 Type;
				int32 Offset;
				glGetActiveUniform(ProgramGPU->Id, InUniformIndex, UniformInBlockName_MaxLength, &NameLength, &Size, &Type, UniformInBlockName);
				glGetActiveUniformsiv(ProgramGPU->Id, 1, &InUniformIndex, GL_UNIFORM_OFFSET, &Offset);

				UniformInBlockName[NameLength - 3] = Size > 1 ? '\0' : UniformInBlockName[NameLength - 3]; //Multi Size variable's name is "abcd[0]". We delete [0] here, and we can also get location with "abcd".

				UniformItem_WithinBlock UniformInBlock;
				UniformInBlock.Name = UniformInBlockName;
				UniformInBlock.DataType = UniformTypeMap[Type];
				UniformInBlock.Offset_Byte = Offset;
				UniformInBlock.Size = Size;
				Block->Uniforms.insert(std::pair<uint32, UniformItem_WithinBlock>(UniformInBlock.Offset_Byte, UniformInBlock));
			}
			delete[] UniformInBlockName;
			delete[] InUniformIndices;
		}
		delete[] UniformBlockName;
		
		std::unordered_map<String, std::shared_ptr<UniformItem_Block>>::iterator it;
		for(it = ProgramGPU->Uniforms_Block.begin(); it != ProgramGPU->Uniforms_Block.end(); it++)
		{
			it->second->Id = DKEngine::GetInstance().GetGPUBufferManager()->CreateUniformBuffer(it->first, it->second);
			glBindBuffer(GL_UNIFORM_BUFFER, it->second->Id);
			glUniformBlockBinding(ProgramGPU->Id, it->second->Index, DKEngine::GetInstance().GetGPUBufferManager()->GetUniformBlockBindingPoint(it->first));
			glBindBufferBase(GL_UNIFORM_BUFFER, DKEngine::GetInstance().GetGPUBufferManager()->GetUniformBlockBindingPoint(it->first), it->second->Id);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);			
		}
	}

	int32 UniformCount;
	glGetProgramiv(ProgramGPU->Id, GL_ACTIVE_UNIFORMS, &UniformCount);
	int32 UniformName_MaxLength;
	glGetProgramiv(ProgramGPU->Id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &UniformName_MaxLength);
	int8* UniformName = new int8[UniformName_MaxLength];
	for (int32 UniformIndex = 0; UniformIndex < UniformCount; UniformIndex++)
	{
		int32 NameLength;
		int32 Size;
		uint32 Type;
		glGetActiveUniform(ProgramGPU->Id, UniformIndex, UniformName_MaxLength, &NameLength, &Size, &Type, UniformName);

		UniformName[NameLength - 3] = Size > 1 ? '\0' : UniformName[NameLength - 3]; //Multi Size variable's name is "abcd[0]". We delete [0] here, and we can also get location with "abcd".

		if (-1 == glGetUniformLocation(ProgramGPU->Id, UniformName)) continue;	//Uniform is in uniform block	

		UniformType T = UniformTypeMap[Type];
		if (T < UniformType::GLSL_TEXTURE2D)
		{
			UniformItem_Basic UniformBasic;
			UniformBasic.Size = Size;
			UniformBasic.DataType = T;
			ProgramGPU->Uniforms_Basic.insert(std::pair<String, UniformItem_Basic>(UniformName, UniformBasic));
		}
		else
		{
			UniformItem_Texture UniformTexture;
			UniformTexture.DataType = T;
			ProgramGPU->Uniforms_Texture.insert(std::pair<String, UniformItem_Texture>(UniformName, UniformTexture));
		}
	}
	delete[] UniformName;
}

void Material::LinkLocation()
{
	glUseProgram(ProgramGPU->Id);
	for (std::unordered_map<String, AttribItem>::iterator it = ProgramGPU->Attribs.begin(); it != ProgramGPU->Attribs.end(); it++)
	{
		String name = it->first;;
		GLint loc = glGetAttribLocation(ProgramGPU->Id, name.c_str());
		ProgramGPU->Attribs[name].Location = loc;
	}

	for (std::unordered_map<String, UniformItem_Basic>::iterator it = ProgramGPU->Uniforms_Basic.begin(); it != ProgramGPU->Uniforms_Basic.end(); it++)
	{
		String name = it->first;
		GLint loc = glGetUniformLocation(ProgramGPU->Id, name.c_str());
		ProgramGPU->Uniforms_Basic[name].Location = loc;
	}

	int32 newLoc;
	std::unordered_map<String, UniformItem_Texture>::iterator it;
	for (it = ProgramGPU->Uniforms_Texture.begin(), newLoc = 0; it != ProgramGPU->Uniforms_Texture.end(); it++, newLoc++)
	{
		String name = it->first;
		GLint loc = glGetUniformLocation(ProgramGPU->Id, name.c_str());
		if (loc == -1) continue;
		glUniform1i(loc, newLoc);
		ProgramGPU->Uniforms_Texture[name].Location = newLoc;
	}

	glUseProgram(0);
}

uint32 Material::CreateShaderGPUObjFromSrcCode(const String & srcCode, ShaderType type)
{
	uint32 outShader = 0;

	switch (type)
	{
	case ShaderType::VertexShader:
		outShader = glCreateShader(GL_VERTEX_SHADER);
		break;
	case ShaderType::FragmentShader:
		outShader = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case ShaderType::ComputeShader:
		outShader = glCreateShader(GL_COMPUTE_SHADER);
		break;
	case ShaderType::GeometryShader:
		outShader = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case ShaderType::TessControlShader:
		outShader = glCreateShader(GL_TESS_CONTROL_SHADER);
		break;
	case ShaderType::TessEvaluationShader:
		outShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		break;
	default:
		std::cout << "loadShader: Unknown shader type requested." << std::endl;
	}

	const int8* vShaderCode = srcCode.c_str();
	glShaderSource(outShader, 1, &vShaderCode, NULL);
	glCompileShader(outShader);

	GLint success;
	GLchar infoLog[1024];
	glGetShaderiv(outShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(outShader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}

	return outShader;
}

void Material::Save(String* Data)
{
	PropertyBase::Save(Data);
	std::vector<String> SourceCodes = CreateShadersSourceCode();
	for(int32 ShaderIndex = 0; ShaderIndex < ProgramGPU->Shaders.size(); ShaderIndex++)
	{
		SaveShaderSourceCode(Data, ProgramGPU->Shaders[ShaderIndex].Type, SourceCodes[ShaderIndex]);
	}
}

void Material::Load(const String& Data)
{
	LoadBaseInfo(Data);
	ProgramGPU->Shaders.resize(ShaderNames.size());
	std::vector<String> SourceCodes = LoadShaderSourceCode(Data);
	assert(ShaderNames.size() == SourceCodes.size());

	CreateShadersFromSrcCode(SourceCodes);
	CreateGPUProgram();
	FindAttibInfos();
	FindUniformInfos();
	LinkLocation();
}

void Material::SaveShaderSourceCode(String* OutData, ShaderType Type, const String& SourceCode)
{
	static String SplitTag = "\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"\
							 "\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
	OutData->append(SplitTag);
	switch (Type)
	{
	case VertexShader: OutData->append(TO_String(VertexShader)); break;
	case FragmentShader: OutData->append(TO_String(FragmentShader)); break;
	case GeometryShader: OutData->append(TO_String(GeometryShader)); break;
	case TessEvaluationShader: OutData->append(TO_String(TessEvaluationShader)); break;
	case TessControlShader: OutData->append(TO_String(TessControlShader)); break;
	case ComputeShader: OutData->append(TO_String(ComputeShader)); break;
	default:
		break;
	}
	OutData->append(SplitTag + "\n");
	OutData->append(SourceCode);
}


std::vector<String> Material::LoadShaderSourceCode(const String& SourceCode)
{
	std::vector<String> ShaderSourceCodes;
	String SubCode = SourceCode;
	static String SplitTag = "\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"\
							"\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
	while (true)
	{
		int32 Pos0 = SubCode.find(SplitTag);
		if (Pos0 == -1) break;
		Pos0 = SubCode.find(SplitTag, SplitTag.length());
		int32 Pos1 = SubCode.find(SplitTag, (int64)Pos0 + 1);
		if (Pos1 == -1) Pos1 = SubCode.length();
		String ShaderStr = SubCode.substr(Pos0 + SplitTag.length(), (int64)Pos1 - Pos0 - SplitTag.length());
		ShaderSourceCodes.push_back(ShaderStr);
		SubCode.erase(0, Pos1);
	}
	return ShaderSourceCodes;
}

void Material::LoadBaseInfo(const String& SourceCode)
{
	static String SplitTag = "\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"\
							"\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
	int32 BaseInfoPos = SourceCode.find(SplitTag);
	String BaseInfo = SourceCode.substr(0, BaseInfoPos);
	PropertyBase::Load(BaseInfo);
}