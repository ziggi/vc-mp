//-----------------------------------------------------------------------------
// File: D3DFont.h
//
// Desc: Texture-based font class
//
// Copyright (c) 1999-2001 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef D3DFONT_H
#define D3DFONT_H
#include <tchar.h>
#include "d3d8.h"

// Font creation flags
#define D3DFONT_BOLD        0x0001
#define D3DFONT_ITALIC      0x0002
#define D3DFONT_ZENABLE     0x0004

// Font rendering flags
#define D3DFONT_CENTERED    0x0001
#define D3DFONT_TWOSIDED    0x0002
#define D3DFONT_FILTERED    0x0004
#define D3DFONT_INVERT		0x0005
#define D3DFONT_SHADOWSCALE 0x0006



//-----------------------------------------------------------------------------
// Name: class CD3DFont
// Desc: Texture-based font class for doing text in a 3D scene.
//-----------------------------------------------------------------------------
class CD3DFont
{
    TCHAR   m_strFontName[80];            // Font properties
    DWORD   m_dwFontFlags;

    LPDIRECT3DDEVICE8       m_pd3dDevice; // A D3DDevice used for rendering
    LPDIRECT3DTEXTURE8      m_pTexture;   // The d3d texture for this font
	LPDIRECT3DTEXTURE8		m_pTextureOutline;  //Outline for the text.
    LPDIRECT3DVERTEXBUFFER8 m_pVB;        // VertexBuffer for rendering text
    DWORD   m_dwTexWidth;                 // Texture dimensions
    DWORD   m_dwTexHeight;
    FLOAT   m_fTextScale;
    FLOAT   m_fTexCoords[128-32][4];

    // Stateblocks for setting and restoring render states
    DWORD   m_dwSavedStateBlock;
    DWORD   m_dwDrawTextStateBlock;
    DWORD   m_dwDrawBoxStateBlock;

	HRESULT PaintGlyphs( LPDIRECT3DTEXTURE8 pDestTexture, DWORD dwBoldFlag, DWORD dwItalicFlag );

public:
    DWORD   m_dwFontHeight;
	// 2D and 3D text drawing functions
    HRESULT DrawText( FLOAT x, FLOAT y, DWORD dwColor, 
                      const TCHAR* strText, DWORD dwFlags=0L, BOOL bIgnoreCodes = FALSE);
    HRESULT DrawTextScaled( FLOAT x, FLOAT y, FLOAT z, 
                            FLOAT fXScale, FLOAT fYScale, DWORD dwColor, 
                            TCHAR* strText, DWORD dwFlags=0L );
    HRESULT Render3DText( TCHAR* strText, DWORD dwFlags=0L,
                          float r=1.0f, float g=1.0f, float b=1.0f, float a=1.0f );

    HRESULT DrawBox(float xTop, float yTop, float fWidth, float fHeight, float pad, DWORD dwColorBox);
    HRESULT DrawTextBox(float xTop, float yTop, float pad, DWORD dwColorBox, DWORD dwColorTxt, const TCHAR* strText, DWORD dwFlags=0L);
    // Function to get extent of text
    HRESULT GetTextExtent( const TCHAR* strText, SIZE* pSize, FLOAT fMaxWidth=0.0f );
	HRESULT FormatText(const TCHAR* theText, TCHAR* outText, float fMaxWidth );
	HRESULT GetTextMax( const TCHAR* strText, const TCHAR** strResult, FLOAT fMaxWidth );
    // Initializing and destroying device-dependent objects
    HRESULT InitDeviceObjects( LPDIRECT3DDEVICE8 pd3dDevice );
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();

    // Constructor / destructor
    CD3DFont( TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags=0L );
    ~CD3DFont();
};

typedef struct _my_vertex{
    FLOAT x, y, z, rhw; // The transformed position for the vertex.
    DWORD color;        // The vertex color.
} my_vertex;

#endif


