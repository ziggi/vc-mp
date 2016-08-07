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

extern CNetGame *pNetGame;

//----------------------------------------------------

void DecompressVector1(VECTOR_PAD * vec, C_VECTOR1 * c1)
{
	vec->X = (float)c1->X;
	vec->X = (float)((double)vec->X / 10000.0);
	vec->Y = (float)c1->Y;
	vec->Y = (float)((double)vec->Y / 10000.0);
	vec->Z = (float)c1->Z;
	vec->Z = (float)((double)vec->Z / 10000.0);	
}

//----------------------------------------------------

CPlayer::CPlayer()
{
	m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;
	m_bytePlayerID = INVALID_PLAYER_ID;
	m_bIsActive = FALSE;
	m_bIsWasted = FALSE;
	m_byteVehicleID = 0;
}

//----------------------------------------------------

void CPlayer::Process()
{
	if(m_bIsActive)
	{
		if(m_byteUpdateFromNetwork != UPDATE_TYPE_NONE) 
		{
			if(ValidateSyncData()) {
				BroadcastSyncData();
			}
			m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;
		}
	}
}

//----------------------------------------------------
// thx CKY.BAM <g>

BOOL CPlayer::ValidateSyncData()
{
	if(m_vecPos.X > 2500.0f || m_vecPos.X < -2500.0f) {
		return FALSE;
	}
	if(m_vecPos.Y > 2500.0f || m_vecPos.Y < -2500.0f) {
		return FALSE;
	}
	if(m_vecPos.Z > 300.0f || m_vecPos.Z  < 0.0f) {
		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------

void CPlayer::BroadcastSyncData()
{
	RakNet::BitStream bsSync;
		
	if(m_byteUpdateFromNetwork == UPDATE_TYPE_FULL_ONFOOT)
	{
		bsSync.Write((BYTE)ID_PLAYER_SYNC);
		bsSync.Write(m_bytePlayerID);
		bsSync.Write((WORD)m_wKeys);
		bsSync.Write(m_vecPos.X);
		bsSync.Write(m_vecPos.Y);
		bsSync.Write(m_vecPos.Z);
		bsSync.Write(m_fRotation);
		bsSync.Write(m_byteAction);
		bsSync.Write(m_byteHealth);
		bsSync.Write(m_byteCurrentWeapon);

		if(IS_FIRING(m_wKeys)) {
			// aiming
			bsSync.Write(m_Aiming.f1x);
			bsSync.Write(m_Aiming.f1y);
			bsSync.Write(m_Aiming.f1z);
			bsSync.Write(m_Aiming.f2x);
			bsSync.Write(m_Aiming.f2y);
			bsSync.Write(m_Aiming.f2z);
			bsSync.Write(m_Aiming.pos1x);
			bsSync.Write(m_Aiming.pos1y);
			bsSync.Write(m_Aiming.pos1z);
		}
		
		pNetGame->BroadcastData(&bsSync,HIGH_PRIORITY,UNRELIABLE,0,m_bytePlayerID);
	}
	else if(m_byteUpdateFromNetwork == UPDATE_TYPE_FULL_INCAR)
	{			
		BYTE	byteWriteVehicleHealth;

		// packing
		byteWriteVehicleHealth = PACK_VEHICLE_HEALTH(m_fVehicleHealth);

		// storing
		bsSync.Write((BYTE)ID_VEHICLE_SYNC);
		bsSync.Write(m_bytePlayerID);
		bsSync.Write(m_byteVehicleID);
		bsSync.Write((WORD)m_wKeys);
		bsSync.Write(m_cvecRoll.X);
		bsSync.Write(m_cvecRoll.Y);
		bsSync.Write(m_cvecRoll.Z);
		bsSync.Write(m_cvecDirection.X);
		bsSync.Write(m_cvecDirection.Y);
		bsSync.Write(m_cvecDirection.Z);
		bsSync.Write(m_vecPos.X);
		bsSync.Write(m_vecPos.Y);
		bsSync.Write(m_vecPos.Z);

		// move + turn speed
		bsSync.Write(m_vecMoveSpeed.X);
		bsSync.Write(m_vecMoveSpeed.Y);

		bsSync.Write(byteWriteVehicleHealth);
		bsSync.Write(m_byteHealth);

		// sending...
		pNetGame->BroadcastData(&bsSync,HIGH_PRIORITY,UNRELIABLE,0,m_bytePlayerID);
	}	
}

//----------------------------------------------------

void CPlayer::StoreOnFootFullSyncData(WORD wKeys,VECTOR * vecPos, 
									  float fRotation,BYTE byteCurrentWeapon,
									  BYTE byteAction)
{
	m_wKeys = wKeys;
	memcpy(&m_vecPos,vecPos,sizeof(VECTOR));
	m_fRotation = fRotation;
	m_byteCurrentWeapon = byteCurrentWeapon;
	m_byteAction = byteAction;
	m_byteUpdateFromNetwork = UPDATE_TYPE_FULL_ONFOOT;
}

//----------------------------------------------------

void CPlayer::StoreInCarFullSyncData(BYTE byteVehicleID, WORD wKeys,
									 C_VECTOR1 * cvecRoll, C_VECTOR1 * cvecDirection, 
									 VECTOR * vecPos, VECTOR * vecMoveSpeed,float fVehicleHealth)
{
	m_byteVehicleID = byteVehicleID;
	m_bIsInVehicle = TRUE;
	m_bIsAPassenger = FALSE;
	m_wKeys = wKeys;

	memcpy(&m_cvecRoll,cvecRoll,sizeof(C_VECTOR1));
	memcpy(&m_cvecDirection,cvecDirection,sizeof(C_VECTOR1));
	memcpy(&m_vecPos,vecPos,sizeof(VECTOR));

	m_fVehicleHealth = fVehicleHealth;
	m_byteUpdateFromNetwork = UPDATE_TYPE_FULL_INCAR;

	MATRIX4X4 matWorld;

	DecompressVector1(&matWorld.vLookRight,cvecRoll);
	DecompressVector1(&matWorld.vLookUp,cvecDirection);
	memcpy(&matWorld.vPos,vecPos,sizeof(VECTOR));
	memcpy(&m_vecMoveSpeed,vecMoveSpeed,sizeof(VECTOR));

	CVehicle *pVehicle = pNetGame->GetVehiclePool()->GetAt(byteVehicleID);
	pVehicle->Update(m_bytePlayerID,&matWorld,vecMoveSpeed,fVehicleHealth);
}

//----------------------------------------------------

void CPlayer::Say(PCHAR szText, BYTE byteTextLength)
{

}

//----------------------------------------------------

void CPlayer::HandleDeath(BYTE byteReason, BYTE byteWhoWasResponsible)
{
	RakNet::BitStream bsPlayerDeath;
	PlayerID playerid = pNetGame->GetRakServer()->GetPlayerIDFromIndex(m_bytePlayerID);

	m_bIsActive = FALSE;
	m_bIsWasted = TRUE;

	BYTE byteScoringModifier;

	byteScoringModifier = 
		pNetGame->GetPlayerPool()->AddResponsibleDeath(byteWhoWasResponsible,m_bytePlayerID);

	bsPlayerDeath.Write(m_bytePlayerID);
	bsPlayerDeath.Write(byteReason);
	bsPlayerDeath.Write(byteWhoWasResponsible);
	bsPlayerDeath.Write(byteScoringModifier);
	
	// Broadcast it
	pNetGame->GetRakServer()->RPC("Death",&bsPlayerDeath,
		HIGH_PRIORITY,RELIABLE,0,playerid,TRUE,FALSE);

	// Now let the player who died know aswell.
	pNetGame->GetRakServer()->RPC("OwnDeath",&bsPlayerDeath,
		HIGH_PRIORITY,RELIABLE,0,playerid,FALSE,FALSE);
	
	logprintf("<%s> died",
		pNetGame->GetPlayerPool()->GetPlayerName(m_bytePlayerID),
		byteReason,byteWhoWasResponsible,byteScoringModifier);
}

//----------------------------------------------------

void CPlayer::SetSpawnInfo(BYTE byteTeam, BYTE byteSkin, VECTOR * vecPos, float fRotation,
		int iSpawnWeapon1, int iSpawnWeapon1Ammo, int iSpawnWeapon2, int iSpawnWeapon2Ammo,
		int iSpawnWeapon3, int iSpawnWeapon3Ammo)
{
	m_SpawnInfo.byteTeam = byteTeam;
	m_SpawnInfo.byteSkin = byteSkin;
	m_SpawnInfo.vecPos.X = vecPos->X;
	m_SpawnInfo.vecPos.Y = vecPos->Y;
	m_SpawnInfo.vecPos.Z = vecPos->Z;
	m_SpawnInfo.fRotation = fRotation;
	m_SpawnInfo.iSpawnWeapons[0] = iSpawnWeapon1;
	m_SpawnInfo.iSpawnWeapons[1] = iSpawnWeapon2;
	m_SpawnInfo.iSpawnWeapons[2] = iSpawnWeapon3;
	m_SpawnInfo.iSpawnWeaponsAmmo[0] = iSpawnWeapon1Ammo;
	m_SpawnInfo.iSpawnWeaponsAmmo[1] = iSpawnWeapon2Ammo;
	m_SpawnInfo.iSpawnWeaponsAmmo[2] = iSpawnWeapon3Ammo;
	m_bHasSpawnInfo = TRUE;
}

//----------------------------------------------------

void CPlayer::Spawn()
{
	if(m_bHasSpawnInfo) {

		// Reset all their sync attributes.
		m_vecPos.X = m_SpawnInfo.vecPos.X;
		m_vecPos.Y = m_SpawnInfo.vecPos.Y;
		m_vecPos.Z = m_SpawnInfo.vecPos.Z;

		m_fRotation = m_SpawnInfo.fRotation;
		memset(&m_vecMoveSpeed,0,sizeof(VECTOR));
		memset(&m_vecTurnSpeed,0,sizeof(VECTOR));
		m_fRotation = 0.0f;
		m_bIsInVehicle=FALSE;
		m_bIsAPassenger=FALSE;
		m_byteVehicleID=0;
				
		SpawnForWorld(m_SpawnInfo.byteTeam,m_SpawnInfo.byteSkin,&m_SpawnInfo.vecPos,m_SpawnInfo.fRotation);
	}
}

//----------------------------------------------------
// This is the method used for respawning.

void CPlayer::SpawnForWorld( BYTE byteTeam, BYTE byteSkin, VECTOR * vecPos, 
							  float fRotation )
{
	RakNet::BitStream bsPlayerSpawn;
	PlayerID playerid = pNetGame->GetRakServer()->GetPlayerIDFromIndex(m_bytePlayerID);

	bsPlayerSpawn.Write(m_bytePlayerID);
	bsPlayerSpawn.Write(byteTeam);
	bsPlayerSpawn.Write(byteSkin);
	bsPlayerSpawn.Write(vecPos->X);
	bsPlayerSpawn.Write(vecPos->Y);
	bsPlayerSpawn.Write(vecPos->Z);
	bsPlayerSpawn.Write(fRotation);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeapons[0]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeaponsAmmo[0]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeapons[1]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeaponsAmmo[1]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeapons[2]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeaponsAmmo[2]);
	
	pNetGame->GetRakServer()->RPC("Spawn",&bsPlayerSpawn,
		HIGH_PRIORITY,RELIABLE_ORDERED,0,playerid,TRUE,FALSE);

	m_bIsActive = TRUE;
	m_bIsWasted = FALSE;

	// Set their initial sync position to their spawn position.
	m_vecPos.X = vecPos->X;
	m_vecPos.Y = vecPos->Y;
	m_vecPos.Z = vecPos->Z;

	m_byteAction = 1;
}

//----------------------------------------------------
// This is the method used for spawning players that
// already exist in the world when the client connects.

void CPlayer::SpawnForPlayer(BYTE byteForPlayerID)
{
	RakNet::BitStream bsPlayerSpawn;

	bsPlayerSpawn.Write(m_bytePlayerID);
	bsPlayerSpawn.Write(m_SpawnInfo.byteTeam);
	bsPlayerSpawn.Write(m_SpawnInfo.byteSkin);
	bsPlayerSpawn.Write(m_vecPos.X);
	bsPlayerSpawn.Write(m_vecPos.Y);
	bsPlayerSpawn.Write(m_vecPos.Z);
	bsPlayerSpawn.Write(m_fRotation);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeapons[0]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeaponsAmmo[0]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeapons[1]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeaponsAmmo[1]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeapons[2]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeaponsAmmo[2]);

	pNetGame->GetRakServer()->RPC("Spawn",&bsPlayerSpawn,HIGH_PRIORITY,RELIABLE_ORDERED,
		0,pNetGame->GetRakServer()->GetPlayerIDFromIndex(byteForPlayerID),FALSE,FALSE);
}

//----------------------------------------------------

void CPlayer::EnterVehicle(BYTE byteVehicleID, BYTE bytePassenger)
{
	RakNet::BitStream bsVehicle;
	PlayerID playerid = pNetGame->GetRakServer()->GetPlayerIDFromIndex(m_bytePlayerID);

	bsVehicle.Write(m_bytePlayerID);
	bsVehicle.Write(byteVehicleID);
	bsVehicle.Write(bytePassenger);

	pNetGame->GetRakServer()->RPC("EnterVehicle",&bsVehicle,HIGH_PRIORITY,RELIABLE_ORDERED,
		0,playerid,TRUE,FALSE);
}

//----------------------------------------------------

void CPlayer::ExitVehicle(BYTE byteVehicleID)
{
	RakNet::BitStream bsVehicle;
	PlayerID playerid = pNetGame->GetRakServer()->GetPlayerIDFromIndex(m_bytePlayerID);

	bsVehicle.Write(m_bytePlayerID);
	bsVehicle.Write(byteVehicleID);

	pNetGame->GetRakServer()->RPC("ExitVehicle",&bsVehicle,HIGH_PRIORITY,RELIABLE_ORDERED,
		0,playerid,TRUE,FALSE);
}

//----------------------------------------------------