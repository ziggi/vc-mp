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
// Portions by spookie and Kryptos
//
//----------------------------------------------------

#include "main.h"

CGame					*pGame=0;
DWORD					dwGameLoop=0;
DWORD					dwRenderLoop=0;
GAME_SETTINGS			tSettings;
CChatWindow				*pChatWindow=0;
CCmdWindow				*pCmdWindow=0;
CNetGame				*pNetGame=0;

BOOL					bGameInited=FALSE;
BOOL					bWindowedMode=FALSE;
BOOL					bShowNameTags=TRUE;
BOOL					bAntiCheat=TRUE;

IDirect3D8				*pD3D;
IDirect3D8Hook			*pD3DHook;
IDirect3DDevice8		*pD3DDevice;
IDirect3DDevice8Hook	*pD3DDeviceHook;

int						iStartGameDelay=200;
HANDLE					hInstance;
CScoreBoard				*pScoreBoard;
CNetStats				*pNetStats;

// forwards

BOOL SubclassGameWindow();
void SetupCommands();

void TheGameLoop();
void TheRenderLoop();

void GameDebugDrawDebugScreens();
LONG WINAPI exc_handler(_EXCEPTION_POINTERS* exc_inf);
int DetermineGTAVersion();
void HookD3DReset(IDirect3DDevice8 *pD3DDeviceHook);

#define UNKNOWN_VERSION	0
#define VICE_10			1
#define VICE_11			2

D3DMATRIX matView;
CBBFont* bbfont = 0;
DWORD HealthBarOldStateBlock = 0;
DWORD HealthBarNewStateBlock = 0;

struct HealthBarVertices_s
{
	float x, y, z;
	D3DCOLOR c;
};

HealthBarVertices_s HealthBarBDRVertices[4] = {
	//  x     y      z     rhw   c
	{-0.3f,  -0.03f, 0.0f, D3DCOLOR_XRGB(0, 0, 0)},  // 1 . . 4
	{-0.3f,   0.03f, 0.0f, D3DCOLOR_XRGB(0, 0, 0)},  // 2 . . 3
	{ 0.3f,   0.03f, 0.0f, D3DCOLOR_XRGB(0, 0, 0)},
	{ 0.3f,  -0.03f, 0.0f, D3DCOLOR_XRGB(0, 0, 0)}
};

HealthBarVertices_s HealthBarBGVertices[4] = {
	//  x     y      z     rhw   c
	{-0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(33, 33, 33)},  // 1 . . 4
	{-0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(33, 33, 33)},  // 2 . . 3
	{ 0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(33, 33, 33)},
	{ 0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(33, 33, 33)}
};

HealthBarVertices_s HealthBarVertices[4] = {
	//  x     y      z     rhw   c
	{-0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(255, 0, 0)},  // 1 . . 4
	{-0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(255, 0, 0)},  // 2 . . 3
	{ 0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(0, 255, 0)},
	{ 0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(0, 255, 0)}
};

// backwards
//----------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if(DLL_PROCESS_ATTACH==fdwReason)
	{
		hInstance = hinstDLL;
		
		InitSettings();

		if(tSettings.bDebug || tSettings.bPlayOnline) {
		
			// Check the GTA version
			if(DetermineGTAVersion() != VICE_10) {
				MessageBox(0,"Incorrect gta-vc.exe version detected.\nYou must use GTA:VC 1.0 to play VC:MP","VC:MP Error",MB_OK);
				SetForegroundWindow(HWND_DESKTOP);
				ExitProcess(1);
			}

			dwGameLoop = (DWORD)TheGameLoop;
			dwRenderLoop = (DWORD)TheRenderLoop;

			pGame = new CGame();
			pGame->StartGame();

			SubclassGameWindow();

			// Time to hook directx...

			// Grab the real IDirect3D8 * from the game.
			pD3D = (IDirect3D8 *)pGame->GetD3D();

			// Grab the real IDirect3DDevice8 * from the game.
			pD3DDevice = (IDirect3DDevice8 *)pGame->GetD3DDevice();

			// Create instances of our hook classes and force GTA to
			// chew on them.
			//pD3DHook = new IDirect3D8Hook;
			pD3DDeviceHook = new IDirect3DDevice8Hook;

			//pGame->setD3D((DWORD)pD3DHook); <- not working, crash in CheckDeviceFormat.
			
			pGame->SetD3DDevice((DWORD)pD3DDeviceHook);
				
			// Create instances of the chat and input classes.
			pChatWindow = new CChatWindow(pD3DDevice);
			pCmdWindow = new CCmdWindow(pD3DDevice);

			pScoreBoard = new CScoreBoard();
			pNetStats = new CNetStats();

			SetupCommands();			
		}
		// else they must want to play single
		// player or they got the command line
		// arguments wrong.
	}

	return TRUE;
}

