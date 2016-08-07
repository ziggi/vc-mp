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

#define GAMESTATE_NONE			0
#define GAMESTATE_CONNECTING	1
#define GAMESTATE_CONNECTED		2
#define GAMESTATE_AWAIT_JOIN	3
#define GAMESTATE_DISCONNECTED  4

#define PACK_VEHICLE_HEALTH(f)		(BYTE)(f / 4)
#define UNPACK_VEHICLE_HEALTH(b)	(float)b * 4

typedef struct _C_VECTOR1 {
	short X,Y,Z;
} C_VECTOR1;

//----------------------------------------------------

void CompressVector1(VECTOR * vec, C_VECTOR1 * c1);
void DecompressVector1(VECTOR * vec, C_VECTOR1 * c1);

//----------------------------------------------------

class CNetGame
{
private:

	CPlayerPool			*m_pPlayerPool;
	CVehiclePool		*m_pVehiclePool;
	RakClientInterface	*m_pRakClient;
	CGameModeGeneric	*m_pGameLogic;

	int					m_iGameState;
	DWORD				m_dwLastScoreUpdateTick;
	UINT				m_uiLastRandSeed;


	void UpdateNetwork();

	void PlayerSync(Packet *p);
	void VehicleSync(Packet *p);
	void ConnectionSucceeded(Packet *p);

public:

	VECTOR m_vecInitPlayerPos;
	VECTOR m_vecInitCameraPos;
	VECTOR m_vecInitCameraLook;
	float	 m_WorldBounds[4]; // pos_x neg_x pos_y neg_y
	int		 m_iSpawnsAvailable;
	BYTE	 m_byteFriendlyFire;
	BYTE	 m_byteShowOnRadar;

	CNetGame(PCHAR szHostOrIp,int iPort,PCHAR szPlayerName,PCHAR szPass);
	~CNetGame();

	int GetGameState() { return m_iGameState; };
	void SetGameState(int iGameState) { m_iGameState = iGameState; };

	CPlayerPool * GetPlayerPool() { return m_pPlayerPool; };
	CVehiclePool * GetVehiclePool() { return m_pVehiclePool; };
	RakClientInterface * GetRakClient() { return m_pRakClient; };
	CGameModeGeneric * GetGameLogic() { return m_pGameLogic; };
	void InitGameLogic();
	void Process();	
	void UpdatePlayerScoresAndPings();
};

//----------------------------------------------------
