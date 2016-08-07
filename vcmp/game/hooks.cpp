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
#include "game.h"
#include "util.h"
#include "keystuff.h"

extern CNetGame* pNetGame;
extern CGame* pGame;

extern DWORD dwGameLoop;
extern DWORD dwRenderLoop;

#define NUDE void _declspec(naked) 

//-----------------------------------------------------------

PED_TYPE	*_pPlayer;
VEHICLE_TYPE *_pVehicle;
DWORD		*_pEntity;
int			_iWeapon;
float		_fUnk;
int			_iPedPieces;
BYTE		_byteUnk;

DWORD	dwStackFrame;
DWORD	dwCurPlayerActor=0;
BYTE	byteCurPlayer=0;
BYTE	byteInternalPlayer=0;
DWORD	dwCameraMode=0;
DWORD   dwRGBARadar=0;
int		iRadarColor1=0;
BYTE	byteSavedCameraMode;
BYTE	*pbyteCameraMode = (BYTE *)0x7E481C;
BYTE	*pbyteCurrentPlayer = (BYTE *)0xA10AFB; 
BYTE	byteUnkSave;

BOOL	bUsePassenger=FALSE;
BOOL	bIsPlayer=FALSE;

VEHICLE_TYPE *ObjectiveVehicle;

extern GTA_CONTROLSET *pGcsInternalKeys;

// CHEATING STUFF

DWORD GetMemSum(BYTE * mem, int size);

BOOL  bIsACheater=FALSE;
BOOL  bHasCheaterBeenPunished=FALSE;
BOOL  bHasSamplesOfPlayerData=FALSE;

DWORD dwSuspectedCheat=0;

DWORD dwStoredPlayerDataSum[2];
float fStoredPlayerHealth=0.0f;
float fStoredPlayerArmour=0.0f;
float fCheaterFlingSpeed = 0.0f;

//-----------------------------------------------------------

BYTE PreGameProcess_HookJmpCode[]	= {0xFF,0x25,0x77,0x5D,0x4A,0x00}; //4A5D77
BYTE PedSetObjective_HookJmpCode[]	= {0xFF,0x25,0x75,0x11,0x40,0x00,0x90,0x90,0x90};
BYTE RadarTranslateColor_HookJmpCode[] = {0xFF,0x25,0x44,0x30,0x4C,0x00,0x90}; // 4C3044
BYTE EnterCarAnimCallback_HookJmpCode[] = {0xFF,0x25,0xD8,0x28,0x51,0x00,0x90,0x90}; // 5128D8
BYTE HookedRand_HookJmpCode[] = {0xFF,0x25,0xE8,0x99,0x64,0x00,0x90,0x90}; // 6499E8
BYTE InflictDamage_HookJmpCode[] = {0xFF,0x25,0x15,0x5B,0x52,0x00}; // 525B15
BYTE InTheGame_HookJmpCode[] = {0xFF,0x25,0x3c,0x5c,0x4a,0x00}; //4A5C3C

//-----------------------------------------------------------

void DoCheatEntryChecking() 
{
	if( pNetGame &&
	    pNetGame->GetPlayerPool()->GetLocalPlayer()->IsActive() &&
		bHasSamplesOfPlayerData )
	{
		if(dwSuspectedCheat == 15) {
			bIsACheater = TRUE;
		}

		if(fStoredPlayerHealth != GamePool_FindPlayerPed()->fHealth) {
			dwSuspectedCheat++; return;
		}
		
		if(fStoredPlayerArmour != GamePool_FindPlayerPed()->fArmour) {
			dwSuspectedCheat++; return;
		}

		if(dwStoredPlayerDataSum[1] != GetMemSum((BYTE *)GamePool_FindPlayerPed()+1032,192)) {
			dwSuspectedCheat++; return;
		}
	}
	else {
		dwSuspectedCheat = 0;
	}
}

//-----------------------------------------------------------

void DoCheatExitStoring()
{	
	if(pNetGame) {
		if(pNetGame->GetPlayerPool()->GetLocalPlayer()->IsActive()) {

			dwStoredPlayerDataSum[1] = GetMemSum((BYTE *)GamePool_FindPlayerPed()+1032,192);
			fStoredPlayerHealth = GamePool_FindPlayerPed()->fHealth;
			fStoredPlayerArmour = GamePool_FindPlayerPed()->fArmour;

			bHasSamplesOfPlayerData = TRUE;
		} else {
			bHasSamplesOfPlayerData = FALSE;
		}
	}
}

//-----------------------------------------------------------

