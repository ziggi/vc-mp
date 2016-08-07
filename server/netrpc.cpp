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
#include "rcon.h"

RakServerInterface		*pRak=0;
extern CNetGame			*pNetGame;
extern char				*szAdminPass;
extern CRcon			*pRcon;

#ifndef WIN32
#	define stricmp strcasecmp
#endif

#define NETGAME_VERSION 6

#define REJECT_REASON_BAD_VERSION   1
#define REJECT_REASON_BAD_NICKNAME  2

void FilterInvalidNickChars(PCHAR szString);

//----------------------------------------------------
// Sent by a client who's wishing to join us in our
// multiplayer-like activities.

void ClientJoin(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	RakNet::BitStream bsReject;
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	CHAR szPlayerName[MAX_PLAYER_NAME];
	BYTE bytePlayerID;
	BYTE byteVersion;
	BYTE byteNickLen;
	BYTE byteRejectReason;

	bsData.Read(byteVersion);

	if(byteVersion != NETGAME_VERSION) {
		byteRejectReason = REJECT_REASON_BAD_VERSION;
		bsReject.Write(byteRejectReason);
		pRak->RPC("ConnectionRejected",&bsReject,HIGH_PRIORITY,RELIABLE,0,sender,FALSE,FALSE);
		return;
	}

	bsData.Read(byteNickLen);
	bsData.Read(szPlayerName,byteNickLen);
	szPlayerName[byteNickLen] = '\0';

	FilterInvalidNickChars(szPlayerName);
	byteNickLen = strlen(szPlayerName);

	if(byteNickLen==0 || byteNickLen > 16 || pPlayerPool->IsNickInUse(szPlayerName)) {
		byteRejectReason = REJECT_REASON_BAD_NICKNAME;
		bsReject.Write(byteRejectReason);
		pRak->RPC("ConnectionRejected",&bsReject,HIGH_PRIORITY,RELIABLE,0,sender,FALSE,FALSE);
		return;
	}

	bytePlayerID = pRak->GetIndexFromPlayerID(sender);
	
	// Add this client to the player pool.
	pPlayerPool->New(bytePlayerID, szPlayerName);

	// Send this client back an 'InitGame' RPC
	RakNet::BitStream bsInitGame;
	bsInitGame.Write((float)pNetGame->m_vecInitPlayerPos.X);
	bsInitGame.Write((float)pNetGame->m_vecInitPlayerPos.Y);
	bsInitGame.Write((float)pNetGame->m_vecInitPlayerPos.Z);
	bsInitGame.Write((float)pNetGame->m_vecInitCameraPos.X);
	bsInitGame.Write((float)pNetGame->m_vecInitCameraPos.Y);
	bsInitGame.Write((float)pNetGame->m_vecInitCameraPos.Z);
	bsInitGame.Write((float)pNetGame->m_vecInitCameraLook.X);
	bsInitGame.Write((float)pNetGame->m_vecInitCameraLook.Y);
	bsInitGame.Write((float)pNetGame->m_vecInitCameraLook.Z);
	bsInitGame.Write((float)pNetGame->m_WorldBounds[0]);
	bsInitGame.Write((float)pNetGame->m_WorldBounds[1]);
	bsInitGame.Write((float)pNetGame->m_WorldBounds[2]);
	bsInitGame.Write((float)pNetGame->m_WorldBounds[3]);
	bsInitGame.Write(pNetGame->m_iSpawnsAvailable);
	bsInitGame.Write(pNetGame->m_byteFriendlyFire);
	bsInitGame.Write(pNetGame->m_byteShowOnRadar);
	bsInitGame.Write(bytePlayerID);
	pRak->RPC("InitGame",&bsInitGame,HIGH_PRIORITY,RELIABLE_ORDERED,0,sender,FALSE,FALSE);

	// Send this client ServerJoins for every existing player
	BYTE x=0;
	RakNet::BitStream * pbsExistingClient;

	while(x!=MAX_PLAYERS) {
		if( (pPlayerPool->GetSlotState(x) == TRUE) && 
			(x != bytePlayerID) ) {

			pbsExistingClient = new RakNet::BitStream();

			pbsExistingClient->Write(x);
			pbsExistingClient->Write(strlen(pPlayerPool->GetPlayerName(x)));
			pbsExistingClient->Write(pPlayerPool->GetPlayerName(x),strlen(pPlayerPool->GetPlayerName(x)));
			pRak->RPC("ServerJoin",pbsExistingClient,HIGH_PRIORITY,RELIABLE_ORDERED,0,sender,FALSE,FALSE);
	
			delete pbsExistingClient;

			// Now also spawn the player for them if they're active.
			CPlayer *pSpawnPlayer = pPlayerPool->GetAt(x);
			if(pSpawnPlayer->IsActive()) {
				pSpawnPlayer->SpawnForPlayer(bytePlayerID);
			}
		}
		x++;
	}

	// Spawn all existing vehicles for player.
	CVehicle *pVehicle;
	x=0;

	while(x!=MAX_VEHICLES) {
		if(pVehiclePool->GetSlotState(x) == TRUE) {
			pVehicle = pVehiclePool->GetAt(x);
			if(pVehicle->IsActive()) pVehicle->SpawnForPlayer(bytePlayerID);
		}
		x++;
	}

	pNetGame->GetGameLogic()->HandleClientJoin(bytePlayerID);
}

