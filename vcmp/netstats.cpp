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
extern CChatWindow *pChatWindow;

CNetStats::CNetStats()
{
	m_dwLastUpdateTick = GetTickCount();
	m_dwLastTotalBytesRecv = 0;
	m_dwLastTotalBytesSent = 0;
	m_dwBPSDownload = 0;
	m_dwBPSUpload = 0;
}

void CNetStats::Draw()
{
	char szDispBuf[1024];
	char szStatBuf[1024];
	D3DDISPLAYMODE dDisplayMode;
	RakNetStatisticsStruct *pRakStats = pNetGame->GetRakClient()->GetStatistics();
	float fDown,fUp;

	if((GetTickCount() - m_dwLastUpdateTick) > 1000) {
		m_dwLastUpdateTick = GetTickCount();
		
		m_dwBPSDownload = ((UINT)(pRakStats->bitsReceived / 8)) - m_dwLastTotalBytesRecv;
		m_dwLastTotalBytesRecv = (UINT)(pRakStats->bitsReceived / 8);

		m_dwBPSUpload = ((UINT)(pRakStats->totalBitsSent / 8)) - m_dwLastTotalBytesSent;
		m_dwLastTotalBytesSent = (UINT)(pRakStats->totalBitsSent / 8);
	}

	if(m_dwBPSDownload != 0) {
		fDown = (float)m_dwBPSDownload / 1024;
	} else {
		fDown = 0.0f;
	}

	if(m_dwBPSUpload != 0) {
		fUp = (float)m_dwBPSUpload / 1024;
	} else {
		fUp = 0.0f;
	}

	sprintf(szDispBuf,"--- Network Stats ---\n\nDownload Rate: %.2f kbps\nUpload Rate: %.2f kbps\n\n",fDown,fUp);
	
	StatisticsToString(pRakStats,szStatBuf,1);

	strcat(szDispBuf,szStatBuf);

	pD3DDevice->GetDisplayMode(&dDisplayMode);
	pChatWindow->m_pD3DFont->DrawTextBox((float)((dDisplayMode.Width / 2) - 100),50.0f,50.0f,0x50000000,0xFFFFFFFF,szDispBuf,0L);
}