NUDE PreGameProcessHook()
{
	_asm mov dwStackFrame, esp
	_asm pushad

	_asm mov edx, dwGameLoop ; upcalls our main game loop handler
	_asm call edx
	
	DoCheatExitStoring();

	_asm popad

	_asm mov esp, dwStackFrame
	_asm push ebx
	_asm sub esp, 112
	_asm push 0
	_asm mov edx, ADDR_PRE_GAME_PROCESS ; back into the real proc
	_asm add edx, 6
	_asm jmp edx
}

//-----------------------------------------------------------

NUDE InTheGameHook()
{
	_asm pushad

	DoCheatEntryChecking();

	_asm popad

	_asm mov edx, 0x4A5D80
	_asm call edx
	_asm pop ecx

	_asm mov edx, 0x4A5BE6
	_asm jmp edx
}

//-----------------------------------------------------------

DWORD GetMemSum(BYTE * mem, int size)
{
	int		x=0;
	DWORD	ret=0;

	while(x!=size) {
		ret+=mem[x];
		x++;
	}

	return ret;
}

//-----------------------------------------------------------

void _stdcall DoOnFootWorldBoundsStuff()
{
	if(pNetGame) {
		CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
		CPlayerPed *pPlayerPed = pGame->FindPlayerPed();

		// Make sure they don't leave their worldy confines.
		if(pLocalPlayer->IsActive() && !pPlayerPed->IsInVehicle()) {
			if(pPlayerPed->EnforceWorldBoundries(
				pNetGame->m_WorldBounds[0],pNetGame->m_WorldBounds[1],
				pNetGame->m_WorldBounds[2],pNetGame->m_WorldBounds[3]))
			{
				pGcsInternalKeys->wKeys1[KEY_ONFOOT_JUMP] = 0xFF;
				pGcsInternalKeys->wKeys2[KEY_ONFOOT_JUMP] = 0x00;
				pPlayerPed->SetArmedWeapon(0);
				//pGame->DisplayTextMessage("Stay within the world boundries");
			}
		}
	}
}
	
//-----------------------------------------------------------
// A hook function that switches keys for
// CPlayerPed::ProcessControl(void)

NUDE CPlayerPed_ProcessControl_Hook()
{
	_asm mov dwCurPlayerActor, ecx
	_asm pushad

	byteInternalPlayer = *pbyteCurrentPlayer;
	byteCurPlayer = FindPlayerNumFromPedPtr(dwCurPlayerActor);

	if( dwCurPlayerActor && 
		byteCurPlayer != 0 &&
		byteInternalPlayer == 0 )
	{		
		// key switching
		GameStoreLocalPlayerKeys(); // remember local player's keys
		GameSetRemotePlayerKeys(byteCurPlayer); // set remote player's keys

		// save the internal cammode.
		byteSavedCameraMode = *pbyteCameraMode;
		*pbyteCameraMode = 4; // onfoot mouse looking mode.

		// aim switching
		GameStoreLocalPlayerAim();
		GameSetRemotePlayerAim(byteCurPlayer);

		*pbyteCurrentPlayer = byteCurPlayer; // Set the internal player to the passed actor

		// call the internal CPlayerPed[]::Process
		_asm popad
		_asm mov edx, 0x537270
		_asm call edx
		_asm pushad

		// restore the camera mode.
		*pbyteCameraMode = byteSavedCameraMode;

		// restore the local player's keys and the internal ID.
		*pbyteCurrentPlayer = 0;
		GameSetLocalPlayerKeys();
		GameSetLocalPlayerAim();
	}
	else // it's the local player
	{
		if(!bIsACheater) {
			DoOnFootWorldBoundsStuff();
		}
		else {
			if(!_pPlayer->byteIsInVehicle) {
				pGcsInternalKeys->wKeys1[KEY_ONFOOT_JUMP] = 0xFF;
				pGcsInternalKeys->wKeys2[KEY_ONFOOT_JUMP] = 0x00;
				_pPlayer = (PED_TYPE *)dwCurPlayerActor;
				_pPlayer->entity.vecMoveSpeed.Z = fCheaterFlingSpeed;
				fCheaterFlingSpeed+=0.025f;
			} else {
				_pPlayer->byteIsInVehicle = 0;
			}
		}

		_asm popad
		_asm mov edx, 0x537270
		_asm call edx
		_asm pushad
	}
	
	_asm popad
	_asm ret
}

//-----------------------------------------------------------
// Hook that replaces CBike::ProcessControl(void)

