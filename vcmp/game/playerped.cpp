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

#include <windows.h>

#include "../main.h"
#include "game.h"
#include "util.h"
#include "keystuff.h"
#include "aimstuff.h"

extern CGame *pGame;
extern CNetGame *pNetGame;

//-----------------------------------------------------------

char szSpecialActorModels[][32] =
{"igdiaz","igpercy","igken","igcandy","ighlary","igjezz","igphil",
"sam","igsonny","igbuddy","igmerc","igdick","igalscb","igbudy2",
"igbudy3","igcolon","igdick","iggonz","ighlry2","igmerc2","igmike",
"igmike2", "igphil2", "igphil3","bounca","burger","cdrivra","cdrivrb",
"cgona","cgonb","cgonc","chef","cmraman","courier","crewa","crewb","cservra",
"dgoona","dgoonb","mba","mbb","mgoona","mporna","printra","printrb","printrc","psycho",
"sgc","spandxa","spandxb","stripa","stripb","stripc","S_keep"};

//-----------------------------------------------------------
// Obtain an instance of CPlayerPed for the local player.

CPlayerPed::CPlayerPed()
{
	m_dwGTAId = 1; // 0x001
	m_pPed = GamePool_FindPlayerPed();
	m_bytePlayerNumber = 0;
}

//-----------------------------------------------------------
// This is the constructor for creating new player.

CPlayerPed::CPlayerPed( int iPlayerNumber, int iModel, float fX,
					    float fY,float fZ,float fRotation )
{
	DWORD dwPlayerID=0;

	m_pPed=0;
	m_dwGTAId=0;

	if((iModel < 107) && !pGame->IsModelLoaded(iModel)) {
		pGame->RequestModel(iModel);
		pGame->LoadRequestedModels();
		while(!pGame->IsModelLoaded(iModel)) { Sleep(1); }
	}

	ScriptCommand(&create_player, &iPlayerNumber, fX, fY, fZ, &dwPlayerID);
	ScriptCommand(&create_actor_from_player,&iPlayerNumber,&dwPlayerID);
	ScriptCommand(&toggle_player_infinite_run,iPlayerNumber,1);
	SetZAngle(fRotation);
	
	m_dwGTAId = dwPlayerID;
	m_pPed = GamePool_Ped_GetAt(m_dwGTAId);
	m_bytePlayerNumber = (BYTE)iPlayerNumber;
	SetPlayerPedPtrRecord(m_bytePlayerNumber,(DWORD)m_pPed);
	ScriptCommand(&set_actor_immunities,m_dwGTAId,1,1,1,1,1);

	SetModel(iModel);
}

//-----------------------------------------------------------

CPlayerPed::~CPlayerPed(){}

//-----------------------------------------------------------

void CPlayerPed::Destroy()
{
	DWORD dwPedPtr = (DWORD)m_pPed;
	_asm mov ecx, dwPedPtr
	_asm mov ebx, [ecx] ; vtable
	_asm push 1
	_asm call [ebx+8] ; destroy
	m_pPed = NULL;
}

//-----------------------------------------------------------

