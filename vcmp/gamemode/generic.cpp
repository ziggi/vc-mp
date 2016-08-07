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
//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author: kyeman
//
//----------------------------------------------------------

#include "../main.h"
#include "../game/keystuff.h"

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;

using namespace RakNet;
extern CNetGame* pNetGame;

//----------------------------------------------------------

CGameModeGeneric::CGameModeGeneric()
{
	m_byteSelectedClass = 0;
	m_dwLastSpawnSelectionTick = GetTickCount();
	HandleClassSelection(pNetGame->GetPlayerPool()->GetLocalPlayer());
}

//----------------------------------------------------------

void CGameModeGeneric::ProcessLocalPlayer(CLocalPlayer *pLocalPlayer)
{
	CCamera *pGameCamera;
	CPlayerPed *pGamePlayer;
	DWORD dwTicksSinceLastSelection;
	MATRIX4X4 matPlayer;
	float fDrawX=25.0f;
	float fDrawY=250.0f;
	char szMsg[256];

	if(FALSE == pLocalPlayer->IsActive() && !pLocalPlayer->IsWasted() && m_bClearedToSpawn)
	{
		// DONT ALLOW ANY ACTIONS IF WE'RE STILL FADING OR WAITING.
		if((GetTickCount() - m_dwInitialSelectionTick) < 2000) return;
		pGame->ToggleKeyInputsDisabled(TRUE);
		
		if(ScriptCommand(&is_key_pressed,0,17)) // FIRE PRESSED SO SPAWN
		{		
			pGameCamera = pGame->GetCamera();
			pGamePlayer = pGame->FindPlayerPed();
			pGameCamera->Restore();
			pLocalPlayer->SpawnPlayer();
			pGame->DisplayHud(TRUE);
			pGameCamera->SetBehindPlayer();
			pGamePlayer->GetMatrix(&matPlayer);			
			ScriptCommand(&play_sound,
				matPlayer.vPos.X,
				matPlayer.vPos.Y,
				matPlayer.vPos.Z,
				10);

			pGame->ToggleKeyInputsDisabled(FALSE);
			return;
		}
		else if(m_bClearedToSpawn) // WE ARE CLEARED TO SPAWN OR SELECT ANOTHER CLASS
		{
			// SHOW INFO ABOUT THE SELECTED CLASS..
			szMsg[0] = '\0';
			strcat(szMsg,"> Use Left and Right arrow keys to select a class.\n");
			strcat(szMsg,"> Press Fire button when ready to spawn.\n");

			CD3DFont *pD3DFont = pChatWindow->m_pD3DFont;
			pD3DFont->DrawText(fDrawX,fDrawY,0xFFFFFFFF,szMsg);

			// GRAB PLAYER MATRIX FOR SOUND POSITION
			pGamePlayer = pGame->FindPlayerPed();
			pGamePlayer->GetMatrix(&matPlayer);

			dwTicksSinceLastSelection = GetTickCount() - m_dwLastSpawnSelectionTick; // used to delay reselection.

			// ALLOW ANOTHER SELECTION WITH LEFT AND RIGHT KEYS
			if( (GetKeyState(VK_LEFT)&0x8000) && (dwTicksSinceLastSelection > 250)) { // LEFT ARROW
				
				m_bClearedToSpawn = FALSE;
				m_dwLastSpawnSelectionTick = GetTickCount();
				
				if(m_byteSelectedClass == 0) {
					m_byteSelectedClass = (pNetGame->m_iSpawnsAvailable - 1);
				}
				else {
					m_byteSelectedClass--;
				}
		
				ScriptCommand(&play_sound,
					matPlayer.vPos.X,
					matPlayer.vPos.Y,
					matPlayer.vPos.Z,
					14);

				pLocalPlayer->RequestClass(m_byteSelectedClass);
				return;
			}

			// ALLOW ANOTHER SELECTION WITH LEFT AND RIGHT KEYS
			if( (GetKeyState(VK_RIGHT)&0x8000) && (dwTicksSinceLastSelection > 250)) { // RIGHT ARROW
	
				m_bClearedToSpawn = FALSE;
				m_dwLastSpawnSelectionTick = GetTickCount();
				
				if(m_byteSelectedClass == (pNetGame->m_iSpawnsAvailable - 1)) {
					m_byteSelectedClass = 0;
				}
				else {
					m_byteSelectedClass++;
				}

				ScriptCommand(&play_sound,
					matPlayer.vPos.X,
					matPlayer.vPos.Y,
					matPlayer.vPos.Z,
					13);

				pLocalPlayer->RequestClass(m_byteSelectedClass);
				return;
			}			
		}
	}
}

//----------------------------------------------------------

void CGameModeGeneric::HandleClassSelection(CLocalPlayer *pLocalPlayer)
{
	CCamera *pGameCamera = pGame->GetCamera();
	CPlayerPed *pGamePlayer = pGame->FindPlayerPed();
	m_bClearedToSpawn = FALSE;

	pGame->DisplayHud(FALSE);

	if(pGameCamera)	{
		pGameCamera->SetPosition(pNetGame->m_vecInitCameraPos.X,
			pNetGame->m_vecInitCameraPos.Y,pNetGame->m_vecInitCameraPos.Z,0.0f,0.0f,0.0f);
		pGameCamera->LookAtPoint(pNetGame->m_vecInitCameraLook.X,
			pNetGame->m_vecInitCameraLook.Y,pNetGame->m_vecInitCameraLook.Z,1);
	}

	pLocalPlayer->RequestClass(m_byteSelectedClass);
	m_dwInitialSelectionTick = GetTickCount();
	m_dwLastSpawnSelectionTick = GetTickCount();
}

//----------------------------------------------------------

void CGameModeGeneric::HandleClassSelectionOutcome(PLAYER_SPAWN_INFO *pSpawnInfo, BOOL bOutcome)
{
	CPlayerPed *pGamePlayer = pGame->FindPlayerPed();

	if(bOutcome) {
		
		if(pGamePlayer)	{
			pGamePlayer->ClearAllWeapons();
			pGamePlayer->Teleport(pNetGame->m_vecInitPlayerPos.X,
				pNetGame->m_vecInitPlayerPos.Y,
				pNetGame->m_vecInitPlayerPos.Z);
			pGamePlayer->SetRotation(0.0f);
			pGamePlayer->SetModel(pSpawnInfo->byteSkin);
			if(pSpawnInfo->iSpawnWeapons[0] != (-1)) {
				pGamePlayer->GiveWeapon(pSpawnInfo->iSpawnWeapons[0],pSpawnInfo->iSpawnWeaponsAmmo[0]);
			}
		}

		m_bClearedToSpawn = TRUE;
	}
}

//----------------------------------------------------------