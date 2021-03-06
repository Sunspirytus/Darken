#version 440 core

#define DirectLight 0
#define PointLight 1
#define SpotLight 2

precision highp float;

uniform	vec4 LightCamera_ZBufferParams; // x = near, y = far, z = fov, w = Aspect

in GS_to_FS
{
	flat int LightType;
}FS;

out float Depth01;

void main()
{
	float LinearDepth01;
	if(FS.LightType == DirectLight)
	{
		LinearDepth01 =  gl_FragCoord.z / gl_FragCoord.w * 0.5 + 0.5;
	}else{
		LinearDepth01 = (2.0 * LightCamera_ZBufferParams.x) / (LightCamera_ZBufferParams.y + LightCamera_ZBufferParams.x - (gl_FragCoord.z * 2.0 - 1.0) *(LightCamera_ZBufferParams.y - LightCamera_ZBufferParams.x));
	}
	//Depth01.x = floor(LinearDepth01 * 1000.0);
	//Depth01.y = clamp(LinearDepth01 * 1000.0 - floor(LinearDepth01 * 1000.0), 0.0, 1.0) * 1000.0;
	Depth01 = LinearDepth01;
}
