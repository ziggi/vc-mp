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
#include <stdio.h>
#include "game.h"
#include "../main.h"

DWORD dwPlayerPedPtrs[MAX_PLAYERS];

#define VCMP_SAFECALL __stdcall

//-----------------------------------------------------------

void VCMP_SAFECALL GameDisableCheatCodes()
{
	DWORD p1,p2;
    VirtualProtect((void*)0x602BDC,16,PAGE_EXECUTE_READWRITE,&p1);
	*(BYTE *)0x602BDC = 0x90;
    memset((void*)0x602BE7,0x90,5);
    VirtualProtect((void*)0x602BDC,16,p1,&p2);
}

//-----------------------------------------------------------

PED_TYPE * VCMP_SAFECALL GamePool_Ped_GetAt(int iID)
{
	PED_TYPE *pActorRet;

	_asm mov ebx, ADDR_PED_TABLE
	_asm mov ecx, [ebx]
	_asm push iID
	_asm mov ebx, ADDR_ACTOR_FROM_ID
	_asm call ebx
	_asm mov pActorRet, eax

	return pActorRet;	
}

//-----------------------------------------------------------

int VCMP_SAFECALL GamePool_Ped_GetIndex(PED_TYPE *pPed)
{
	int iRetVal;

	_asm mov ebx, ADDR_PED_TABLE
	_asm mov ecx, [ebx]
	_asm push pPed
	_asm mov ebx, ADDR_ID_FROM_ACTOR
	_asm call ebx
	_asm mov iRetVal, eax

	return iRetVal;
}

//-----------------------------------------------------------

VEHICLE_TYPE * VCMP_SAFECALL GamePool_Vehicle_GetAt(int iID)
{	
	VEHICLE_TYPE *pVehicleRet;

	_asm mov ebx, ADDR_VEHICLE_TABLE
	_asm mov ecx, [ebx]
	_asm push iID
	_asm mov ebx, ADDR_VEHICLE_FROM_ID
	_asm call ebx
	_asm mov pVehicleRet, eax

	return pVehicleRet;
}

//-----------------------------------------------------------

int VCMP_SAFECALL GamePool_Vehicle_GetIndex(VEHICLE_TYPE *pVehicle)
{
	int iRetVal;

	_asm mov ebx, ADDR_VEHICLE_TABLE
	_asm mov ecx, [ebx]
	_asm push pVehicle
	_asm mov ebx, ADDR_ID_FROM_VEHICLE
	_asm call ebx
	_asm mov iRetVal, eax

	return iRetVal;		
}

//-----------------------------------------------------------
// Return the PED_TYPE * of the local player.

PED_TYPE * VCMP_SAFECALL GamePool_FindPlayerPed()
{
	return GamePool_Ped_GetAt(1);
}

//-----------------------------------------------------------
// Convert weapon ID to ide model ID

