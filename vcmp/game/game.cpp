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

#include "game.h"
#include "util.h"
#include "keystuff.h"

void GameInstallHooks();

//-----------------------------------------------------------

CGame::CGame()
{
	m_pInternalCamera = new CCamera();
	m_pInternalPlayer = NULL;
}

//-----------------------------------------------------------

CPlayerPed *CGame::NewPlayer( int iPlayerID, int iModel,
							  float fPosX, float fPosY, float fPosZ,
							  float fRotation )
{
	CPlayerPed *pPlayerNew = new CPlayerPed(iPlayerID,iModel,fPosX,fPosY,fPosZ,fRotation);
	return pPlayerNew;
}

//-----------------------------------------------------------

CVehicle *CGame::NewVehicle(int iType, float fPosX, float fPosY,
							 float fPosZ, float fRotation)
{
	CVehicle *pVehicleNew = new	CVehicle(iType,fPosX,fPosY,fPosZ,fRotation);
	return pVehicleNew;
}

//-----------------------------------------------------------

int CGame::GetWeaponModelFromWeapon(int iWeaponID)
{
	return GameGetWeaponModelFromWeapon(iWeaponID);
}

//-----------------------------------------------------------

BYTE byteEnableInput1[] = {0xE8,0xB2,0xD9,0xFF,0xFF};
BYTE byteEnableInput2[] = {0xE8,0xF2,0xD9,0xFF,0xFF};
BYTE byteDisableInput[] = {0xB0,0x01,0x90,0x90,0x90};
BYTE byteEnablePadKeyBuf[] = {0x66,0xC7,0x04,0x5D,0x18,0x4A,0x86,0x00,0xFF,0x00};

void CGame::ToggleKeyInputsDisabled(BOOL bDisable)
{
	DWORD oldProt, oldProt2;

	if(bDisable == FALSE) {
		VirtualProtect((PVOID)0x602BDD,10,PAGE_EXECUTE_READWRITE,&oldProt);
		memcpy((PVOID)0x602BDD,byteEnablePadKeyBuf,10);
		VirtualProtect((PVOID)0x602BDD,10,oldProt,&oldProt2);

		VirtualProtect((PVOID)0x601349,5,PAGE_EXECUTE_READWRITE,&oldProt);
		memcpy((PVOID)0x601349,byteEnableInput2,5);
		VirtualProtect((PVOID)0x601349,5,oldProt,&oldProt2);

	} else { // TRUE
		VirtualProtect((PVOID)0x602BDD,10,PAGE_EXECUTE_READWRITE,&oldProt);
		memset((PVOID)0x602BDD,0x90,10);
		VirtualProtect((PVOID)0x602BDD,10,oldProt,&oldProt2);

		VirtualProtect((PVOID)0x601349,5,PAGE_EXECUTE_READWRITE,&oldProt);
		memcpy((PVOID)0x601349,byteDisableInput,5);
		VirtualProtect((PVOID)0x601349,5,oldProt,&oldProt2);
	}	
}

//-----------------------------------------------------------
// extern for the exception box

LONG WINAPI exc_handler(_EXCEPTION_POINTERS* exc_inf);