//----------------------------------------------------
// Main loop which is called before game processing.
// This is actually a result of a hook inside GTA's
// main loop (TheGameLoop())

void TheGameLoop()
{
	// If the game is not inited then do it now.
	if(!bGameInited && FileCheckSum())
	{
		if(tSettings.bPlayOnline)
		{			
			pNetGame = new CNetGame(tSettings.szConnectHost,atoi(tSettings.szConnectPort),
				tSettings.szNickName,tSettings.szConnectPass);
		}

		pGame->ToggleFrameLimiterState(TRUE);
		bGameInited = TRUE;

		return;
	}

	// The NetGame processing has been moved
	// to the Direct3DDevice::Present hook.
	// This is to solve alt-tab issues.

   // BEGIN: HEALTH BAR AND PLAYER NAME STUFF 
    if(!pGame->IsMenuActive() && bShowNameTags) {

      if(!bbfont) 
      { 
         bbfont = new CBBFont(pD3DDevice, "vcmpfnt"); 
         bbfont->Initialise(); 
      } 

      if (!HealthBarOldStateBlock) 
      { 
         pD3DDevice->BeginStateBlock(); 
         pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE); 
         pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); 
         pD3DDevice->SetRenderState(D3DRS_ZENABLE, 1); 
         pD3DDevice->SetRenderState(D3DRS_FILLMODE, 3); 
         pD3DDevice->SetRenderState(D3DRS_CULLMODE, 1); 
         pD3DDevice->SetRenderState(D3DRS_WRAP0, 0); 
         pD3DDevice->SetRenderState(D3DRS_CLIPPING, 1); 
         pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND, 0); 
         pD3DDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 1); 
         pD3DDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, 0); 
         pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 15); 
         pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1); 
         pD3DDevice->SetRenderState(D3DRS_SRCBLEND, 5); 
         pD3DDevice->SetRenderState(D3DRS_DESTBLEND, 6); 
         pD3DDevice->SetRenderState(D3DRS_BLENDOP, 1); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, 4); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, 2); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, 0); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, 4); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, 0); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, 0); 
         pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, 1); 
         pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, 1); 
         pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD); 
         pD3DDevice->SetVertexShader(D3DFVF_XYZ|D3DFVF_DIFFUSE); 
         pD3DDevice->SetStreamSource(0, NULL, 0); 
         pD3DDevice->EndStateBlock(&HealthBarOldStateBlock); 
      } 

      if (!HealthBarNewStateBlock) 
      { 
         pD3DDevice->BeginStateBlock(); 
         pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE); 
         pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); 
         pD3DDevice->SetRenderState(D3DRS_ZENABLE, 1); 
         pD3DDevice->SetRenderState(D3DRS_FILLMODE, 3); 
         pD3DDevice->SetRenderState(D3DRS_CULLMODE, 1); 
         pD3DDevice->SetRenderState(D3DRS_WRAP0, 0); 
         pD3DDevice->SetRenderState(D3DRS_CLIPPING, 1); 
         pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND, 0); 
         pD3DDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 1); 
         pD3DDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, 0); 
         pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 15); 
         pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1); 
         pD3DDevice->SetRenderState(D3DRS_SRCBLEND, 5); 
         pD3DDevice->SetRenderState(D3DRS_DESTBLEND, 6); 
         pD3DDevice->SetRenderState(D3DRS_BLENDOP, 1); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, 4); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, 2); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, 0); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, 1); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, 0); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0); 
         pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, 0); 
         pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, 1); 
         pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, 1); 
         pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD); 
         pD3DDevice->SetVertexShader(D3DFVF_XYZ|D3DFVF_DIFFUSE); 
         pD3DDevice->EndStateBlock(&HealthBarNewStateBlock); 
      } 

      pD3DDevice->CaptureStateBlock(HealthBarOldStateBlock); 
      pD3DDevice->ApplyStateBlock(HealthBarNewStateBlock); 

      D3DXMATRIX matTransposed; 
      D3DXMatrixTranspose(&matTransposed, (D3DXMATRIX*)&matView); 
      matTransposed._14 = matTransposed._24 = matTransposed._34 = 0.0f; 

      if(pNetGame) 
      { 
         CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool(); 
         for(int x=0; x < MAX_PLAYERS; x++) 
         { 
            if(pPlayerPool->GetSlotState(x) == TRUE) // player is in use 
            { 
               CRemotePlayer* Player = pPlayerPool->GetAt(x); 

               if( Player->IsActive() && 
                  (Player->GetDistanceFromLocalPlayer() <= 80.0f) ) // Active and within reasonable distance 
               { 
                  CPlayerPed* PlayerPed = Player->GetPlayerPed(); 
                   
                  if(PlayerPed->IsOnScreen()) 
                  { // They're onscreen 
                     MATRIX4X4 matPlayer; 
                     PlayerPed->GetMatrix(&matPlayer); 
                     matTransposed._41 = matPlayer.vPos.X; 
                     matTransposed._42 = matPlayer.vPos.Y; 
                     matTransposed._43 = matPlayer.vPos.Z + 1.0f; 
                     pD3DDevice->SetTransform(D3DTS_WORLD, &matTransposed); 

                     float Health = Player->GetReportedHealth(); 
                     if (Health > 100.0f) Health = 100.0f; 

                     HealthBarBGVertices[0].x = HealthBarBGVertices[1].x = (0.0058f * Health) - 0.29f; 
                     pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarBDRVertices, sizeof(HealthBarVertices_s)); 
                     pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarVertices,    sizeof(HealthBarVertices_s)); 
                     pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarBGVertices,  sizeof(HealthBarVertices_s)); 
                      
                     if(bbfont) { 
                        bbfont->Begin(); 
                        //bbfont->Draw(pPlayerPool->GetPlayerName(x), 0.285f, 0xFF000000);
						bbfont->Draw(pPlayerPool->GetPlayerName(x), 0.300f, Player->GetTeamColorAsARGB()); 
                        bbfont->End(); 
                     } 
                  } 
               } 
            } 
         } 
      } 

	  pD3DDevice->ApplyStateBlock(HealthBarOldStateBlock); 
	  // END:   HEALTH BAR AND PLAYER NAME STUFF 
	}

	if(!pGame->IsMenuActive())
	{
		if((pNetGame) && GetAsyncKeyState(VK_F5)) {
			pGame->DisplayHud(FALSE);
			pScoreBoard->Draw();
		}
		else if((pNetGame) && GetAsyncKeyState(VK_F6)) {
			pGame->DisplayHud(FALSE);
			pNetStats->Draw();
		} 
		else
		{
			pGame->DisplayHud(TRUE);
			if(pChatWindow) pChatWindow->Draw();
			if(pCmdWindow) pCmdWindow->Draw();
		}
	}
}

