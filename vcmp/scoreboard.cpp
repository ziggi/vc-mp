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
// File Author: kyeman (in a rush)
//
//----------------------------------------------------

#include "main.h"
#include <stdio.h>

extern CNetGame* pNetGame;
extern GAME_SETTINGS tSettings;
extern IDirect3DDevice8 *pD3DDevice;
D3DDISPLAYMODE DisplayMode;
extern CChatWindow * pChatWindow;


void CScoreBoard::Draw()
{
	CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
	pNetGame->UpdatePlayerScoresAndPings();	

	char szScoreBuf[8192] = {0};
	char szScoreWrite[1024] = {0};
	char szColorCodedNick[256] = {0};
	DWORD dwNickColor;
	DWORD dwChatColor = 0xFFFFFFFF;
	char * t;
	char * szNick;

	sprintf(szScoreWrite,"VC-MP dev-shot release 0.1 - %s:%s\n\n",
		    tSettings.szConnectHost,tSettings.szConnectPort);
	strcat(szScoreBuf,szScoreWrite);

	sprintf(szScoreWrite,"Player\t\t\tScore\t\tPing\n\n");
	strcat(szScoreBuf,szScoreWrite);

	// write local player nick with color
	t = szColorCodedNick;
	
	dwNickColor = pPlayerPool->GetLocalPlayer()->GetTeamColorAsARGB();
	szNick = pPlayerPool->GetLocalPlayerName();

	*t = '\01'; t++; // color start
	memcpy(t,&dwNickColor,sizeof(DWORD)); t+=4; // nick color
	memcpy(t,szNick,strlen(szNick)); t+=strlen(szNick); // nick string
	*t = '\01'; t++; // color start
	memcpy(t,&dwChatColor,sizeof(DWORD)); t+=4; // chat color
	*t = '\0';
	
	sprintf(szScoreWrite,"%s\t\t\t%d\t\t%d\n",
		szColorCodedNick,pPlayerPool->GetLocalPlayerScore(),
		pPlayerPool->GetLocalPlayerPing());
	strcat(szScoreBuf,szScoreWrite);

	int x=0;

	while(x!=MAX_PLAYERS) {
		if(pPlayerPool->GetSlotState(x) == TRUE) {

				// write local player nick with color
				t = szColorCodedNick;
	
				dwNickColor = pPlayerPool->GetAt(x)->GetTeamColorAsARGB();
				szNick = pPlayerPool->GetPlayerName(x);

				*t = '\01'; t++; // color start
				memcpy(t,&dwNickColor,sizeof(DWORD)); t+=4; // nick color
				memcpy(t,szNick,strlen(szNick)); t+=strlen(szNick); // nick string
				*t = '\01'; t++; // color start
				memcpy(t,&dwChatColor,sizeof(DWORD)); t+=4; // chat color
				*t = '\0';

				sprintf(szScoreWrite,"%u: %s\t\t\t%d\t\t%d\n",
						x,szColorCodedNick,pPlayerPool->GetScore(x),
						pPlayerPool->GetPing(x));

				strcat(szScoreBuf,szScoreWrite);			
		}
		x++;
	}

	pD3DDevice->GetDisplayMode(&DisplayMode);
	pChatWindow->m_pD3DFont->DrawTextBox((float)((DisplayMode.Width / 2) - 100),50.0f,50.0f,0x50000000,0xFFFFFFFF,szScoreBuf,0L);
}
