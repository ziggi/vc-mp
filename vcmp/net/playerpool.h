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

#pragma once

#define INVALID_PLAYER_ID 255

//----------------------------------------------------

class CPlayerPool
{
private:
	
	BOOL m_bPlayerSlotState[MAX_PLAYERS];
	CLocalPlayer  *m_pLocalPlayer;
	BYTE	m_byteLocalPlayerID;
	CRemotePlayer *m_pPlayers[MAX_PLAYERS];
	CHAR	m_szLocalPlayerName[MAX_PLAYER_NAME+1];
	CHAR	m_szPlayerNames[MAX_PLAYERS][MAX_PLAYER_NAME+1];
	ULONG	m_ulIPAddress[MAX_PLAYERS]; // used by admin
	
	int		m_iLocalPlayerScore;
	int		m_iLocalPlayerPing;
	int		m_iScore[MAX_PLAYERS];
	int		m_iPing[MAX_PLAYERS];

public:
	BOOL Process();

	void SetLocalPlayerName(PCHAR szName) { strcpy(m_szLocalPlayerName,szName); };
	PCHAR GetLocalPlayerName() { return m_szLocalPlayerName; };
	PCHAR GetPlayerName(BYTE bytePlayerID) { return m_szPlayerNames[bytePlayerID]; };

	CLocalPlayer * GetLocalPlayer() { return m_pLocalPlayer; };
	BYTE FindRemotePlayerIDFromGtaPtr(PED_TYPE * pActor);

	BOOL New(BYTE bytePlayerID, PCHAR szPlayerName);
	BOOL Delete(BYTE bytePlayerID, BYTE byteReason);

	CRemotePlayer* GetAt(BYTE bytePlayerID) {
		if(bytePlayerID > MAX_PLAYERS) { return NULL; }
		return m_pPlayers[bytePlayerID];
	};

	BOOL GetSlotState(BYTE bytePlayerID) {
		if(bytePlayerID > MAX_PLAYERS) { return FALSE; }
		return m_bPlayerSlotState[bytePlayerID];
	};
	
	void SetLocalPlayerID(BYTE byteID) {
		strcpy(m_szPlayerNames[byteID],m_szLocalPlayerName);
		m_byteLocalPlayerID = byteID;
	};

	void UpdatePing(BYTE bytePlayerID,int iPing) { 
		if(bytePlayerID == m_byteLocalPlayerID) {
			m_iLocalPlayerPing = iPing;
		} else {
			m_iPing[bytePlayerID] = iPing;
		}
	};

	void UpdateScore(BYTE bytePlayerID,int iScore) { 
		if(bytePlayerID == m_byteLocalPlayerID) {
			m_iLocalPlayerScore = iScore;
		} else {
			m_iScore[bytePlayerID] = iScore;
		}
	};

	void UpdateIPAddress(BYTE bytePlayerID, ULONG ip) {
		m_ulIPAddress[bytePlayerID] = ip;
	}

	int GetPing(BYTE bytePlayerID) { return m_iPing[bytePlayerID]; };
	int GetScore(BYTE bytePlayerID) { return m_iScore[bytePlayerID]; };
	int GetLocalPlayerPing() { return m_iLocalPlayerPing; };
	int GetLocalPlayerScore() { return m_iLocalPlayerScore; };
	BYTE GetLocalPlayerID() { return m_byteLocalPlayerID; };
	ULONG GetIPAddress(BYTE bytePlayerID) { return m_ulIPAddress[bytePlayerID]; };

	CPlayerPool();
	~CPlayerPool();
};

//----------------------------------------------------