void CGame::StartGame()
{
	DWORD dwVP, dwVP2;
	
	InitPlayerPedPtrRecords();
	GameKeyStatesInit();
	GameAimSyncInit();
	
	SetUnhandledExceptionFilter(exc_handler);

	// Patch to prevent game stopping during a pause
	// (Credits to Luke)
	VirtualProtect((PVOID)0x4D0DA0,7,PAGE_EXECUTE_READWRITE,&dwVP);
	memset((PVOID)0x4D0DA0,0x90,7); // nop * 7
	VirtualProtect((PVOID)0x4D0DA0,7,dwVP,&dwVP2);

	// Patch for GetPlayerTableFromPlayerActor()
	VirtualProtect((PVOID)0x531D40,8,PAGE_EXECUTE_READWRITE,&dwVP);
	memset((PVOID)0x531D40,0x90,8); // nop * 8
	VirtualProtect((PVOID)0x531D40,8,dwVP,&dwVP2);

	// Patch for SetCurrentWeapon fixes fucking FindPlayerPed shit.4FF970
	VirtualProtect((PVOID)0x4FF970,1,PAGE_EXECUTE_READWRITE,&dwVP);
	*(BYTE *)0x4FF970 = 0x9D; // turns mov [eax+.. to mov [ebp+..
	VirtualProtect((PVOID)0x4FF970,1,dwVP,&dwVP2);
	
	// Player weapon pickups.
	VirtualProtect((PVOID)0x4F6538,1,PAGE_EXECUTE_READWRITE,&dwVP);
	*(BYTE *)0x4F6538 = 0x75;
	VirtualProtect((PVOID)0x4F6538,1,dwVP,&dwVP2);

	// Patch CUserDisplay::Process() to not call CPlacename::Process()
	VirtualProtect((PVOID)0x4D1405,5,PAGE_EXECUTE_READWRITE,&dwVP);
	memset((PVOID)0x4D1405,0x90,5); // nop * 5
	VirtualProtect((PVOID)0x4D1405,5,dwVP,&dwVP2);
	
	// Patch CPed::RefreshSkin() so that it doesn't try to set any
	// animations 50D96A
	VirtualProtect((PVOID)0x50D96A,5,PAGE_EXECUTE_READWRITE,&dwVP);
	memset((PVOID)0x50D96A,0x90,5); // nop * 5
	VirtualProtect((PVOID)0x50D96A,5,dwVP,&dwVP2);

	// Patch clear weapons to not set armed weapon to fists.4FF767
	VirtualProtect((PVOID)0x4FF767,7,PAGE_EXECUTE_READWRITE,&dwVP);
	memset((PVOID)0x4FF767,0x90,7); // nop * 7
	VirtualProtect((PVOID)0x4FF767,7,dwVP,&dwVP2);
	
	/* DoDriveByShootings CWeapon::Update call. 5C9817
	VirtualProtect((PVOID)0x5C9817,8,PAGE_EXECUTE_READWRITE,&dwVP);
	memset((PVOID)0x5C9817,0x90,8); // nop * 8
	VirtualProtect((PVOID)0x5C9817,8,dwVP,&dwVP2);*/

	// Patch to modify the scm path
	VirtualProtect((PVOID)0x6886AC,9,PAGE_EXECUTE_READWRITE,&dwVP);
	strcpy((PCHAR)0x6886AC,"vcmp.scm");	
	VirtualProtect((PVOID)0x6886AC,9,dwVP,&dwVP2);

	// Patch to modify the scm path
	VirtualProtect((PVOID)0x6D7368,14,PAGE_EXECUTE_READWRITE,&dwVP);
	strcpy((PCHAR)0x6D7368,"data\vcmp.scm");	
	VirtualProtect((PVOID)0x6D7368,14,dwVP,&dwVP2);

	/* Patch to modify the gxt path
	VirtualProtect((PVOID)0x69A4B0,256,PAGE_EXECUTE_READWRITE,&dwVP);
	strcpy((PCHAR)0x69A4B0,"vcmp.gxt");
	strcpy((PCHAR)0x69A4C0,"vcmp.gxt");
	strcpy((PCHAR)0x69A4CC,"vcmp.gxt");
	strcpy((PCHAR)0x69A4D8,"vcmp.gxt");
	strcpy((PCHAR)0x69A4E4,"vcmp.gxt");
	VirtualProtect((PVOID)0x69A4B0,256,dwVP,&dwVP2);*/

	// Patch to modify the loadsc0 txd
	VirtualProtect((PVOID)0x6D5E9C,16,PAGE_EXECUTE_READWRITE,&dwVP);
	strcpy((PCHAR)0x6D5E9C,"ldvcmp0");
	VirtualProtect((PVOID)0x6D5E9C,16,dwVP,&dwVP2);

	VirtualProtect((PVOID)0x68E594,16,PAGE_EXECUTE_READWRITE,&dwVP);
	strcpy((PCHAR)0x68E594,"ldvcmp0");
	VirtualProtect((PVOID)0x68E594,16,dwVP,&dwVP2);

	VirtualProtect((PVOID)0x68E6F4,16,PAGE_EXECUTE_READWRITE,&dwVP);
	strcpy((PCHAR)0x68E6F4,"ldvcmp0");
	VirtualProtect((PVOID)0x68E6F4,16,dwVP,&dwVP2);
	
	// Patch to increase vehicle pool limit from 110 to 200
	VirtualProtect((PVOID)0x4C02E4,128,PAGE_EXECUTE_READWRITE,&dwVP);
	*(BYTE *)0x4C02E4 = 0x6A;
	*(BYTE *)0x4C02E5 = 0x00; // push 0 (unused param)
	*(BYTE *)0x4C02E6 = 0x68;
	*(BYTE *)0x4C02E7 = 0xC8;
	*(BYTE *)0x4C02E8 = 0x00;
	*(BYTE *)0x4C02E9 = 0x00;
	*(BYTE *)0x4C02EA = 0x00; // push 200
	VirtualProtect((PVOID)0x4C02E4,128,dwVP,&dwVP2);

	// Patch for cAudioManager::GetPedCommentSfx() 5EA1FC
	VirtualProtect((PVOID)0x5EA1FC,1,PAGE_EXECUTE_READWRITE,&dwVP);
	*(BYTE *)0x5EA1FC = 0x75; // jnz
	VirtualProtect((PVOID)0x5EA1FC,1,dwVP,&dwVP2);

	/* ProcessVehicleOneShots (Reverse logic for CPed::IsPlayer()
	VirtualProtect((PVOID)0x5EB6CD,1,PAGE_EXECUTE_READWRITE,&dwVP);
	*(BYTE *)0x5EB6CD = 0x85; // jnz
	VirtualProtect((PVOID)0x5EB6CD,1,dwVP,&dwVP2);*/

	// For passenger engine audio hack (applied later).
	VirtualProtect((PVOID)0x5F2175,2,PAGE_EXECUTE_READWRITE,&dwVP);

	// For 537723 bug (not checking in vehicle)
	VirtualProtect((PVOID)0x537723,2,PAGE_EXECUTE_READWRITE,&dwVP);
	*(BYTE *)0x537723 = 0x90;
	*(BYTE *)0x537724 = 0x90;
	VirtualProtect((PVOID)0x537723,2,dwVP,&dwVP2);

	// For Bike/Passenger bug (test driver for 0)
	VirtualProtect((PVOID)0x5C91F5,4,PAGE_EXECUTE_READWRITE,&dwVP);
	*(BYTE *)0x5C91F5 = 0x85;
	*(BYTE *)0x5C91F6 = 0xC9; // test eax, eax to test ecx, ecx
	*(BYTE *)0x5C91F8 = 0x09; // jz +9 - exit if 0
	VirtualProtect((PVOID)0x5C91F5,4,dwVP,&dwVP2);

	// For Boat driveby bug (test driver for 0)
	VirtualProtect((PVOID)0x5C9558,4,PAGE_EXECUTE_READWRITE,&dwVP);
	*(BYTE *)0x5C9558 = 0x85;
	*(BYTE *)0x5C9559 = 0xC9; // test eax, eax to test ecx, ecx
	*(BYTE *)0x5C955B = 0x09; // jz +9 - exit if 0
	VirtualProtect((PVOID)0x5C9558,4,dwVP,&dwVP2);

	// Hack to prevent replays.
	VirtualProtect((PVOID)0x4A45C3,5,PAGE_EXECUTE_READWRITE,&dwVP);
	memset((PVOID)0x4A45C3,0x90,5); // nop * 5
	VirtualProtect((PVOID)0x4A45C3,5,dwVP,&dwVP2);

	// Hack to get rid of the evasive dive.
	VirtualProtect((PVOID)0x4F6A20,3,PAGE_EXECUTE_READWRITE,&dwVP);
	*(BYTE *)0x4F6A20 = 0xC2;
	*(BYTE *)0x4F6A21 = 0x08;
	*(BYTE *)0x4F6A22 = 0x00;
	VirtualProtect((PVOID)0x4F6A20,3,dwVP,&dwVP2);

	// Hack to get rid of the weapon pickup message (Tab)
	VirtualProtect((PVOID)0x440B2C,5,PAGE_EXECUTE_READWRITE,&dwVP);
	memset((PVOID)0x440B2C,0x90,5); // nop * 5
	VirtualProtect((PVOID)0x440B2C,5,dwVP,&dwVP2);
	
	// Install all hooks
	GameInstallHooks();

	GameDisableCheatCodes();

	*(DWORD *)ADDR_MENU = 0;
	*(DWORD *)ADDR_STARTGAME = 1;
}

