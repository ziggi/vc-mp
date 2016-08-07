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

//----------------------------------------------------

class CVehiclePool
{
private:
	
	BOOL m_bVehicleSlotState[MAX_VEHICLES];
	CVehicle *m_pVehicles[MAX_VEHICLES];

public:
	
	CVehiclePool();
	~CVehiclePool();

	BOOL New(BYTE byteVehicleID, BYTE byteVehicleType, VECTOR * vecPos, float fRotation, int iColor1, int iColor2);
	BOOL Delete(BYTE byteVehicleID);	
		
	// Retrieve a vehicle by id
	CVehicle* GetAt(BYTE byteVehicleID) {
		if(byteVehicleID > MAX_VEHICLES) { return NULL; }
		return m_pVehicles[byteVehicleID];
	};

	// Find out if the slot is inuse.
	BOOL GetSlotState(BYTE byteVehicleID) {
		if(byteVehicleID > MAX_VEHICLES) { return FALSE; }
		return m_bVehicleSlotState[byteVehicleID];
	};

	BOOL InitialiseFromConfig(CConfig *pConfig);
	void Process();
};

//----------------------------------------------------