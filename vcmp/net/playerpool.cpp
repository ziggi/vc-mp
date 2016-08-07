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
//----------------------------------------------------

#include "../main.h"

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;

char szQuitReasons[][32] = {
"Timeout",
"Leaving",
"Kicked"
};

//----------------------------------------------------

CPlayerPool::CPlayerPool()
{
	BYTE bytePlayerID = 0;

	m_pLocalPlayer = new CLocalPlayer();

	while(bytePlayerID < MAX_PLAYERS) {
		m_bPlayerSlotState[bytePlayerID] = FALSE;
		m_pPlayers[bytePlayerID] = NULL;
		bytePlayerID++;
	}
}

//----------------------------------------------------

CPlayerPool::~CPlayerPool()
{	
	BYTE bytePlayerID = 0;

	delete m_pLocalPlayer;

	while(bytePlayerID < MAX_PLAYERS) {
		Delete(bytePlayerID,0);
		bytePlayerID++;
	}
}

//----------------------------------------------------

BOOL CPlayerPool::New(BYTE bytePlayerID, PCHAR szPlayerName)
{
	m_pPlayers[bytePlayerID] = new CRemotePlayer();

	if(m_pPlayers[bytePlayerID])
	{
		strcpy(m_szPlayerNames[bytePlayerID],szPlayerName);
		m_pPlayers[bytePlayerID]->SetID(bytePlayerID);
		m_bPlayerSlotState[bytePlayerID] = TRUE;

		m_iScore[bytePlayerID] = 0;
		m_iPing[bytePlayerID] = 0;
		m_ulIPAddress[bytePlayerID] = 0;

		if(pChatWindow) 
			pChatWindow->AddInfoMessage("*** %s joined the server.",szPlayerName);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------

BOOL CPlayerPool::Delete(BYTE bytePlayerID, BYTE byteReason)
{
	if(!GetSlotState(bytePlayerID) || !m_pPlayers[bytePlayerID]) {
		return FALSE;
	}

	m_bPlayerSlotState[bytePlayerID] = FALSE;
	delete m_pPlayers[bytePlayerID];
	m_pPlayers[bytePlayerID] = NULL;

	if(pChatWindow) {
		pChatWindow->AddInfoMessage("*** %s left the server. (%s)",
		m_szPlayerNames[bytePlayerID],szQuitReasons[byteReason]);
	}

	return TRUE;
}

//----------------------------------------------------

BOOL CPlayerPool::Process()
{
	BYTE bytePlayerID = 0;

	m_pLocalPlayer->Process();

	while(bytePlayerID < MAX_PLAYERS) {
		if(TRUE == m_bPlayerSlotState[bytePlayerID]) {
			m_pPlayers[bytePlayerID]->Process();
		}
		bytePlayerID++;
	}

	return TRUE;
}

//----------------------------------------------------

BYTE CPlayerPool::FindRemotePlayerIDFromGtaPtr(PED_TYPE * pActor)
{
	CPlayerPed *pPlayerPed;

	BYTE bytePlayerID = 0;

	while(bytePlayerID < MAX_PLAYERS)  {

		if(TRUE == m_bPlayerSlotState[bytePlayerID]) {
			pPlayerPed = m_pPlayers[bytePlayerID]->GetPlayerPed();

			if(pPlayerPed) {
				PED_TYPE *pTestActor = pPlayerPed->GetGtaActor();
				if((pTestActor != NULL) && (pActor == pTestActor)) // found it
					return (BYTE)m_pPlayers[bytePlayerID]->GetID();
			}
		}

		bytePlayerID++;
	}

	return INVALID_PLAYER_ID;	
}
	
//----------------------------------------------------