NUDE CBike_ProcessControl_Hook()
{
	_asm mov _pVehicle, ecx // store the CBike pointer.
	_asm pushad

	byteInternalPlayer = *pbyteCurrentPlayer;
	
	if( (_pVehicle->pDriver) &&
		(_pVehicle->pDriver != GamePool_FindPlayerPed()) &&
		(byteInternalPlayer==0) ) // not player's car
	{
		// get the current driver's player number
		byteCurPlayer = FindPlayerNumFromPedPtr((DWORD)_pVehicle->pDriver);
	
		GameStoreLocalPlayerKeys(); // save local player keys
		GameSetRemotePlayerKeys(byteCurPlayer); // set remote player keys.
		*pbyteCurrentPlayer = byteCurPlayer; // set internal ID to this remote player

		// call CBike::ProcessControl(void)
		_asm popad
		_asm mov eax, 0x60E3E0
		_asm call eax
		_asm pushad

		// restore local player keys and internal ID.
		*pbyteCurrentPlayer = 0;
		GameSetLocalPlayerKeys();
	}
	else
	{
		_asm popad
		_asm mov eax, 0x60E3E0
		_asm call eax
		_asm pushad
	}

	_asm popad
	_asm ret
}

//-----------------------------------------------------------
// Hook that replaces CBoat::ProcessControl(void)

NUDE CBoat_ProcessControl_Hook()
{
	_asm mov _pVehicle, ecx // store the CBoat pointer.
	_asm pushad

	byteInternalPlayer = *(BYTE *)0xA10AFB;
	
	if( (_pVehicle->pDriver) &&
		(_pVehicle->pDriver != GamePool_FindPlayerPed()) &&
		(byteInternalPlayer==0) ) // not player's car
	{
		// get the current driver's player number
		byteCurPlayer = FindPlayerNumFromPedPtr((DWORD)_pVehicle->pDriver);
	
		GameStoreLocalPlayerKeys(); // save local player keys
		GameSetRemotePlayerKeys(byteCurPlayer); // set remote player keys.
		*pbyteCurrentPlayer = byteCurPlayer; // set internal ID to this remote player

		// call CBoat::ProcessControl(void)
		_asm popad
		_asm mov eax, 0x59FE90
		_asm call eax
		_asm pushad

		// restore local player keys and internal ID.
		*pbyteCurrentPlayer = 0;
		GameSetLocalPlayerKeys();
	}
	else
	{
		_asm popad
		_asm mov eax, 0x59FE90
		_asm call eax
		_asm pushad
	}

	_asm popad
	_asm ret
}

//-----------------------------------------------------------
// Hook for CAutomobile::ProcessControl(void)

NUDE CAutomobile_ProcessControl_Hook()
{
	_asm mov _pVehicle, ecx
	_asm pushad

	byteInternalPlayer = *(BYTE *)0xA10AFB;
	
	if( (_pVehicle->pDriver) &&
		(_pVehicle->pDriver != GamePool_FindPlayerPed()) &&
		(byteInternalPlayer==0) ) // not player's car
	{
		// get the current driver's player number
		byteCurPlayer = FindPlayerNumFromPedPtr((DWORD)_pVehicle->pDriver);
	
		GameStoreLocalPlayerKeys(); // save local player keys
		GameSetRemotePlayerKeys(byteCurPlayer); // set remote player keys.
		*pbyteCurrentPlayer = byteCurPlayer; // set internal ID to this remote player

		// call CAutomobile::ProcessControl(void)
		_asm popad
		_asm mov edi, 0x593030
		_asm call edi
		_asm pushad

		// restore local player keys and internal ID.
		*pbyteCurrentPlayer = 0;
		GameSetLocalPlayerKeys();
	}
	else
	{
		_asm popad
		_asm mov edi, 0x593030
		_asm call edi
		_asm pushad
	}

	_asm popad
	_asm ret
}

//-----------------------------------------------------------

void _stdcall DoEnterVehicleNotification(BOOL bPassenger)
{
	/*
	if(pNetGame) {
		ObjectiveVehicle = (VEHICLE_TYPE *)_pVehicle;			
		pNetGame->GetPlayerPool()->GetLocalPlayer()
			->SendEnterVehicleNotification(pNetGame->GetVehiclePool()
			->FindIDFromGtaPtr(ObjectiveVehicle),bPassenger);
	}*/
}

//-----------------------------------------------------------

