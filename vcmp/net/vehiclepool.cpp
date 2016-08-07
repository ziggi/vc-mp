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
#include "../game/util.h"

extern CGame *pGame;
extern CNetGame *pNetGame;
extern CChatWindow *pChatWindow;

//----------------------------------------------------

CVehiclePool::CVehiclePool()
{
	BYTE byteVehicleID = 0;

	while(byteVehicleID != MAX_VEHICLES) {
		m_bVehicleSlotState[byteVehicleID] = FALSE;
		m_pVehicles[byteVehicleID] = NULL;
		m_pGTAVehicles[byteVehicleID] = NULL;
		byteVehicleID++;
	}
}

//----------------------------------------------------

CVehiclePool::~CVehiclePool()
{	
	BYTE byteVehicleID = 0;

	while(byteVehicleID != MAX_VEHICLES) {
		Delete(byteVehicleID);
		byteVehicleID++;
	}
}

//----------------------------------------------------

BOOL CVehiclePool::New( BYTE byteVehicleID, BYTE byteVehicleType,
					    VECTOR * vecPos, float fRotation,
					    int iColor1, int iColor2,
					    VECTOR * vecSpawnPos, float fSpawnRotation )
{

	// Setup the spawninfo for the next respawn.
	m_SpawnInfo[byteVehicleID].byteVehicleType = byteVehicleType;
	m_SpawnInfo[byteVehicleID].vecPos.X = vecSpawnPos->X;
	m_SpawnInfo[byteVehicleID].vecPos.Y = vecSpawnPos->Y;
	m_SpawnInfo[byteVehicleID].vecPos.Z = vecSpawnPos->Z;
	m_SpawnInfo[byteVehicleID].fRotation = fSpawnRotation;
	m_SpawnInfo[byteVehicleID].iColor1 = iColor1;
	m_SpawnInfo[byteVehicleID].iColor2 = iColor2;

	// Now go ahead and spawn it at the location we got passed.
	return Spawn(byteVehicleID,byteVehicleType,vecPos,fRotation,iColor1,iColor2);
}

//----------------------------------------------------

BOOL CVehiclePool::Delete(BYTE byteVehicleID)
{
	if(!GetSlotState(byteVehicleID) || !m_pVehicles[byteVehicleID]) {
		return FALSE;
	}

	m_bVehicleSlotState[byteVehicleID] = FALSE;
	delete m_pVehicles[byteVehicleID];
	m_pVehicles[byteVehicleID] = NULL;

	return TRUE;
}

//----------------------------------------------------

BOOL CVehiclePool::Spawn( BYTE byteVehicleID, BYTE byteVehicleType,
					      VECTOR * vecPos, float fRotation,
					      int iColor1, int iColor2 )
{	
	if(m_pVehicles[byteVehicleID] != NULL) {
		Delete(byteVehicleID);
	}

	m_pVehicles[byteVehicleID] = pGame->NewVehicle(byteVehicleType,
		vecPos->X,vecPos->Y,vecPos->Z,fRotation);

	if(m_pVehicles[byteVehicleID])
	{	
		if(iColor1 != (-1)) {
			m_pVehicles[byteVehicleID]->SetColor(iColor1,iColor2);
		}

		m_pGTAVehicles[byteVehicleID] = m_pVehicles[byteVehicleID]->m_pVehicle;
		m_bVehicleSlotState[byteVehicleID] = TRUE;

		m_bIsActive[byteVehicleID] = TRUE;
		m_bIsWasted[byteVehicleID] = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------

int CVehiclePool::FindIDFromGtaPtr(VEHICLE_TYPE * pGtaVehicle)
{
	int x=1;
	
	while(x!=MAX_VEHICLES) {
		if(pGtaVehicle == m_pGTAVehicles[x]) return x;
		x++;
	}

	return (-1);
}

//----------------------------------------------------

int CVehiclePool::FindGtaIDFromID(int iID)
{
	return GamePool_Vehicle_GetIndex(m_pGTAVehicles[iID]);
}

//----------------------------------------------------

void CVehiclePool::Process()
{
	BYTE x=0;
	CVehicle *pVehicle;
	DWORD dwThisTime = GetTickCount();

	while(x != MAX_VEHICLES)
	{
		if(GetSlotState(x) == TRUE) { // It's inuse.
			pVehicle = m_pVehicles[x];

			if(m_bIsActive[x]) {
	
				if(pVehicle->GetHealth() == 0.0f) { // It's dead
					SetForRespawn(x);
				}
				else if(pVehicle->HasExceededWorldBoundries(
					pNetGame->m_WorldBounds[0],pNetGame->m_WorldBounds[1],
					pNetGame->m_WorldBounds[2],pNetGame->m_WorldBounds[3]))
				{
					if(pVehicle->IsOkToRespawn()) {
						SetForRespawn(x);
					}
				}
				else if(pVehicle->GetVehicleSubtype() != VEHICLE_SUBTYPE_BOAT &&
						pVehicle->GetVehicleSubtype() != VEHICLE_SUBTYPE_PLANE &&
						pVehicle->HasSunk()) // Not boat and has sunk.
				{
					SetForRespawn(x);
				}
				else 
				{
					if(pVehicle->IsDriverLocalPlayer()) {
						pVehicle->SetInvulnerable(FALSE);
					} else {
						pVehicle->SetInvulnerable(TRUE);
					}

					// Lock vehicles beyond given radius.
					if(pVehicle->GetDistanceFromLocalPlayerPed() > 300.0f) {
						pVehicle->SetLockedState(1);
					} else {
						pVehicle->SetLockedState(0);
					}

					if(pVehicle->GetVehicleSubtype() == VEHICLE_SUBTYPE_BIKE) {
						pVehicle->VerifyControlState();
					}

					// code to respawn vehicle after it has been idle for 4 minutes
					pVehicle->UpdateLastDrivenTime();

					if(pVehicle->m_bHasBeenDriven) {
						if((dwThisTime - pVehicle->m_dwTimeSinceLastDriven) > 250000) {
							SetForRespawn(x);
						}
					}						
				}
			}
			else // !m_bIsActive
			{
				if(m_iRespawnDelay[x] != 0) {
					m_iRespawnDelay[x]--;
				}
				else {
					if(pVehicle->IsOkToRespawn()) {
						Spawn(x,m_SpawnInfo[x].byteVehicleType,&m_SpawnInfo[x].vecPos,
							m_SpawnInfo[x].fRotation,m_SpawnInfo[x].iColor1,m_SpawnInfo[x].iColor2);
					}
				}	
			}		
		}

		x++;

	}
}

//----------------------------------------------------

void CVehiclePool::SetForRespawn(BYTE byteVehicleID)
{
	CVehicle *pVehicle = m_pVehicles[byteVehicleID];

	if(pVehicle->IsOkToRespawn()) {
		m_bIsActive[byteVehicleID] = FALSE;
		m_bIsWasted[byteVehicleID] = TRUE;
		m_iRespawnDelay[byteVehicleID] = 150;
	}
}

//----------------------------------------------------