int VCMP_SAFECALL GameGetWeaponModelFromWeapon(int iWeaponID)
{
	switch(iWeaponID)
	{
	case WEAPON_BRASSKNUCKLE:
		return WEAPON_MODEL_BRASSKNUCKLE;

	case WEAPON_SCREWDRIVER:
		return WEAPON_MODEL_SCREWDRIVER;

	case WEAPON_GOLFCLUB:
		return WEAPON_MODEL_GOLFCLUB;

	case WEAPON_NITESTICK:
		return WEAPON_MODEL_NITESTICK;

	case WEAPON_KNIFECUR:
		return WEAPON_MODEL_KNIFECUR;

	case WEAPON_BASEBALL_BAT:
		return WEAPON_MODEL_BASEBALL_BAT;

	case WEAPON_HAMMER:
		return WEAPON_MODEL_HAMMER;

	case WEAPON_CLEAVER:
		return WEAPON_MODEL_CLEAVER;

	case WEAPON_MACHETE:
		return WEAPON_MODEL_MACHETE;

	case WEAPON_KATANA:
		return WEAPON_MODEL_KATANA;

	case WEAPON_CHAINSAW:
		return WEAPON_MODEL_CHAINSAW;

	case WEAPON_GRENADE:
		return WEAPON_MODEL_GRENADE;

	case WEAPON_TEARGAS:
		return WEAPON_MODEL_TEARGAS;

	case WEAPON_MOLOTOV:
		return WEAPON_MODEL_MOLOTOV;

	case WEAPON_MISSILE:
		return WEAPON_MODEL_MISSILE;

	case WEAPON_COLT45:
		return WEAPON_MODEL_COLT45;

	case WEAPON_PYTHON:
		return WEAPON_MODEL_PYTHON;

	case WEAPON_CHROMEGUN:
		return WEAPON_MODEL_CHROMEGUN;

	case WEAPON_SHOTGSPA:
		return WEAPON_MODEL_SHOTGSPA;

	case WEAPON_BUDDYSHOT:
		return WEAPON_MODEL_BUDDYSHOT;

	case WEAPON_TEC9:
		return WEAPON_MODEL_TEC9;

	case WEAPON_UZI:
		return WEAPON_MODEL_UZI;

	case WEAPON_INGRAMSL:
		return WEAPON_MODEL_INGRAMSL;

	case WEAPON_MP5LNG:
		return WEAPON_MODEL_MP5LNG;

	case WEAPON_M4:
		return WEAPON_MODEL_M4;

	case WEAPON_RUGER:
		return WEAPON_MODEL_RUGER;

	case WEAPON_SNIPER:
		return WEAPON_MODEL_SNIPER;

	case WEAPON_LASER:
		return WEAPON_MODEL_LASER;

	case WEAPON_ROCKETLA:
		return WEAPON_MODEL_ROCKETLA;

	case WEAPON_FLAME:
		return WEAPON_MODEL_FLAME;

	case WEAPON_M60:
		return WEAPON_MODEL_M60;

	case WEAPON_MINIGUN:
		return WEAPON_MODEL_MINIGUN;

	case WEAPON_BOMB:
		return WEAPON_MODEL_BOMB;

	case WEAPON_HELICANNON:
		return WEAPON_MODEL_M60;

	case WEAPON_CAMERA:
		return WEAPON_MODEL_CAMERA;
	}

	return -1;
}

//-----------------------------------------------------------

DWORD dwHudColors[] = {
0x778899FF, // light slate grey
0xFF8C13FF, // dark orange
0xC715FFFF, // Medium violet red
0x20B2AAFF, // sea green
0xFFD720FF, // gold
0xDC143CFF, // crimson
0x6495EDFF, // cornflower blue
0xFF1493FF, // deeppink
0xF4A460FF, // sandy
0xEE82EEFF, // violet
0x8b4513FF, // chocolate
0xf0e68cFF, // khaki
0x148b8bFF, // dark cyan
0x14ff7fFF, // spring green
0x556b2fFF, // olive green
0x191970FF // midnight blue
};

DWORD VCMP_SAFECALL TranslateColorCodeToRGBA(int iCode)
{
	if(iCode < sizeof(dwHudColors)) {
		return dwHudColors[iCode];
	} else {
		return 0xFFFFFFFF;
	}
}

//-----------------------------------------------------------

BOOL VCMP_SAFECALL GameIsEntityOnScreen(DWORD * pdwEnt)
{
	BYTE byteRet=0;

	_asm mov ecx, pdwEnt
	_asm mov edx, ADDR_ENTITY_ONSCREEN
	_asm call edx
	_asm mov byteRet, al

	if(byteRet) return TRUE;

	return FALSE;
}

//-----------------------------------------------------------

void VCMP_SAFECALL InitPlayerPedPtrRecords() 
{
	memset(&dwPlayerPedPtrs[0],0,sizeof(DWORD) * MAX_PLAYERS);
}

//-----------------------------------------------------------

void VCMP_SAFECALL SetPlayerPedPtrRecord(BYTE bytePlayer, DWORD dwPedPtr) 
{
	dwPlayerPedPtrs[bytePlayer] = dwPedPtr;
}

//-----------------------------------------------------------

BYTE VCMP_SAFECALL FindPlayerNumFromPedPtr(DWORD dwPedPtr)
{
	BYTE x = 0;
	while(x != MAX_PLAYERS)	{
		if(dwPlayerPedPtrs[x] == dwPedPtr) return x;
		x++;
	}
	return 0;
}

//-----------------------------------------------------------
