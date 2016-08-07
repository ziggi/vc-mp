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

#define VCMP_SAFECALL __stdcall

#define ENTITY_TYPE_UNKNOWN		0
#define ENTITY_TYPE_PED			1
#define ENTITY_TYPE_VEHICLE		2

int VCMP_SAFECALL GameGetWeaponModelFromWeapon(int iWeaponID);
void VCMP_SAFECALL GameDisableCheatCodes();
PED_TYPE * VCMP_SAFECALL GamePool_Ped_GetAt(int iID);
int VCMP_SAFECALL GamePool_Ped_GetIndex(PED_TYPE *pPed);
VEHICLE_TYPE * VCMP_SAFECALL GamePool_Vehicle_GetAt(int iID);
int VCMP_SAFECALL GamePool_Vehicle_GetIndex(VEHICLE_TYPE *pVehicle);
PED_TYPE * VCMP_SAFECALL GamePool_FindPlayerPed();
DWORD VCMP_SAFECALL TranslateColorCodeToRGBA(int iCode);
BOOL VCMP_SAFECALL GameIsEntityOnScreen(DWORD * pdwEnt);
void VCMP_SAFECALL InitPlayerPedPtrRecords();
void VCMP_SAFECALL SetPlayerPedPtrRecord(BYTE bytePlayer, DWORD dwPedPtr);
BYTE VCMP_SAFECALL FindPlayerNumFromPedPtr(DWORD dwPedPtr);

//-----------------------------------------------------------

