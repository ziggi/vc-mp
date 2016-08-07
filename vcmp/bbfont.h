// PUBLIC DOMAIN

#pragma once

#include "main.h"

#define cMaxChars 80
#define cVertexFVF D3DFVF_XYZ|D3DFVF_TEX1

#pragma pack(1)
class CBBFont
{
private:
	struct sVertex
	{
		float x, y, z, tu, tv;
		void Initialise(float fX, float fY, float fTu, float fTv)
		{
			x	= fX;
			y	= fY;
			z	= 0.0f;
			tu	= fTu;
			tv	= fTv;
		}
	};
	IDirect3DDevice8* m_pDevice;
	IDirect3DTexture8* m_pTexture;
	IDirect3DVertexBuffer8* m_pVB;

	DWORD m_dwSavedStates;
	DWORD m_dwFontStates;
	
	char* m_pTextureFileName;
	char* m_pMetricsFileName;

	char m_Metrics[255];
	float m_fCharWidth;
	float m_fCharHeight;
	float m_fScale;
public:
	CBBFont(IDirect3DDevice8* pDevice, char* pFileName);
	~CBBFont();

	BOOL Initialise();
	float Draw(char* pText, float fYOffset, DWORD dwColor);
	void Begin();
	void End();
	void InitQuad(char c, float x, float y, sVertex* pVB);
};