//----------------------------------------------------
// Sent by client with global chat text

void Chat(PCHAR Data, int iBitLength, PlayerID sender)
{
	CHAR szText[256];
	BYTE byteTextLen;
	CPlayerPool *pPool = pNetGame->GetPlayerPool();

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	bsData.Read(byteTextLen);
	bsData.Read(szText,byteTextLen);
	szText[byteTextLen] = '\0';

	if(!pPool->GetSlotState(pRak->GetIndexFromPlayerID(sender))) return;

	logprintf("[%s]: %s",
		pPool->GetPlayerName(pRak->GetIndexFromPlayerID(sender)),
		szText);

	pRcon->OutputChatMessage(pPool->GetPlayerName(pRak->GetIndexFromPlayerID(sender)),
		szText);

	// Now pass it off to all the other clients.
	RakNet::BitStream bsSend;
	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);
	bsSend.Write(bytePlayerID);
	bsSend.Write(byteTextLen);
	bsSend.Write(szText,byteTextLen);

	pRak->RPC("Chat",&bsSend,HIGH_PRIORITY,RELIABLE,0,sender,TRUE,FALSE);
}

//----------------------------------------------------
// Sent by client who wishes to request a class from
// the gamelogic handler.

void RequestClass(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	
	BYTE byteRequestedClass;
	BYTE byteRequestOutcome = 0;
	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);
	bsData.Read(byteRequestedClass);

	if(!pNetGame->GetPlayerPool()->GetSlotState(bytePlayerID)) return;
	
	if(pNetGame->GetGameLogic()->HandleSpawnClassRequest(bytePlayerID,byteRequestedClass)) 
	{
		byteRequestOutcome = 1;
	}

	RakNet::BitStream bsSpawnRequestReply;
	CPlayer *pPlayer=pNetGame->GetPlayerPool()->GetAt(bytePlayerID);
	PLAYER_SPAWN_INFO *SpawnInfo = pPlayer->GetSpawnInfo();

	bsSpawnRequestReply.Write(byteRequestOutcome);
	bsSpawnRequestReply.Write(SpawnInfo->byteTeam);
	bsSpawnRequestReply.Write(SpawnInfo->byteSkin);
	bsSpawnRequestReply.Write((float)SpawnInfo->vecPos.X);
	bsSpawnRequestReply.Write((float)SpawnInfo->vecPos.Y);
	bsSpawnRequestReply.Write((float)SpawnInfo->vecPos.Z);
	bsSpawnRequestReply.Write((float)SpawnInfo->fRotation);
	bsSpawnRequestReply.Write(SpawnInfo->iSpawnWeapons[0]);
	bsSpawnRequestReply.Write(SpawnInfo->iSpawnWeaponsAmmo[0]);
	bsSpawnRequestReply.Write(SpawnInfo->iSpawnWeapons[1]);
	bsSpawnRequestReply.Write(SpawnInfo->iSpawnWeaponsAmmo[1]);
	bsSpawnRequestReply.Write(SpawnInfo->iSpawnWeapons[2]);
	bsSpawnRequestReply.Write(SpawnInfo->iSpawnWeaponsAmmo[2]);

	pRak->RPC("RequestClass",&bsSpawnRequestReply,HIGH_PRIORITY,RELIABLE,0,sender,FALSE,FALSE);
}


//----------------------------------------------------
// Sent by client when they're spawning/respawning.

void Spawn(PCHAR Data, int iBitLength, PlayerID sender)
{
	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);
	if(!pNetGame->GetPlayerPool()->GetSlotState(bytePlayerID)) return;
	CPlayer	*pPlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);
	pPlayer->Spawn();
}

//----------------------------------------------------
// Sent by client when they die.