void _stdcall DoExitVehicleNotification()
{
	/*
	if(pNetGame) {
		ObjectiveVehicle = (VEHICLE_TYPE *)_pVehicle;
		if(ObjectiveVehicle->pDriver != GamePool_FindPlayerPed()) {
			pNetGame->GetPlayerPool()->GetLocalPlayer()
					->SendExitVehicleNotification(pNetGame->GetVehiclePool()
					->FindIDFromGtaPtr(ObjectiveVehicle));
		}
	}*/
}

//-----------------------------------------------------------
// Hooks CPed::SetObjective(enum eObjective)

NUDE CPed_SetObjective_Hook()
{	
	
}

//-----------------------------------------------------------

NUDE RadarTranslateColor()
{
	_asm mov eax, [esp+4]
	_asm mov iRadarColor1, eax
	TranslateColorCodeToRGBA(iRadarColor1); // return will still be in eax.
	_asm ret
}

//-----------------------------------------------------------

NUDE CantFindFuckingAnim()
{
	_asm mov eax, [esp+4]
	_asm test eax, eax
	_asm jnz its_ok

	_asm ret ; was 0, so foobarred

its_ok:

	_asm mov edx, 0x405AC0
	_asm add edx, 4

}

//-----------------------------------------------------------
// ok, this bullshit procedure don't check the fucking
// vehicle pointer for 0 and caused the widely hated 5128fb crash.

NUDE EnterCarAnimCallback_Hook()
{
	_asm mov edx, [esp+4]
    _asm mov eax, [esp+8]

	_asm mov _pPlayer, eax
	_asm pushad

	if( _pPlayer->pVehicle == 0 &&
	    _pPlayer != GamePool_FindPlayerPed()) {
		_asm popad
		_asm ret
	}

	_asm popad
	_asm mov ebp, 0x5128E0
	_asm add ebp, 8
	_asm jmp ebp	
}

//-----------------------------------------------------------
// The rand() function in GTA is hooked and we can
// control the seed this way.

NUDE HookedRand_Hook()
{
	rand();
	_asm ret
}

//-----------------------------------------------------------

#define NO_TEAM 255

BOOL _stdcall IsFriendlyFire(PED_TYPE *pPlayer,DWORD *pdwEnt, int iWeapon, float fUnk, int PedPiece, BYTE byteUnk)
{
	CPlayerPool *pPlayerPool;
	CRemotePlayer *pRemotePlayer;
	CLocalPlayer *pLocalPlayer;
	BYTE byteRemotePlayerID;
	BYTE byteLocalPlayerTeam;

	if(pPlayer == GamePool_FindPlayerPed()) {
		if(pNetGame && pNetGame->m_byteFriendlyFire) {
			
			pPlayerPool = pNetGame->GetPlayerPool();
			pLocalPlayer = pPlayerPool->GetLocalPlayer();
			byteLocalPlayerTeam = pLocalPlayer->GetTeam();

			if(pLocalPlayer->IsWasted() || (byteLocalPlayerTeam == NO_TEAM)) return FALSE;

			byteRemotePlayerID = pPlayerPool->FindRemotePlayerIDFromGtaPtr((PED_TYPE *)pdwEnt);

			if(byteRemotePlayerID != INVALID_PLAYER_ID) {
				pRemotePlayer = pPlayerPool->GetAt(byteRemotePlayerID);
				if(pRemotePlayer->GetTeam() == byteLocalPlayerTeam) {
					return TRUE;
				} else {
					return FALSE;
				}
			}

			// didn't find pdwEnt in the player pool.
			// this is where we could check for a vehicle.
		}
	}

	return FALSE;	
}

//-----------------------------------------------------------

NUDE CPed_InflictDamageHook()
{
	_asm mov dwStackFrame, esp
	_asm mov _pPlayer, ecx
	_asm mov eax, [esp+4]
	_asm mov _pEntity, eax
	_asm mov eax, [esp+8]
	_asm mov _iWeapon, eax
	_asm mov eax, [esp+12]
	_asm mov _fUnk, eax
	_asm mov eax, [esp+16]
	_asm mov _iPedPieces, eax
	_asm mov al, [esp+20]
	_asm mov _byteUnk, al
	_asm pushad

	if(pNetGame) {

		if(IsFriendlyFire(_pPlayer,_pEntity,_iWeapon,_fUnk,_iPedPieces,_byteUnk)) {
			_asm popad
			_asm mov esp, dwStackFrame
			_asm xor al, al
			_asm retn 0x14
		}

		/*
		if(_pPlayer == GamePool_FindPlayerPed()) {
			fLastDamageAmount = _fUnk;
			fLastHealth = _pPlayer->fHealth - fLastDamageAmount;
			bLastDamageProcessed = FALSE;
		}*/
	}

	_asm popad
	_asm mov esp, dwStackFrame
	_asm fld ds:[0x694170]
	_asm mov edx, 0x525B20
	_asm add edx, 6
	_asm jmp edx
}

