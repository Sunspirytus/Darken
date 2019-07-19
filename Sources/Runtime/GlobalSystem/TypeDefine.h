#pragma once

#include "glm.hpp"
#include "half.hpp"
#include "glad.h"
#include "glfw3.h"

using half_float::half;

namespace dk
{
	typedef bool Bool;

	typedef half float16;
	typedef float float32;
	typedef double float64;

	typedef char int8;
	typedef short int16;
	typedef int int32;
	typedef long long int64;
	typedef long long Address;

	typedef unsigned char uint8;
	typedef unsigned char Byte;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;

	typedef glm::fvec2 Vector2f;
	typedef glm::fvec3 Vector3f;
	typedef glm::fvec4 Vector4f;
	
	typedef glm::dvec2 Vector2d;
	typedef glm::dvec3 Vector3d;
	typedef glm::dvec4 Vector4d;
	
	typedef glm::i32vec2 Vector2i;
	typedef glm::i32vec3 Vector3i;
	typedef glm::i32vec4 Vector4i;

	typedef glm::fmat2 Mat2f;
	typedef glm::fmat3 Mat3f;
	typedef glm::fmat4 Mat4f;
	
	typedef glm::dmat2 Mat2d;
	typedef glm::dmat3 Mat3d;
	typedef glm::dmat4 Mat4d;
}

using namespace dk;