void CPlayerPed::SetKeys(WORD wKeys)
{
	GTA_CONTROLSET * pPlayerKeys = GameGetPlayerKeys(m_bytePlayerNumber);
	
	if(wKeys & 1) {
		pPlayerKeys->wKeys2[KEY_INCAR_HANDBRAKE] = 0xFF;
		pPlayerKeys->wKeys1[KEY_INCAR_HANDBRAKE] = 0xFF;
	} else { 
		pPlayerKeys->wKeys2[KEY_INCAR_HANDBRAKE] = 0x00;
		pPlayerKeys->wKeys1[KEY_INCAR_HANDBRAKE] = 0x00;
	}
	wKeys >>= 1; // 1
	
	if(wKeys & 1) {
		pPlayerKeys->wKeys2[KEY_INCAR_LOOKR] = 0xFF;
		pPlayerKeys->wKeys1[KEY_INCAR_LOOKR] = 0xFF;
	} else { 
		pPlayerKeys->wKeys2[KEY_INCAR_LOOKR] = 0x00;
		pPlayerKeys->wKeys1[KEY_INCAR_LOOKR] = 0x00;
	}
	wKeys >>= 1; // 2

	if(wKeys & 1) {
		pPlayerKeys->wKeys2[KEY_INCAR_LOOKL] = 0xFF;
		pPlayerKeys->wKeys1[KEY_INCAR_LOOKL] = 0xFF;
	} else { 
		pPlayerKeys->wKeys2[KEY_INCAR_LOOKL] = 0x00;
		pPlayerKeys->wKeys1[KEY_INCAR_LOOKL] = 0x00;
	}
	wKeys >>= 1; // 3

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_INCAR_TURRETUD] = 0xFF80;
		pPlayerKeys->wKeys2[KEY_INCAR_TURRETUD] = 0xFF80;
	} else {
		pPlayerKeys->wKeys1[KEY_INCAR_TURRETUD] = 0x0;
		pPlayerKeys->wKeys2[KEY_INCAR_TURRETUD] = 0x0;
	}		
	wKeys >>= 1; // 4

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_INCAR_TURRETUD] = 0x80;
		pPlayerKeys->wKeys2[KEY_INCAR_TURRETUD] = 0x80;
	} else {
		pPlayerKeys->wKeys1[KEY_INCAR_TURRETUD] = 0x0;
		pPlayerKeys->wKeys2[KEY_INCAR_TURRETUD] = 0x0;
	}		
	wKeys >>= 1; // 5

	if(wKeys & 1)
		pPlayerKeys->wKeys2[KEY_ONFOOT_CROUCH] = 0xFF;
	else
		pPlayerKeys->wKeys2[KEY_ONFOOT_CROUCH] = 0x00;
	wKeys >>= 1; // 6

	if(wKeys & 1)
		pPlayerKeys->wKeys2[KEY_ONFOOT_FIRE] = 0xFF;
	else
		pPlayerKeys->wKeys2[KEY_ONFOOT_FIRE] = 0x00;
	wKeys >>= 1; // 7

	if(wKeys & 1)
		pPlayerKeys->wKeys2[KEY_ONFOOT_JUMP] = 0xFF;
	else
		pPlayerKeys->wKeys2[KEY_ONFOOT_JUMP] = 0x00;
	wKeys >>= 1; // 8

	if(wKeys & 1)
		pPlayerKeys->wKeys1[KEY_ONFOOT_CROUCH] = 0xFF;
	else
		pPlayerKeys->wKeys1[KEY_ONFOOT_CROUCH] = 0x00;
	wKeys >>= 1; // 9

	if(wKeys & 1)
		pPlayerKeys->wKeys1[KEY_ONFOOT_FIRE] = 0xFF;
	else
		pPlayerKeys->wKeys1[KEY_ONFOOT_FIRE] = 0x00;
	wKeys >>= 1; // 10

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_ONFOOT_SPRINT] = 0xFF;
		pPlayerKeys->wKeys2[KEY_ONFOOT_SPRINT] = 0xFF;
	}
	else {
		pPlayerKeys->wKeys1[KEY_ONFOOT_SPRINT] = 0x00;
		pPlayerKeys->wKeys2[KEY_ONFOOT_SPRINT] = 0x00;
	}
	wKeys >>= 1; // 11

	if(wKeys & 1)
		pPlayerKeys->wKeys1[KEY_ONFOOT_JUMP] = 0xFF;
	else
		pPlayerKeys->wKeys1[KEY_ONFOOT_JUMP] = 0x00;
	wKeys >>= 1; // 12

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_ONFOOT_RIGHT] = 0xFF;
		pPlayerKeys->wKeys2[KEY_ONFOOT_RIGHT] = 0xFF;
	}
	else {
		pPlayerKeys->wKeys1[KEY_ONFOOT_RIGHT] = 0x00;
		pPlayerKeys->wKeys2[KEY_ONFOOT_RIGHT] = 0x00;
	}
	wKeys >>= 1; // 13

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_ONFOOT_LEFT] = 0xFF;
		pPlayerKeys->wKeys2[KEY_ONFOOT_LEFT] = 0xFF;
	}
	else {
		pPlayerKeys->wKeys1[KEY_ONFOOT_LEFT] = 0x00;
		pPlayerKeys->wKeys2[KEY_ONFOOT_LEFT] = 0x00;
	}
	wKeys >>= 1; // 14

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_ONFOOT_BACKWARD] = 0xFF;
		pPlayerKeys->wKeys2[KEY_ONFOOT_BACKWARD] = 0xFF;
	}
	else {
		pPlayerKeys->wKeys1[KEY_ONFOOT_BACKWARD] = 0x00;
		pPlayerKeys->wKeys2[KEY_ONFOOT_BACKWARD] = 0x00;
	}
	wKeys >>= 1; // 15
	
	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_ONFOOT_FORWARD] = 0xFF;
		pPlayerKeys->wKeys2[KEY_ONFOOT_FORWARD] = 0xFF;
	}
	else {
		pPlayerKeys->wKeys1[KEY_ONFOOT_FORWARD] = 0x00;
		pPlayerKeys->wKeys2[KEY_ONFOOT_FORWARD] = 0x00;
	}

	GameStoreRemotePlayerKeys(m_bytePlayerNumber,pPlayerKeys);
}