//----------------------------------------------------

void TheRenderLoop()
{
	
}

//----------------------------------------------------

void InitSettings()
{
	PCHAR szCmdLine = GetCommandLine();

	memset(&tSettings,0,sizeof(GAME_SETTINGS));

	while(*szCmdLine) {

		if(*szCmdLine == '-' || *szCmdLine == '/') {
			szCmdLine++;
			switch(*szCmdLine) {
				case 'd':
				case 'D':
					tSettings.bDebug = TRUE;
					tSettings.bPlayOnline = FALSE;
					break;
				case 'c':
				case 'C':
					tSettings.bPlayOnline = TRUE;
					tSettings.bDebug = FALSE;
					break;
				case 'w':
				case 'W':
					bWindowedMode = TRUE;
					break;
				case 'h':
				case 'H':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szConnectHost);
					break;
				case 'p':
				case 'P':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szConnectPort);
					break;
				case 'n':
				case 'N':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szNickName);
					break;
				case 'z':
				case 'Z':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szConnectPass);
					break;
			}
		}

		szCmdLine++;
	}
}

//----------------------------------------------------

void SetStringFromCommandLine(char *szCmdLine, char *szString)
{
	while(*szCmdLine == ' ') szCmdLine++;
	while(*szCmdLine &&
		  *szCmdLine != ' ' &&
		  *szCmdLine != '-' &&
		  *szCmdLine != '/') 
	{
		*szString = *szCmdLine;
		szString++; szCmdLine++;
	}
	*szString = '\0';
}

//----------------------------------------------------

int DetermineGTAVersion()
{
	BYTE* VerCheck = (BYTE*)0x608578;
	
	switch (*VerCheck)
	{
	case 0x81: 
		return VICE_11;
	case 0x5D: 
		return VICE_10;
	}
	
	return UNKNOWN_VERSION;
}

//----------------------------------------------------