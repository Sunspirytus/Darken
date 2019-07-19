#include "SystemTextures.h"
#include "Util.h"

#ifndef PI
#define PI 3.1415926535897932384626433832795028841968f
#endif // !PI

SystemTextureFactory::SystemTextureFactory()
{
}

SystemTextureFactory::~SystemTextureFactory()
{
	glDeleteTextures(1, &PreIntegratedGF_Tex);
}

void SystemTextureFactory::GeneratePreIntegratedGFTexture()
{
	uint32 Width = 128;
	uint32 Height = 32;
	uint32 DestStride = Width * 2;

	float16 * data = new float16[Width * Height * 2];
	memset(data, 0, (uint64)Width * Height * 2 * sizeof(float16));

	for (uint32 y = 0; y < Height; y++)
	{
		float32 Roughness = (float32)(y + 0.5f) / Height;
		float32 m = Roughness * Roughness;
		float32 m2 = m * m;

		for (uint32 x = 0; x < Width; x++)
		{
			float32 NoV = (float32)(x + 0.5f) / Width;

			Vector3f V;
			V.x = sqrt(1.0f - NoV * NoV);	// sin
			V.y = 0.0f;
			V.z = NoV;								// cos

			float32 A = 0.0f;
			float32 B = 0.0f;
			float32 C = 0.0f;

			const uint32 NumSamples = 128;
			for (uint32 i = 0; i < NumSamples; i++)
			{
				float32 E1 = (float32)i / NumSamples;
				float32 E2 = (float32) ((float64)ReverseBits(i) / (float64)0x100000000LL);

				{
					float32 Phi = 2.0f * PI * E1;
					float32 CosPhi = cos(Phi);
					float32 SinPhi = sin(Phi);
					float32 CosTheta = sqrt((1.0f - E2) / (1.0f + (m2 - 1.0f) * E2));
					float32 SinTheta = sqrt(1.0f - CosTheta * CosTheta);

					Vector3f H(SinTheta * cos(Phi), SinTheta * sin(Phi), CosTheta);
					Vector3f L = 2.0f * (Math::Dot(V, H)) * H - V;

					float32 NoL = Math::Max(L.z, 0.0f);
					float32 NoH = Math::Max(H.z, 0.0f);
					float32 VoH = Math::Max(Math::Dot(V, H), 0.0f);

					if (NoL > 0.0f)
					{
						float32 Vis_SmithV = NoL * (NoV * (1 - m) + m);
						float32 Vis_SmithL = NoV * (NoL * (1 - m) + m);
						float32 Vis = 0.5f / (Vis_SmithV + Vis_SmithL);

						float32 NoL_Vis_PDF = NoL * Vis * (4.0f * VoH / NoH);
						float32 Fc = 1.0f - VoH;
						Fc *= pow(Fc*Fc, 2.0f);
						A += NoL_Vis_PDF * (1.0f - Fc);
						B += NoL_Vis_PDF * Fc;
					}
				}

				{
					float32 Phi = 2.0f * PI * E1;
					float32 CosPhi = cos(Phi);
					float32 SinPhi = sin(Phi);
					float32 CosTheta = sqrt(E2);
					float32 SinTheta = sqrt(1.0f - CosTheta * CosTheta);

					Vector3f L(SinTheta * cos(Phi), SinTheta * sin(Phi), CosTheta);
					Vector3f H = Math::Normalize(V + L);

					float32 NoL = Math::Max(L.z, 0.0f);
					float32 NoH = Math::Max(H.z, 0.0f);
					float32 VoH = Math::Max(Math::Dot(V, H), 0.0f);

					float32 FD90 = 0.5f + 2.0f * VoH * VoH * Roughness;
					float32 FdV = 1.0f + (FD90 - 1.0f) * pow(1.0f - NoV, 5);
					float32 FdL = 1.0f + (FD90 - 1.0f) * pow(1.0f - NoL, 5);
					C += FdV * FdL;// * ( 1.0f - 0.3333f * Roughness );
				}
			}
			A /= NumSamples;
			B /= NumSamples;
			C /= NumSamples;

			float16* Dest = (float16*)(data + (uint64)x * 2 + (uint64)y * DestStride);
			Dest[0] = (float16)A;
			Dest[1] = (float16)B;
		}
	}

	glGenTextures(1, &PreIntegratedGF_Tex);
	glBindTexture(GL_TEXTURE_2D, PreIntegratedGF_Tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, Width, Height, 0, GL_RG, GL_HALF_FLOAT, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	delete [] data;
}

uint32 SystemTextureFactory::GetPreIntegratedGF_Tex()
{
	return PreIntegratedGF_Tex;
}