void Death(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);
	if(!pNetGame->GetPlayerPool()->GetSlotState(bytePlayerID)) return;
	CPlayer	*pPlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);

	BYTE byteDeathReason;
	BYTE byteWhoWasResponsible;

	bsData.Read(byteDeathReason);
	bsData.Read(byteWhoWasResponsible);

	if(pPlayer) {
		pPlayer->HandleDeath(byteDeathReason,byteWhoWasResponsible);
	}
}

//----------------------------------------------------
// Sent by client when they want to enter a
// vehicle gracefully.

void EnterVehicle(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);
	if(!pNetGame->GetPlayerPool()->GetSlotState(bytePlayerID)) return;
	CPlayer	*pPlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);
	BYTE byteVehicleID;
	BYTE bytePassenger;

	bsData.Read(byteVehicleID);
	bsData.Read(bytePassenger);
	pPlayer->EnterVehicle(byteVehicleID,bytePassenger);

	//logprintf("%u enters vehicle %u",bytePlayerID,byteVehicleID);
}

//----------------------------------------------------
// Sent by client when they want to exit a
// vehicle gracefully.

void ExitVehicle(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);
	if(!pNetGame->GetPlayerPool()->GetSlotState(bytePlayerID)) return;
	CPlayer	*pPlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);
	BYTE byteVehicleID;

	bsData.Read(byteVehicleID);
	pPlayer->ExitVehicle(byteVehicleID);

	//logprintf("%u exits vehicle %u",bytePlayerID,byteVehicleID);
}

//----------------------------------------------------
// Sent by client when they want score and ping information.

void UpdateScoreAndPing(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	RakNet::BitStream bsSend;
	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);
	if(!pNetGame->GetPlayerPool()->GetSlotState(bytePlayerID)) return;

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	int write_counter=0;
	PlayerID pID;

	BYTE x=0;
	while(x!=MAX_PLAYERS) {
		if(pPlayerPool->GetSlotState(x) == TRUE) {

			pID = pRak->GetPlayerIDFromIndex(x);

			bsSend.Write((BYTE)x);
			bsSend.Write((int)pPlayerPool->GetScore(x));
			bsSend.Write((int)pRak->GetLastPing(pRak->GetPlayerIDFromIndex(x)));
			
			if(pPlayerPool->IsAdmin(bytePlayerID)) {
				bsSend.Write(pID.binaryAddress);
			} else {
				bsSend.Write((unsigned long)0UL);
			}

			write_counter++;
		}
		x++;
	}
		
	pRak->RPC("UpdateScPing",&bsSend,HIGH_PRIORITY,RELIABLE,0,sender,FALSE,FALSE);
}

//----------------------------------------------------

void Admin(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);
	if(!pNetGame->GetPlayerPool()->GetSlotState(bytePlayerID)) return;

	char szPassTest[65];
	int iPassLen;

	bsData.Read(iPassLen);
	if(iPassLen > 64) return;
	bsData.Read(szPassTest,iPassLen);

	szPassTest[iPassLen] = '\0';
	
	if(!strcmp(szPassTest,szAdminPass)) {
		pPlayerPool->SetAdmin(bytePlayerID);
	}
}

//----------------------------------------------------

void KickPlayer(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);
	if(!pNetGame->GetPlayerPool()->GetSlotState(bytePlayerID)) return;

	BYTE byteKickPlayer;

	if(pPlayerPool->IsAdmin(bytePlayerID)) {
		bsData.Read(byteKickPlayer);	
		pNetGame->KickPlayer(byteKickPlayer);
	}
}

//----------------------------------------------------

void BanIPAddress(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);
	if(!pNetGame->GetPlayerPool()->GetSlotState(bytePlayerID)) return;

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	char ban_ip[256];
	int iBanIpLen;

	if(pPlayerPool->IsAdmin(bytePlayerID)) {
		bsData.Read(iBanIpLen);
		bsData.Read(ban_ip,iBanIpLen);
		ban_ip[iBanIpLen] = 0;

		pNetGame->AddBan(ban_ip);
	}
}
//----------------------------------------------------

void RegisterRPCs(RakServerInterface * pRakServer)
{
	pRak = pRakServer;

	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, ClientJoin);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Chat);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, RequestClass);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Spawn);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Death);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, EnterVehicle);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, ExitVehicle);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, UpdateScoreAndPing);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Admin);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, KickPlayer);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, BanIPAddress);
}

//----------------------------------------------------

void UnRegisterRPCs(RakServerInterface * pRakServer)
{
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, ClientJoin);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Chat);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, RequestClass);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Spawn);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Death);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, EnterVehicle);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, ExitVehicle);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, UpdateScoreAndPing);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Admin);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, KickPlayer);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, BanIPAddress);

	pRak = 0;
}

//----------------------------------------------------