//-----------------------------------------------------------

void InstallMethodHook(DWORD dwInstallAddress,DWORD dwHookFunction)
{
	DWORD dwVP, dwVP2;
	VirtualProtect((LPVOID)dwInstallAddress,4,PAGE_EXECUTE_READWRITE,&dwVP);
	*(PDWORD)dwInstallAddress = (DWORD)dwHookFunction;
	VirtualProtect((LPVOID)dwInstallAddress,4,dwVP,&dwVP2);
}

//-----------------------------------------------------------

void InstallHook( DWORD dwInstallAddress,
				  DWORD dwHookFunction,
				  DWORD dwHookStorage,
				  BYTE * pbyteJmpCode,
				  int iJmpCodeSize )
{
	DWORD dwVP, dwVP2;

	// Install the pointer to procaddr.
	VirtualProtect((PVOID)dwHookStorage,4,PAGE_EXECUTE_READWRITE,&dwVP);
	*(PDWORD)dwHookStorage = (DWORD)dwHookFunction;
	VirtualProtect((PVOID)dwHookStorage,4,dwVP,&dwVP2);

	// Install the Jmp code.
	VirtualProtect((PVOID)dwInstallAddress,iJmpCodeSize,PAGE_EXECUTE_READWRITE,&dwVP);		
	memcpy((PVOID)dwInstallAddress,pbyteJmpCode,iJmpCodeSize);
	VirtualProtect((PVOID)dwInstallAddress,iJmpCodeSize,dwVP,&dwVP2);	
}

//-----------------------------------------------------------

void GameInstallHooks()
{	
	InstallHook(0x6499F0,(DWORD)HookedRand_Hook,0x6499E8,HookedRand_HookJmpCode,
		sizeof(HookedRand_HookJmpCode));

	// Below is the Render2DStuff hook, don't be confused by the poor naming.
	InstallHook(ADDR_PRE_GAME_PROCESS,(DWORD)PreGameProcessHook,
		ADDR_PRE_GAME_PROCESS_STORAGE,PreGameProcess_HookJmpCode,
		sizeof(PreGameProcess_HookJmpCode));

	InstallHook(0x4A5BE0,(DWORD)InTheGameHook,0x4A5C3C,InTheGame_HookJmpCode,
		sizeof(InTheGame_HookJmpCode));
	
	// Install CPlayerPed::ProcessControl hook.
	InstallMethodHook(0x694D90,(DWORD)CPlayerPed_ProcessControl_Hook);

	// Install CBike::ProcessControl hook.
	InstallMethodHook(0x6D7B54,(DWORD)CBike_ProcessControl_Hook);

	// Install CBoat::ProcessControl hook.
	InstallMethodHook(0x69B0D4,(DWORD)CBoat_ProcessControl_Hook);

	// Install CAutomobile::ProcessControl hook.
	InstallMethodHook(0x69ADB0,(DWORD)CAutomobile_ProcessControl_Hook);
	
	/* Install CPed::SetObjective() hook.
	InstallHook(ADDR_SET_OBJECTIVE,(DWORD)CPed_SetObjective_Hook,
		ADDR_SET_OBJECTIVE_STORAGE,PedSetObjective_HookJmpCode,sizeof(PedSetObjective_HookJmpCode));*/
								
	// Install Hook for RadarTranslateColor
	InstallHook(0x4C3050,(DWORD)RadarTranslateColor,0x4C3044,
		RadarTranslateColor_HookJmpCode,sizeof(RadarTranslateColor_HookJmpCode));
	
	InstallHook(0x525B20,(DWORD)CPed_InflictDamageHook,0x525B15,
		InflictDamage_HookJmpCode,sizeof(InflictDamage_HookJmpCode));

	// Install Hook for enter car animation callback..
	// Update: Causing even more problems.
	InstallHook(0x5128E0,(DWORD)EnterCarAnimCallback_Hook,0x5128D8,
		EnterCarAnimCallback_HookJmpCode,sizeof(EnterCarAnimCallback_HookJmpCode));
	
	/* Hook/patch code to get around 0x405AC5 animation bug
	InstallHook(0x405AC0,(DWORD)CantFindFuckingAnim,0x405A95,
		CantFindFuckingAnim_HookJmpCode,sizeof(CantFindFuckingAnim_HookJmpCode));*/
}

//-----------------------------------------------------------