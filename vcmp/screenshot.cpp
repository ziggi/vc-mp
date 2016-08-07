//----------------------------------------------------
//
// GPL code license:
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author: kyeman
//
// -Portions based on public domain code by Michael Fötsch
// -Portions based on public domain example code by the
// Independant JPEG Group.
//
//----------------------------------------------------


#include <stdio.h>
#include <windows.h>
#include "d3d8/include/d3dfont.h"
#include "screenshot.h"

#define SAFERELEASE(x) if (x) { x->Release(); x = NULL; x; }

//----------------------------------------------------

CScreenshot::CScreenshot(IDirect3DDevice8 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
}

//----------------------------------------------------

BOOL CScreenshot::TakeScreenShot(PCHAR szFileName)
{
	HANDLE hDib;
	BOOL bRet=FALSE;

	if(m_pD3DDevice)
	{
		hDib = CreateDibFromCurrentSurface();
		if(NULL == hDib) return FALSE;
	}	

	bRet = CreateJpegFileFromDib(hDib,90,szFileName);
	FreeDib(hDib);

	return bRet;
}

//----------------------------------------------------

HANDLE CScreenshot::CreateDibFromCurrentSurface()
{
	LPDIRECT3DSURFACE8		pSurface = 0;
    LPDIRECT3D8				pD3D = 0;
    BYTE					*pbyteDibBuffer = 0;

    D3DDEVICE_CREATION_PARAMETERS dcp;

    dcp.AdapterOrdinal = D3DADAPTER_DEFAULT;
    m_pD3DDevice->GetCreationParameters(&dcp);

    D3DDISPLAYMODE dm;
    dm.Width = dm.Height = 0;

    m_pD3DDevice->GetDirect3D(&pD3D);
    if(pD3D) {   
		// get the current screen dimensions
        pD3D->GetAdapterDisplayMode(dcp.AdapterOrdinal, &dm);
        SAFERELEASE(pD3D);
    }
	else {
		return NULL;
	}
	
	// Grab the ARBG surface.
    m_pD3DDevice->CreateImageSurface(dm.Width,dm.Height,D3DFMT_A8R8G8B8,&pSurface);
	m_pD3DDevice->GetFrontBuffer(pSurface);

    // Setup the dib header.
    BITMAPINFOHEADER bmih;
    bmih.biSize = sizeof(bmih);
    bmih.biWidth = dm.Width;
    bmih.biHeight = dm.Height;
    bmih.biPlanes = 1;
    bmih.biBitCount = 24;
    bmih.biCompression = BI_RGB;
    bmih.biSizeImage = dm.Width * dm.Height * 3;
    bmih.biXPelsPerMeter = 0;
    bmih.biYPelsPerMeter = 0;
    bmih.biClrUsed = 0;
    bmih.biClrImportant = 0;

   	pbyteDibBuffer = (PBYTE)calloc((bmih.biSizeImage+sizeof(BITMAPINFOHEADER)+1),1);
    if(!pbyteDibBuffer) return NULL;

	// Lock the surface.
    D3DLOCKED_RECT LockedRect;
    pSurface->LockRect(&LockedRect,NULL,D3DLOCK_READONLY);
	
	// Copy the bitmap info header into the dib buffer.
    memcpy(pbyteDibBuffer,&bmih,sizeof(BITMAPINFOHEADER));

    PDWORD lpSrc;
    PBYTE lpDest = pbyteDibBuffer+sizeof(BITMAPINFOHEADER);

    for(int y = dm.Height - 1; y >= 0; y--) 
	{
        lpSrc = (PDWORD)(LockedRect.pBits) + y * dm.Width;
		for(int x = 0; x < (signed)dm.Width; x++) 
		{
			// Store the pixels
            *(DWORD *)(lpDest) = *lpSrc;
            lpSrc++;
            lpDest += 3;
        }
    }

    // we can unlock and release the surface
    pSurface->UnlockRect();
    SAFERELEASE(pSurface);

	return pbyteDibBuffer;
}

//----------------------------------------------------

void CScreenshot::FreeDib(HANDLE hDib)
{
	free(hDib);
}

//-----------------------------------------------------------------

BOOL CScreenshot::CreateJpegFileFromDib(HANDLE hDib, int nQuality, PCHAR szJpegFile)
{
    LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)hDib;
    byte *buf2 = 0;

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr       jerr;

    FILE		*pOutFile;
    int         nSampsPerRow;
    JSAMPARRAY  jsmpArray;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    if((pOutFile = fopen(szJpegFile, "wb")) == NULL) {
		jpeg_destroy_compress(&cinfo);
        return FALSE;
    }

    jpeg_stdio_dest(&cinfo, pOutFile);

    cinfo.image_width      = lpbi->biWidth;
    cinfo.image_height     = lpbi->biHeight;
    cinfo.input_components = 3;
    cinfo.in_color_space   = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo,nQuality,TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    //JSAMPLEs per row in output buffer
    nSampsPerRow = cinfo.image_width * cinfo.input_components; 

    //Allocate array of pixel RGB values
    jsmpArray = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo,JPOOL_IMAGE,
		nSampsPerRow,cinfo.image_height);

	ConvertDibToSamps(hDib,nSampsPerRow,cinfo,jsmpArray);
    jpeg_write_scanlines(&cinfo,jsmpArray,cinfo.image_height);

    jpeg_finish_compress(&cinfo);
	fclose(pOutFile);
    jpeg_destroy_compress(&cinfo);
	return TRUE;
}

//----------------------------------------------------

void CScreenshot::ConvertDibToSamps(HANDLE hDib,int nSampsPerRow,
									struct jpeg_compress_struct cinfo,
									JSAMPARRAY jsmpPixels)
{
   int r=0, p=0, q=0, b=0, n=0, 
       nUnused=0, nBytesWide=0, nUsed=0, nLastBits=0, nLastNibs=0, nCTEntries=0,
       nRow=0, nByte=0, nPixel=0;

   BYTE bytCTEnt=0;
   LPBITMAPINFOHEADER pbBmHdr= (LPBITMAPINFOHEADER)hDib; //The bit count tells you the format of the bitmap: //Decide how many entries will be in the color table (if any) 

   nCTEntries = 0;   //No color table needed

   //Point to the color table and pixels
   DWORD     dwCTab = (DWORD)pbBmHdr + pbBmHdr->biSize;
   LPRGBQUAD pCTab  = (LPRGBQUAD)(dwCTab);
   LPSTR     lpBits = (LPSTR)pbBmHdr +
                      (WORD)pbBmHdr->biSize +
                      (WORD)(nCTEntries * sizeof(RGBQUAD));

   LPBYTE   lpPixels = (LPBYTE)  lpBits;
   RGBQUAD* pRgbQs   = (RGBQUAD*)lpBits;
   WORD*    wPixels  = (WORD*)   lpBits;

   nBytesWide =  (pbBmHdr->biWidth*3);
   nUnused    =  (((nBytesWide + 3) / 4) * 4) - nBytesWide;
   nBytesWide += nUnused;

   for(r=0; r < (pbBmHdr->biHeight);r++)
   {
      for(p=0,q=0;p < (nBytesWide-nUnused); p+=3,q+=3)
	  { 
          nRow = (pbBmHdr->biHeight-r-1) * nBytesWide;
          nPixel  = nRow + p;

          jsmpPixels[r][q+0] = lpPixels[nPixel+2]; //Red
          jsmpPixels[r][q+1] = lpPixels[nPixel+1]; //Green
          jsmpPixels[r][q+2] = lpPixels[nPixel+0]; //Blue
       }
   }
}

//----------------------------------------------------
// EOF