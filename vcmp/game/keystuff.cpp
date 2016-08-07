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

#include <stdio.h>
#include <windows.h>
#include "common.h"
#include "keystuff.h"
#include "address.h"

//-----------------------------------------------------------

GTA_CONTROLSET *pGcsInternalKeys = (GTA_CONTROLSET *)ADDR_KEYSTATES;
GTA_CONTROLSET GcsLocalPlayerKeys;
GTA_CONTROLSET GcsRemotePlayerKeys[MAX_PLAYERS];

BYTE	byteSaveDriveByLeft;
BYTE	byteSaveDriveByRight;
BYTE   *pbyteDriveByLeft =  (BYTE *)0x7E4818;
BYTE   *pbyteDriveByRight = (BYTE *)0x7E4819;

//-----------------------------------------------------------

void GameKeyStatesInit()
{
	int x=0;
	memset(&GcsLocalPlayerKeys,0,sizeof(GTA_CONTROLSET));
	while(x!=MAX_PLAYERS) {		
		memset(&GcsRemotePlayerKeys[x],0,sizeof(GTA_CONTROLSET));
		x++;
	}
}

//-----------------------------------------------------------

void GameStoreLocalPlayerKeys()
{	
	memcpy(&GcsLocalPlayerKeys,pGcsInternalKeys,sizeof(GTA_CONTROLSET));

	// save current driveby state
	byteSaveDriveByLeft = *pbyteDriveByLeft;
	byteSaveDriveByRight = *pbyteDriveByRight;
}

//-----------------------------------------------------------

void GameSetLocalPlayerKeys()
{
	memcpy(pGcsInternalKeys,&GcsLocalPlayerKeys,sizeof(GTA_CONTROLSET));

	// restore the driveby state also
	*pbyteDriveByLeft = byteSaveDriveByLeft;
	*pbyteDriveByRight = byteSaveDriveByRight;
}

//-----------------------------------------------------------

void GameStoreRemotePlayerKeys(int iPlayer, GTA_CONTROLSET * pGcsKeyStates)
{
	memcpy(&GcsRemotePlayerKeys[iPlayer],pGcsKeyStates,sizeof(GTA_CONTROLSET));
}

//-----------------------------------------------------------

void GameSetRemotePlayerKeys(int iPlayer)
{
	// memcpy(&GcsRemotePlayerKeys[iPlayer],&GcsLocalPlayerKeys,sizeof(GTA_CONTROLSET));

	memcpy(pGcsInternalKeys,&GcsRemotePlayerKeys[iPlayer],sizeof(GTA_CONTROLSET));

	if(GcsRemotePlayerKeys[iPlayer].wKeys1[KEY_INCAR_LOOKL]) {
		*pbyteDriveByLeft = 1;
	} else {
		*pbyteDriveByLeft = 0;
	}	

	if(GcsRemotePlayerKeys[iPlayer].wKeys1[KEY_INCAR_LOOKR]) {
		*pbyteDriveByRight = 1;
	} else {
		*pbyteDriveByRight = 0;
	}
}

//-----------------------------------------------------------

GTA_CONTROLSET * GameGetInternalKeys()
{
	return pGcsInternalKeys;
}

//-----------------------------------------------------------

GTA_CONTROLSET * GameGetLocalPlayerKeys()
{
	return &GcsLocalPlayerKeys;
}

//-----------------------------------------------------------

GTA_CONTROLSET * GameGetPlayerKeys(int iPlayer)
{
	return &GcsRemotePlayerKeys[iPlayer];
}

//-----------------------------------------------------------

void GameResetPlayerKeys(int iPlayer)
{
	memset(&GcsRemotePlayerKeys[iPlayer],0,sizeof(GTA_CONTROLSET));
}

//-----------------------------------------------------------

void GameResetLocalKeys()
{
	memset(pGcsInternalKeys,0,80);
}

//-----------------------------------------------------------