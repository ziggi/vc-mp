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

#include "netgame.h"

//----------------------------------------------------

CVehiclePool::CVehiclePool()
{
	BYTE byteVehicleID = 0;
	while(byteVehicleID != MAX_VEHICLES) {
		m_bVehicleSlotState[byteVehicleID] = FALSE;
		m_pVehicles[byteVehicleID] = NULL;
		byteVehicleID++;
	}
}

//----------------------------------------------------

BOOL CVehiclePool::InitialiseFromConfig(CConfig *pConfig)
{
    int iVehicleArrayCount;
    BYTE d=1;

    int iModelID;
    VECTOR vecPos;
    float fRotation;
    int iColor1, iColor2;
    char * szConfigLine;

    iVehicleArrayCount = pConfig->GetConfigArrayCount("VEHICLE");
	iVehicleArrayCount++;

    while((int)d < iVehicleArrayCount) {

         szConfigLine = pConfig->GetConfigEntryAsString("VEHICLE",d);

		 sscanf(szConfigLine,"%d%f%f%f%f%d%d",&iModelID,
                   &vecPos.X,&vecPos.Y,&vecPos.Z,&fRotation,&iColor1,&iColor2);

         New(d,(BYTE)iModelID,&vecPos,fRotation,iColor1,iColor2);
         d++;
    }

    return TRUE;
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

BOOL CVehiclePool::New(BYTE byteVehicleID, BYTE byteVehicleType,
					   VECTOR * vecPos, float fRotation,
					   int iColor1, int iColor2)
{
	m_pVehicles[byteVehicleID] = new CVehicle(byteVehicleType,vecPos,fRotation,iColor1,iColor2);

	/*
	logprintf("CVehiclePool::New(%u,%.2f,%.2f,%.2f,%.2f,%d,%d)",
		byteVehicleType,vecPos->X,vecPos->Y,vecPos->Z,fRotation,iColor1,iColor2);*/

	if(m_pVehicles[byteVehicleID])
	{
		m_pVehicles[byteVehicleID]->SetID(byteVehicleID);
		m_bVehicleSlotState[byteVehicleID] = TRUE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------

BOOL CVehiclePool::Delete(BYTE byteVehicleID)
{
	if(!GetSlotState(byteVehicleID) || !m_pVehicles[byteVehicleID])
	{
		return FALSE;
	}

	m_bVehicleSlotState[byteVehicleID] = FALSE;
	delete m_pVehicles[byteVehicleID];
	m_pVehicles[byteVehicleID] = NULL;

	return TRUE;
}

//----------------------------------------------------

void CVehiclePool::Process()
{
	
}

//----------------------------------------------------