//-----------------------------------------------------------

WORD CPlayerPed::GetKeys()
{
	WORD wRet=0;

	GTA_CONTROLSET *pInternalKeys = GameGetInternalKeys();

	// Just down keys
	if(pInternalKeys->wKeys1[KEY_ONFOOT_FORWARD]) wRet |= 1;
	wRet <<= 1;

	if(pInternalKeys->wKeys1[KEY_ONFOOT_BACKWARD]) wRet |= 1;
	wRet <<= 1;
	
	if(pInternalKeys->wKeys1[KEY_ONFOOT_LEFT]) wRet |= 1;
	wRet <<= 1;

	if(pInternalKeys->wKeys1[KEY_ONFOOT_RIGHT]) wRet |= 1;
	wRet <<= 1;

	if(pInternalKeys->wKeys1[KEY_ONFOOT_JUMP]) wRet |= 1;
	wRet <<= 1;

	if(pInternalKeys->wKeys1[KEY_ONFOOT_SPRINT]) wRet |= 1;
	wRet <<=1;

	if(pInternalKeys->wKeys1[KEY_ONFOOT_FIRE]) wRet |= 1;
	wRet <<= 1;

	if(pInternalKeys->wKeys1[KEY_ONFOOT_CROUCH]) wRet |= 1;
	wRet <<= 1;

	// Jump held
	if(pInternalKeys->wKeys2[KEY_ONFOOT_JUMP]) wRet |= 1;
	wRet <<= 1;

	// Fire held
	if(pInternalKeys->wKeys2[KEY_ONFOOT_FIRE]) wRet |= 1;
	wRet <<= 1;

	// Crouch held
	if(pInternalKeys->wKeys2[KEY_ONFOOT_CROUCH]) wRet |= 1;
	wRet <<= 1;
	
	if(pInternalKeys->wKeys1[KEY_INCAR_TURRETUD] == 0x80) wRet |= 1;
	wRet <<= 1;
	
	if(pInternalKeys->wKeys1[KEY_INCAR_TURRETUD] == 0xFF80) wRet |= 1;
	wRet <<= 1;

	if(pInternalKeys->wKeys2[KEY_INCAR_LOOKL]) wRet |= 1;
	wRet <<= 1;

	if(pInternalKeys->wKeys2[KEY_INCAR_LOOKR]) wRet |= 1;
	wRet <<= 1;

	if(pInternalKeys->wKeys1[KEY_INCAR_HANDBRAKE]) wRet |= 1;
	
	return wRet;
}

//-----------------------------------------------------------

CAMERA_AIM * CPlayerPed::GetCurrentAim()
{
	return GameGetInternalAim();
}

//-----------------------------------------------------------

void CPlayerPed::SetCurrentAim(CAMERA_AIM *pAim)
{
	GameStoreRemotePlayerAim(m_bytePlayerNumber, pAim);
}

//-----------------------------------------------------------

