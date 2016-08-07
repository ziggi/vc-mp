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

#pragma once

#pragma pack(1)
typedef struct _PLAYER_SPAWN_INFO
{
	BYTE byteTeam;
	BYTE byteSkin;
	VECTOR vecPos;
	float fRotation;
	int iSpawnWeapons[3];
	int iSpawnWeaponsAmmo[3];
} PLAYER_SPAWN_INFO;

#define IS_FIRING(x) (x & 0x200) // for checking the keystate firing bit
#define CEASE_FIRE_CEASE_FIRE(x) (x & 0xFDFF)

#define NO_TEAM 255

//----------------------------------------------------------

class CLocalPlayer
{
private:

	CPlayerPed				*m_pPlayerPed;
	PLAYER_SPAWN_INFO		m_SpawnInfo;
	BOOL					m_bHasSpawnInfo;
	BOOL					m_bIsActive;
	BOOL					m_bIsWasted;
	CHAR					m_szPlayerName[256];
	WORD					m_wLastKeys;
	DWORD					m_dwLastSendTick;
	DWORD					m_dwLastPassengerSendTick;

public:

	CLocalPlayer();
	~CLocalPlayer(){};

	BOOL IsActive() { return m_bIsActive; };
	BOOL IsWasted() { return m_bIsWasted; };
	BOOL HasSpawnInfo() { return m_bHasSpawnInfo; };

	BOOL Process();
	BOOL DestroyPlayer();

	void SendOnFootFullSyncData();
	void SendInCarFullSyncData();
	void SendInCarPassengerData();
	int  GetOptimumInCarSendRate();
	int  GetOptimumOnFootSendRate();

	BYTE GetTeam() { 
		if(m_bHasSpawnInfo) return m_SpawnInfo.byteTeam;
		else return NO_TEAM;
	};
	
	void SendWastedNotification();
	void HandleDeath(BYTE byteReason, BYTE byteWhoKilled, BYTE byteScoringModifier);
	
	void RequestClass(BYTE byteClass);

	void SetSpawnInfo(BYTE byteTeam, BYTE byteSkin, VECTOR * vecPos, float fRotation,
		int iSpawnWeapon1, int iSpawnWeapon1Ammo, int iSpawnWeapon2, int iSpawnWeapon2Ammo,
		int iSpawnWeapon3, int iSpawnWeapon3Ammo);

	BOOL SpawnPlayer( BYTE byteTeam,BYTE byteSkin,VECTOR * vecPos,float fRotation,
		int iSpawnWeapon1,int iSpawnWeapon1Ammo,int iSpawnWeapon2,int iSpawnWeapon2Ammo,
		int iSpawnWeapon3,int iSpawnWeapon3Ammo);

	BOOL SpawnPlayer();

	CPlayerPed * GetPlayerPed() { return m_pPlayerPed; };

	void Say(PCHAR szText);

	void SendExitVehicleNotification(BYTE byteVehicleID);
	void SendEnterVehicleNotification(BYTE byteVehicleID, BOOL bPassenger);
	
	DWORD GetTeamColorAsRGBA();
	DWORD GetTeamColorAsARGB();
};


//----------------------------------------------------------