// PUBLIC DOMAIN
/*
	Based off code by DrunkenHyena - Many thanks :)
*/

#include "bbfont.h"
#include <stdio.h>

extern D3DMATRIX matView;

CBBFont::CBBFont(IDirect3DDevice8* pDevice, char* pFileName)
{
	m_pDevice			= pDevice;
	m_pTexture			= NULL;
	m_pVB				= NULL;
	m_dwSavedStates		= 0;
	m_dwFontStates		= 0;
	m_fCharWidth		= 0.0f;
	m_fCharHeight		= 0.0f;
	m_fScale			= 0.01f;

	m_pTextureFileName = (char*)malloc(strlen(pFileName) + 5);
	sprintf(m_pTextureFileName, "%s.png", pFileName);
	m_pMetricsFileName = (char*)malloc(strlen(pFileName) + 5);
	sprintf(m_pMetricsFileName, "%s.dat", pFileName);
	memset((void*)&m_Metrics, 0, sizeof(m_Metrics));
}

CBBFont::~CBBFont()
{
	m_pDevice->DeleteStateBlock(m_dwSavedStates);
	m_pDevice->DeleteStateBlock(m_dwFontStates);
	free(m_pTextureFileName);
	free(m_pMetricsFileName);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pVB);
}

BOOL CBBFont::Initialise()
{
	if (!m_pDevice)
		return FALSE;

	D3DXCreateTextureFromFileEx(m_pDevice, m_pTextureFileName, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pTexture);

	m_pDevice->CreateVertexBuffer(((cMaxChars*6)+1)*sizeof(sVertex), D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, cVertexFVF, D3DPOOL_DEFAULT, &m_pVB);

	D3DSURFACE_DESC desc;
	m_pTexture->GetLevelDesc(0, &desc);
	m_fCharWidth = desc.Width / 16.0f;
	m_fCharHeight = desc.Height / 16.0f;

	FILE* f = fopen(m_pMetricsFileName, "rb");
	if (f)
	{
		fread(&m_Metrics, 256, 1, f);
		fclose(f);
	} else {
		memset(&m_Metrics, (int)m_fCharWidth, sizeof(m_Metrics));
	}

	return TRUE;
}

float CBBFont::Draw(char* pText, float fYOffset, DWORD dwColor)
{
	m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, dwColor);

	int len = (int)strlen(pText);
	if (len > cMaxChars)
		len = cMaxChars;

	sVertex *vert_ptr;
	m_pVB->Lock(0, 0, (BYTE**)&vert_ptr, D3DLOCK_DISCARD);

	float fX = 0.0f;
	for (int i=0; i<len; i++)
	{
		fX += (float)m_Metrics[pText[i]] * m_fScale;
	}
	fX = -(fX / 2);

	for (i=0; i<len; i++)
	{
		InitQuad(pText[i], fX, fYOffset, vert_ptr);
		fX += (float)m_Metrics[pText[i]] * m_fScale;
		vert_ptr += 6;
	}

	m_pVB->Unlock();

	m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, len*2);

	return fX;
}

void CBBFont::Begin()
{
	if(!m_dwSavedStates || !m_dwFontStates)
	{
		m_pDevice->BeginStateBlock();
		m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		m_pDevice->SetTexture(0, NULL);
		m_pDevice->SetStreamSource(0, NULL, 0);
		m_pDevice->SetVertexShader(cVertexFVF);
		m_pDevice->EndStateBlock(&m_dwSavedStates);

		m_pDevice->BeginStateBlock();
		m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pDevice->SetTexture(0, m_pTexture);
		m_pDevice->SetStreamSource(0, m_pVB, sizeof(sVertex));
		m_pDevice->SetVertexShader(cVertexFVF);
		m_pDevice->EndStateBlock(&m_dwFontStates);
	}

	m_pDevice->CaptureStateBlock(m_dwSavedStates);
	m_pDevice->ApplyStateBlock(m_dwFontStates);
}

void CBBFont::End()
{
	m_pDevice->ApplyStateBlock(m_dwSavedStates);
}

void CBBFont::InitQuad(char c, float x, float y2, sVertex* pVB)
{
   float x2  = x + ((float)m_Metrics[c] * m_fScale);
   float y  = y2 + (m_fCharHeight * m_fScale);
   float tu  = (c % 16) / 16.0f;
   float tu2 = tu + ((1 / 16.0f) * (m_Metrics[c] / m_fCharWidth));
   float tv  = (c - (c % 16)) / 256.0f;
   float tv2 = tv + 1 / 16.0f;

   //                x   y   tu   tv
   pVB[0].Initialise(x,  y2, tu,  tv2);
   pVB[1].Initialise(x,  y,  tu,  tv);
   pVB[2].Initialise(x2, y,  tu2, tv);
   pVB[3] = pVB[0];
   pVB[4] = pVB[2];
   pVB[5].Initialise(x2, y2, tu2, tv2);
}