//-----------------------------------------------------------

BOOL CGame::IsMenuActive()
{
	if(*(DWORD *)ADDR_MENU != 0) 
		return TRUE;
	else
		return FALSE;
}

//-----------------------------------------------------------

void CGame::RequestModel(int iModelID)
{
	ScriptCommand(&request_model, iModelID);
}

//-----------------------------------------------------------

void CGame::LoadRequestedModels()
{
	ScriptCommand(&load_requested_models);
}

//-----------------------------------------------------------

BOOL CGame::IsModelLoaded(int iModelID)
{
	if(ScriptCommand(&is_model_available, iModelID)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

//-----------------------------------------------------------

void CGame::DisplayHud(BOOL bSwitch)
{
	if(bSwitch)
		*(BYTE *)ADDR_HUD_SWITCH = 1;
	else
		*(BYTE *)ADDR_HUD_SWITCH = 0;
}
	
//-----------------------------------------------------------

void CGame::ToggleFrameLimiterState(BOOL bState)
{
	if(bState) {
		*(BYTE *)ADDR_FRAME_LIMITER = 1;
	}
	else {
		*(BYTE *)ADDR_FRAME_LIMITER = 0;
	}
}

//-----------------------------------------------------------

void CGame::DisplayTextMessage(PCHAR szText)
{

}

//-----------------------------------------------------------