BOOL CPlayerPed::EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY)
{
	MATRIX4X4 matWorld;
	VECTOR vecMoveSpeed;

	if(!m_pPed) return FALSE;

	GetMatrix(&matWorld);
	GetMoveSpeedVector(&vecMoveSpeed);

	if(matWorld.vPos.X > fPX) // greatest X coord check
	{
		if(vecMoveSpeed.X != 0.0f) {
			vecMoveSpeed.X = -0.25f;
		}
		SetMoveSpeedVector(vecMoveSpeed);
		return TRUE;
	}
	else if(matWorld.vPos.X < fZX)  // least X coord check
	{
		if(vecMoveSpeed.X != 0.0f) {
			vecMoveSpeed.X = 0.25f;
		}
		SetMoveSpeedVector(vecMoveSpeed);
		return TRUE;
	}
	else if(matWorld.vPos.Y > fPY) // Y coord check
	{
		if(vecMoveSpeed.Y != 0.0f) {
			vecMoveSpeed.Y = -0.25f;
		}

		SetMoveSpeedVector(vecMoveSpeed);
		return TRUE;
	}
	else if(matWorld.vPos.Y < fNY)
	{
		if(vecMoveSpeed.Y != 0.0f) {
			vecMoveSpeed.Y = 0.25f;
		}

		SetMoveSpeedVector(vecMoveSpeed);
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------

BOOL CPlayerPed::HasAmmoForCurrentWeapon()
{
	int iAmmo;

	if(m_pPed) {

		if(GamePool_Ped_GetAt(m_dwGTAId) == 0) return FALSE;

		int iWeapon = (int)GetCurrentWeapon();

		// mellee weapons always have ammo
		if(iWeapon < 12) return TRUE;

		ScriptCommand(&get_player_weapon_ammo,m_bytePlayerNumber,iWeapon,&iAmmo);

		if(iAmmo) {
			return TRUE;
		} else {
			return FALSE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------

BYTE CPlayerPed::GetCurrentWeapon()
{
	DWORD dwRetVal;
	if(GamePool_Ped_GetAt(m_dwGTAId) == 0) return 0;
	ScriptCommand(&get_player_armed_weapon,m_bytePlayerNumber,&dwRetVal);
	return (BYTE)dwRetVal;
}

//-----------------------------------------------------------

void CPlayerPed::SetInitialState()
{
	if(m_pPed) m_pPed->byteJumpFlags = 0xA;
}

//-----------------------------------------------------------

int CPlayerPed::GetCurrentVehicleID()
{
	VEHICLE_TYPE *pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;
	return GamePool_Vehicle_GetIndex(pVehicle);
}

//-----------------------------------------------------------

void CPlayerPed::ShowMarker(int iMarkerColor)
{	
	DWORD hndMarker;
	DWORD dwPedID;
	float f=0.0f;

	dwPedID = GamePool_Ped_GetIndex(m_pPed);

	_asm push 2
	_asm push 4
	_asm mov eax, dwPedID
	_asm push eax
	_asm push 2
	_asm mov edx, ADDR_TIE_MARKER_TO_ACTOR
	_asm call edx
	_asm mov hndMarker, eax
	_asm pop ecx
	_asm pop ecx
	_asm pop ecx
	_asm pop ecx

	_asm push iMarkerColor
	_asm push hndMarker
	_asm mov edx, ADDR_SET_MARKER_COLOR
	_asm call edx
	_asm pop ecx
	_asm pop ecx

	_asm push 2
	_asm push hndMarker
	_asm mov edx, ADDR_SHOW_ON_RADAR1
	_asm call edx
	_asm pop ecx
	_asm pop ecx

}

//-----------------------------------------------------------

BOOL CPlayerPed::IsOnScreen()
{
	if(m_pPed) {
		return GameIsEntityOnScreen((DWORD *)m_pPed);
	}
	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::Say(UINT uiNum)
{
	if(m_pPed)
	{
		DWORD dwPedPtr = (DWORD)m_pPed;
		_asm mov ecx, dwPedPtr
		_asm push uiNum
		_asm mov edx, ADDR_CPED_SAY
		_asm call edx
	}
}

//-----------------------------------------------------------

void CPlayerPed::SetZAngle(float z)
{
	if(m_pPed) {
		ScriptCommand(&set_player_z_angle,m_bytePlayerNumber,z);
	}
}

//-----------------------------------------------------------

float CPlayerPed::GetHealth()
{	
	return m_pPed->fHealth;
}

//-----------------------------------------------------------

void CPlayerPed::SetHealth(float fHealth)
{	
	m_pPed->fHealth = fHealth;
}	

//-----------------------------------------------------------

float CPlayerPed::GetArmour()
{	
	return m_pPed->fArmour;
}

//-----------------------------------------------------------

void CPlayerPed::SetArmour(float fArmour)
{	
	m_pPed->fArmour = fArmour;
}	

//-----------------------------------------------------------

BOOL CPlayerPed::IsDead()
{
	if(m_pPed->fHealth > 0.0f) return FALSE;
	return TRUE;
}

//-----------------------------------------------------------

BYTE CPlayerPed::GetAction()
{
	return m_pPed->byteAction;
}

//-----------------------------------------------------------

void CPlayerPed::SetAction(BYTE byteTrigger)
{
	m_pPed->byteAction = byteTrigger;
}
//-----------------------------------------------------------

BYTE CPlayerPed::GetShootingFlags()
{
	return m_pPed->byteShootFlags;
}

//-----------------------------------------------------------

void CPlayerPed::SetShootingFlags(BYTE byteShooting)
{
	m_pPed->byteShootFlags= byteShooting;
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsInVehicle()
{
	if(m_pPed->byteIsInVehicle) {
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

float CPlayerPed::GetRotation()
{
	return (m_pPed->fRotation1);
}

//-----------------------------------------------------------

void CPlayerPed::SetRotation(float fRotation)
{
	m_pPed->fRotation1 = fRotation;
	m_pPed->fRotation2 = fRotation;
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsAPassenger()
{
	if( m_pPed->byteIsInVehicle &&
		m_pPed->pVehicle )
	{
		VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;

		if(pVehicle->pDriver != m_pPed) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------

VEHICLE_TYPE * CPlayerPed::GetGtaVehicle()
{
	return (VEHICLE_TYPE *)m_pPed->pVehicle;
}

//-----------------------------------------------------------

void CPlayerPed::GetMatrix(PMATRIX4X4 Matrix)
{
	Matrix->vLookRight.X = m_pPed->entity.mat.vLookRight.X;
	Matrix->vLookRight.Y = m_pPed->entity.mat.vLookRight.Y;
	Matrix->vLookRight.Z = m_pPed->entity.mat.vLookRight.Z;
	Matrix->vLookUp.X = m_pPed->entity.mat.vLookUp.X;
	Matrix->vLookUp.Y = m_pPed->entity.mat.vLookUp.Y;
	Matrix->vLookUp.Z = m_pPed->entity.mat.vLookUp.Z;
	Matrix->vLookAt.X = m_pPed->entity.mat.vLookAt.X;
	Matrix->vLookAt.Y = m_pPed->entity.mat.vLookAt.Y;
	Matrix->vLookAt.Z = m_pPed->entity.mat.vLookAt.Z;
	Matrix->vPos.X = m_pPed->entity.mat.vPos.X;
	Matrix->vPos.Y = m_pPed->entity.mat.vPos.Y;
	Matrix->vPos.Z = m_pPed->entity.mat.vPos.Z;
}

//-----------------------------------------------------------

void CPlayerPed::SetMatrix(MATRIX4X4 Matrix)
{
	m_pPed->entity.mat.vLookRight.X = Matrix.vLookRight.X;
	m_pPed->entity.mat.vLookRight.Y = Matrix.vLookRight.Y;
	m_pPed->entity.mat.vLookRight.Z = Matrix.vLookRight.Z;
	m_pPed->entity.mat.vLookUp.X = Matrix.vLookUp.X;
	m_pPed->entity.mat.vLookUp.Y = Matrix.vLookUp.Y;
	m_pPed->entity.mat.vLookUp.Z = Matrix.vLookUp.Z;
	m_pPed->entity.mat.vLookAt.X = Matrix.vLookAt.X;
	m_pPed->entity.mat.vLookAt.Y = Matrix.vLookAt.Y;
	m_pPed->entity.mat.vLookAt.Z = Matrix.vLookAt.Z;
	m_pPed->entity.mat.vPos.X = Matrix.vPos.X;
	m_pPed->entity.mat.vPos.Y = Matrix.vPos.Y;
	m_pPed->entity.mat.vPos.Z = Matrix.vPos.Z;
}

//-----------------------------------------------------------

void CPlayerPed::GetMoveSpeedVector(PVECTOR Vector)
{
	Vector->X = m_pPed->entity.vecMoveSpeed.X;
	Vector->Y = m_pPed->entity.vecMoveSpeed.Y;
	Vector->Z = m_pPed->entity.vecMoveSpeed.Z;
}

//-----------------------------------------------------------

void CPlayerPed::SetMoveSpeedVector(VECTOR Vector)
{
	m_pPed->entity.vecMoveSpeed.X = Vector.X;
	m_pPed->entity.vecMoveSpeed.Y = Vector.Y;
	m_pPed->entity.vecMoveSpeed.Z = Vector.Z;
}

//-----------------------------------------------------------

void CPlayerPed::GetTurnSpeedVector(PVECTOR Vector)
{
	Vector->X = m_pPed->entity.vecTurnSpeed.X;
	Vector->Y = m_pPed->entity.vecTurnSpeed.Y;
	Vector->Z = m_pPed->entity.vecTurnSpeed.Z;
}

//-----------------------------------------------------------

void CPlayerPed::SetTurnSpeedVector(VECTOR Vector)
{
	m_pPed->entity.vecTurnSpeed.X = Vector.X;
	m_pPed->entity.vecTurnSpeed.Y = Vector.Y;
	m_pPed->entity.vecTurnSpeed.Z = Vector.Z;
}

//-----------------------------------------------------------

void CPlayerPed::GiveWeapon(int iWeaponID, int iAmmo)
{
	int iModelID = GameGetWeaponModelFromWeapon(iWeaponID);

	if(!pGame->IsModelLoaded(iModelID)) {
		pGame->RequestModel(iModelID);
		pGame->LoadRequestedModels();
		while(!pGame->IsModelLoaded(iModelID)) Sleep(1);
	}

	ScriptCommand(&give_player_weapon,m_bytePlayerNumber,iWeaponID,iAmmo);
	SetArmedWeapon(iWeaponID);
}

//-----------------------------------------------------------

void CPlayerPed::CheckAndRepairInvisProblems()
{
	if( (m_pPed->byteAction != ACTION_EXITING_VEHICLE) &&
		((m_pPed->byteIsInVehicle) || (m_pPed->byteAction == 50)) ) {
		// action 50 is driving vehicle.

		m_pPed->byteIsInVehicle = 0;
		m_pPed->byteAction = 1;
	}
}

//-----------------------------------------------------------

void CPlayerPed::Teleport(float x, float y, float z)
{
	DWORD dwPedPtr = (DWORD)m_pPed;
	_asm mov ecx, dwPedPtr
	_asm push z
	_asm push y
	_asm push x
	_asm mov edx, 0x4F5690
	_asm call edx
}

//-----------------------------------------------------------

void CPlayerPed::ClearTargetAndVehicle()
{

}

//-----------------------------------------------------------

void CPlayerPed::ClearAllWeapons()
{
	ScriptCommand(&remove_player_weapons,m_bytePlayerNumber);
}

//-----------------------------------------------------------

void CPlayerPed::SetArmedWeapon(int iWeaponType)
{
	ScriptCommand(&set_player_armed_weapon,m_bytePlayerNumber,iWeaponType);
}

//-----------------------------------------------------------

void CPlayerPed::PutDirectlyInVehicle(int iVehicleID)
{
	DWORD dwPlayerID = m_bytePlayerNumber;
	ScriptCommand(&put_player_in_car,dwPlayerID,iVehicleID);
			
	VEHICLE_TYPE * pVehicle = GamePool_Vehicle_GetAt(iVehicleID);
}

//-----------------------------------------------------------

void CPlayerPed::EnterVehicleAsDriver(int iVehicleID)
{
	VEHICLE_TYPE *pVehicle = GamePool_Vehicle_GetAt(iVehicleID);

	if(pVehicle) {
		SetObjective((PDWORD)pVehicle,18); // enter as driver
	}
}

//-----------------------------------------------------------

void CPlayerPed::EnterVehicleAsPassenger(int iVehicleID)
{
	VEHICLE_TYPE *pVehicle = GamePool_Vehicle_GetAt(iVehicleID);

	if(pVehicle) {
		SetObjective((PDWORD)pVehicle,17); // enter as passenger
	}
}

//-----------------------------------------------------------

void CPlayerPed::ExitCurrentVehicle()
{
	if(m_pPed->byteIsInVehicle) {
		SetObjective((PDWORD)m_pPed->pVehicle,16);
	}
}

//-----------------------------------------------------------

void CPlayerPed::RemoveFromVehicleAndPutAt(float fX, float fY, float fZ)
{
	if(m_pPed) {

		VEHICLE_TYPE *pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;
		ScriptCommand(&put_player_at_and_remove_from_car,m_bytePlayerNumber,fX,fY,fZ);

		/*
		if(m_pPed->entity.nModelIndex <= 106) {
			SetModel(m_pPed->entity.nModelIndex); // attempts to bypass model destruction probs
		}*/
	}
}

//-----------------------------------------------------------

void CPlayerPed::ForceIntoPassengerSeat(UINT uiVehicleID, UINT uiSeat)
{
	PED_TYPE * pPed = m_pPed;
	VEHICLE_TYPE * pVehicle = GamePool_Vehicle_GetAt(uiVehicleID);
	UINT uiPassengerOffset;

	if(!pPed || !pVehicle) return;
	if(uiSeat > pVehicle->byteMaxPassengers) return;

	pPed->byteAction = 50;

	pVehicle->pPassengers[uiSeat-1] = pPed;
	uiPassengerOffset = 424 + (uiSeat * 4);

	_asm mov     eax, pVehicle
	_asm add	 eax, uiPassengerOffset
	_asm push    eax
	_asm mov     eax, pVehicle
	_asm mov     ecx, pPed
	_asm mov	 edx, ADDR_ACTOR_PUT_IN_VEHICLE
	_asm call    edx

	_asm mov	 ebx, pPed
	_asm mov     eax, pVehicle
	_asm mov     [ebx+936], eax
	_asm lea     eax, [ebx+936]
	_asm mov     ecx, [ebx+936]
	_asm push    eax
	_asm mov	 edx, ADDR_ACTOR_PUT_IN_VEHICLE
	_asm call    edx

	pPed->byteIsInVehicle = 1;
	pPed->entity.nControlFlags2 &= 0xFE;

	_asm push 0
	_asm push pVehicle
	_asm mov ecx, pPed
	_asm mov edx, ADDR_VEHICLE_SET_DRIVER
	_asm call edx
}

//-----------------------------------------------------------

void CPlayerPed::SetModel(int iModel)
{
	PCHAR			szModelName=0;
	DWORD dwPedPtr = (DWORD)m_pPed;

	if(iModel == 8) return; // invalid skin
	
	if(iModel > 106)
	{
		iModel -= 106;
		if(iModel < 54)
		{
			szModelName = szSpecialActorModels[iModel];

			_asm mov ecx, dwPedPtr
			_asm push szModelName
			_asm mov edx, ADDR_SET_SKIN_MODELNAME
			_asm call edx

			_asm push 0
			_asm mov edx, ADDR_LOAD_REQUESTED_MODELS2
			_asm call edx
			_asm pop ecx

			_asm mov ecx, dwPedPtr
			_asm mov edx, ADDR_REFRESH_ACTOR_SKIN
			_asm call edx
		}
	}
	else // default.ide number
	{
		if(!pGame->IsModelLoaded(iModel)) {
			pGame->RequestModel(iModel);
			pGame->LoadRequestedModels();
			while(!pGame->IsModelLoaded(iModel)) Sleep(1);
		}

		_asm mov ecx, dwPedPtr
		_asm push iModel
		_asm mov edx, ADDR_SET_ACTOR_SKIN
		_asm call edx
	}
}

//-----------------------------------------------------------

int CPlayerPed::GetModel()
{
	return m_pPed->entity.nModelIndex;	
}

//-----------------------------------------------------------

void CPlayerPed::SetObjective(PDWORD pObjectiveEntity, int iFunction)
{
	DWORD dwPedPtr = (DWORD)m_pPed;

	if(dwPedPtr && pObjectiveEntity)
	{
		_asm mov ecx, dwPedPtr
		_asm push pObjectiveEntity
		_asm push iFunction
		_asm mov edx, ADDR_SET_OBJECTIVE
		_asm call edx
	}
}

//-----------------------------------------------------------

void CPlayerPed::RestartIfWastedAt(VECTOR *vecRestart, float fRotation)
{
	ScriptCommand(&restart_if_wasted_at,vecRestart->X,vecRestart->Y,
		vecRestart->Z,fRotation);
}

//-----------------------------------------------------------

void CPlayerPed::TogglePlayerControllable(int iControllable)
{
	DWORD dwPlayerID=0;
	ScriptCommand(&toggle_player_controllable,dwPlayerID,iControllable);
}

//-----------------------------------------------------------

void CPlayerPed::SetDead()
{
	if(m_pPed) {
		m_pPed->fHealth = 0.0f;
		DWORD dwPed = (DWORD)m_pPed;
		_asm mov ecx, dwPed
		_asm mov edx, 0x4F6430
		_asm call edx
	}
}

//-----------------------------------------------------------

BYTE CPlayerPed::FindDeathReasonAndResponsiblePlayer(BYTE * nPlayer)
{
	BYTE byteDeathReason;
	BYTE bytePlayerIDWhoKilled;
	CVehiclePool *pVehiclePool;
	CPlayerPool *pPlayerPool;

	// grab the vehicle/player pool now anyway, even though we may not need it.
	if(pNetGame) {
		pVehiclePool = pNetGame->GetVehiclePool();
		pPlayerPool = pNetGame->GetPlayerPool();
	}
	else { // just leave if there's no netgame.
		*nPlayer = INVALID_PLAYER_ID;
		return INVALID_PLAYER_ID;
	}

	if(m_pPed) 
	{
		byteDeathReason = (BYTE)m_pPed->dwWeaponUsed;

		if(byteDeathReason < WEAPON_CAMERA) { // It's a weapon of some sort.

			if(m_pPed->pdwDamageEntity) { // check for a player pointer.
				
				bytePlayerIDWhoKilled = pPlayerPool->
					FindRemotePlayerIDFromGtaPtr((PED_TYPE *)m_pPed->pdwDamageEntity);

				if(bytePlayerIDWhoKilled != INVALID_PLAYER_ID) {
					// killed by another player with a weapon, this is all easy.
					*nPlayer = bytePlayerIDWhoKilled;
					return byteDeathReason;
				}
			}
			else { // weapon was used but who_killed is 0 (?)
				*nPlayer = INVALID_PLAYER_ID;
				return INVALID_PLAYER_ID;
			}
		}
		else if(byteDeathReason == WEAPON_DROWN) {
			*nPlayer = INVALID_PLAYER_ID;
			return WEAPON_DROWN;
		}
		else if(byteDeathReason == WEAPON_DRIVEBY) {

			if(m_pPed->pdwDamageEntity) {
				// now, if we can find the vehicle
				// we can probably derive the responsible player.
				// Look in the vehicle pool for this vehicle.
				if(pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE *)m_pPed->pdwDamageEntity) != (-1))
				{
					VEHICLE_TYPE *pGtaVehicle = (VEHICLE_TYPE *)m_pPed->pdwDamageEntity;

					bytePlayerIDWhoKilled = pPlayerPool->
						FindRemotePlayerIDFromGtaPtr((PED_TYPE *)pGtaVehicle->pDriver);
											
					if(bytePlayerIDWhoKilled != INVALID_PLAYER_ID) {
						*nPlayer = bytePlayerIDWhoKilled;
						return WEAPON_DRIVEBY;
					}
				}									
			}
		}
		else if(byteDeathReason == WEAPON_COLLISION) {

			if(m_pPed->pdwDamageEntity) {
				if(pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE *)m_pPed->pdwDamageEntity) != (-1))
				{
					VEHICLE_TYPE *pGtaVehicle = (VEHICLE_TYPE *)m_pPed->pdwDamageEntity;
										
					bytePlayerIDWhoKilled = pPlayerPool->
						FindRemotePlayerIDFromGtaPtr((PED_TYPE *)pGtaVehicle->pDriver);
						
					if(bytePlayerIDWhoKilled != INVALID_PLAYER_ID) {
						*nPlayer = bytePlayerIDWhoKilled;
						return WEAPON_COLLISION;
					}
				}									
			}
		}
	}

	// Unhandled death type.
	*nPlayer = INVALID_PLAYER_ID;
	return INVALID_PLAYER_ID;
}

//-----------------------------------------------------------

UINT CPlayerPed::GetPassengerSeat()
{
	UINT x=0;
	VEHICLE_TYPE *pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;

	if(pVehicle) {
		while(x!=7) {
			if(pVehicle->pPassengers[x] == m_pPed) {
				return (x+1);
			}
			x++;
		}	
	}
	return 0;
}

//-----------------------------------------------------------