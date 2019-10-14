#version 440 core

#define DirectLight 0
#define PointLight 1
#define SpotLight 2

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform int LightType;

uniform mat4 DirectShadowVPMatrix;
uniform mat4 PointShadowVPMatrices[6];
uniform mat4 SpotShadowVPMatrix;

out GS_to_FS
{
    int LightType;
}FS;

void main()
{
    FS.LightType = LightType;
    if(LightType == DirectLight)
    {
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            vec4 WorldPosition = gl_in[i].gl_Position;
            gl_Position = DirectShadowVPMatrix * WorldPosition;
            EmitVertex();
        }   
        EndPrimitive();
        return;
    }
    else if(LightType == PointLight)
    {
        for(int face = 0; face < 6; ++face)
        {
            gl_Layer = face; // built-in variable that specifies to which face we render.
            for(int i = 0; i < 3; ++i) // for each triangle's vertices
            {
                vec4 WorldPosition = gl_in[i].gl_Position;
                gl_Position = PointShadowVPMatrices[face] * WorldPosition;
                EmitVertex();
            }    
            EndPrimitive();
        }
        return;
    }
    else if(LightType == SpotLight)
    {
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            vec4 WorldPosition = gl_in[i].gl_Position;
            gl_Position = SpotShadowVPMatrix * WorldPosition;
            EmitVertex();
        }   
        EndPrimitive();
        return;
    }
}  