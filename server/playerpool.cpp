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

#include "netgame.h"
#include "rcon.h"

extern CNetGame *pNetGame;
extern CRcon	*pRcon;

#ifndef WIN32
#	define stricmp strcasecmp
#endif

//----------------------------------------------------

CPlayerPool::CPlayerPool()
{
	BYTE bytePlayerID = 0;
	while(bytePlayerID != MAX_PLAYERS) {
		m_bPlayerSlotState[bytePlayerID] = FALSE;
		m_pPlayers[bytePlayerID] = NULL;
		bytePlayerID++;
	}
}

//----------------------------------------------------

CPlayerPool::~CPlayerPool()
{	
	BYTE bytePlayerID = 0;
	while(bytePlayerID != MAX_PLAYERS) {
		Delete(bytePlayerID,0);
		bytePlayerID++;
	}
}

//----------------------------------------------------

BOOL CPlayerPool::New(BYTE bytePlayerID, PCHAR szPlayerName)
{
	m_pPlayers[bytePlayerID] = new CPlayer();

	if(m_pPlayers[bytePlayerID])
	{
		strcpy(m_szPlayerName[bytePlayerID],szPlayerName);
		m_pPlayers[bytePlayerID]->SetID(bytePlayerID);
		m_bPlayerSlotState[bytePlayerID] = TRUE;
		m_iPlayerScore[bytePlayerID] = 0;
		m_bIsAnAdmin[bytePlayerID] = FALSE;

		// Notify all the other players of a newcommer with
		// a 'ServerJoin' join RPC 
		RakNet::BitStream bsSend;
		bsSend.Write(bytePlayerID);
		bsSend.Write(strlen(szPlayerName));
		bsSend.Write(szPlayerName,strlen(szPlayerName));
		pNetGame->GetRakServer()->RPC("ServerJoin",&bsSend,HIGH_PRIORITY,RELIABLE_ORDERED,0,
			pNetGame->GetRakServer()->GetPlayerIDFromIndex(bytePlayerID),TRUE,FALSE);
		
		pRcon->ConsolePrintf("[join] %u %s",bytePlayerID,szPlayerName);

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
	if(!GetSlotState(bytePlayerID) || !m_pPlayers[bytePlayerID])
	{
		return FALSE;
	}

	m_bPlayerSlotState[bytePlayerID] = FALSE;
	delete m_pPlayers[bytePlayerID];
	m_pPlayers[bytePlayerID] = NULL;
	
	// Notify all the other players that this client is quiting.
	RakNet::BitStream bsSend;
	bsSend.Write(bytePlayerID);
	bsSend.Write(byteReason);
	pNetGame->GetRakServer()->RPC("ServerQuit",&bsSend,HIGH_PRIORITY,RELIABLE_ORDERED,0,
		pNetGame->GetRakServer()->GetPlayerIDFromIndex(bytePlayerID),TRUE,FALSE);

	pRcon->ConsolePrintf("[part] %u %s %u",bytePlayerID,m_szPlayerName[bytePlayerID],byteReason);
	
	return TRUE;
}

//----------------------------------------------------

void CPlayerPool::Process()
{
	BYTE bytePlayerID = 0;

	while(bytePlayerID != MAX_PLAYERS) {
		if(TRUE == m_bPlayerSlotState[bytePlayerID]) {
			m_pPlayers[bytePlayerID]->Process();
		}
		bytePlayerID++;
	}
}

//----------------------------------------------------
// Add information to the pool about a kill.
// Return constant describing the scoring.

BYTE CPlayerPool::AddResponsibleDeath(BYTE byteWhoKilled, BYTE byteWhoDied)
{
	CPlayer *pWhoKilled;
	CPlayer *pWhoDied;

	if( byteWhoKilled != INVALID_PLAYER_ID &&
		byteWhoKilled < MAX_PLAYERS &&
		byteWhoDied < MAX_PLAYERS ) {

		if(m_bPlayerSlotState[byteWhoKilled] && m_bPlayerSlotState[byteWhoDied]) {

			pWhoKilled = m_pPlayers[byteWhoKilled];
			pWhoDied = m_pPlayers[byteWhoDied];

			if( (pWhoKilled->GetTeam() == 255) ||
				(pWhoKilled->GetTeam() != pWhoDied->GetTeam()) )
			{
				m_iPlayerScore[byteWhoKilled]++;

				pRcon->OutputDeathMessage(VALID_KILL,byteWhoDied,byteWhoKilled);

				return VALID_KILL;
			}
			else {
				m_iPlayerScore[byteWhoKilled]--;

				pRcon->OutputDeathMessage(TEAM_KILL,byteWhoDied,byteWhoKilled);

				return TEAM_KILL;
			}
		}
		return SELF_KILL;
	}

	if(byteWhoKilled == INVALID_PLAYER_ID && byteWhoDied < MAX_PLAYERS)
	{
		if(m_bPlayerSlotState[byteWhoDied]) {

			pRcon->OutputDeathMessage(SELF_KILL,byteWhoDied,0);

			return SELF_KILL;
		}
	}

	pRcon->OutputDeathMessage(SELF_KILL,byteWhoDied,0);
	
	return SELF_KILL;						
}

//----------------------------------------------------

float CPlayerPool::GetDistanceFromPlayerToPlayer(BYTE bytePlayer1, BYTE bytePlayer2)
{
	VECTOR	*vecFromPlayer;
	VECTOR	*vecThisPlayer;
	float	fSX,fSY;

	CPlayer * pPlayer1 = GetAt(bytePlayer1);
	CPlayer * pPlayer2 = GetAt(bytePlayer2);

	vecFromPlayer = &pPlayer1->m_vecPos;
	vecThisPlayer = &pPlayer2->m_vecPos;
	
	fSX = (vecThisPlayer->X - vecFromPlayer->X) * (vecThisPlayer->X - vecFromPlayer->X);
	fSY = (vecThisPlayer->Y - vecFromPlayer->Y) * (vecThisPlayer->Y - vecFromPlayer->Y);
	
	return (float)sqrt(fSX + fSY);	
}

//----------------------------------------------------

BOOL CPlayerPool::IsNickInUse(PCHAR szNick)
{
	int x=0;
	while(x!=MAX_PLAYERS) {
		if(GetSlotState((BYTE)x)) {
			if(!stricmp(GetPlayerName((BYTE)x),szNick)) {
				return TRUE;
			}
		}
		x++;
	}
	return FALSE;
}

//----------------------------------------------------
