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

#include "main.h"

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;

//----------------------------------------------------

CCmdWindow::CCmdWindow(IDirect3DDevice8 *pD3DDevice)
{
	SIZE size;

	m_bEnabled			= FALSE;
	m_pD3DFont			= NULL;
	m_iCmdCount			= 0;

	if(pD3DDevice) {
		m_pD3DFont = new CD3DFont(CHAT_FONT,CHAT_FONT_SIZE+4,0);
		m_pD3DFont->InitDeviceObjects(pD3DDevice);
		m_pD3DFont->RestoreDeviceObjects();
	}

	m_pD3DFont->GetTextExtent("W",&size);

	m_fDrawStartX		= 25.0f;
	m_fDrawPadding		= 3.0f;
	m_fDrawStartY		= 30.0f + ((size.cy+m_fDrawPadding) * MAX_DISP_MESSAGES);

	m_szInputBuffer[0] = '\0';
	m_dwDrawColor = D3DCOLOR_ARGB(255,255,255,255);
	m_dwBackgroundColor = D3DCOLOR_ARGB(255,0,0,0);

	m_pD3DDevice = pD3DDevice;
}

//----------------------------------------------------

CCmdWindow::~CCmdWindow()
{
	if(m_pD3DFont) delete m_pD3DFont;
}

//----------------------------------------------------

void CCmdWindow::Enable()
{
	m_bEnabled = TRUE;
	pGame->ToggleKeyInputsDisabled(TRUE);
}

//----------------------------------------------------

void CCmdWindow::Disable()
{
	m_bEnabled = FALSE;
	pGame->ToggleKeyInputsDisabled(FALSE);
}

//----------------------------------------------------
void CCmdWindow::Draw()
{
	char tmp_buf[MAX_CMD_INPUT+3];

	if(m_pD3DFont && m_bEnabled) {
		sprintf(tmp_buf,"> %s_",m_szInputBuffer);
		m_pD3DFont->DrawText(m_fDrawStartX+1.0f,m_fDrawStartY+1.0f,m_dwBackgroundColor,
			tmp_buf);
		m_pD3DFont->DrawText(m_fDrawStartX,m_fDrawStartY,m_dwDrawColor,
			tmp_buf);
	} else {
		if((NULL == m_pD3DFont) && m_pD3DDevice) { // Create/recreate it.
			m_pD3DFont = new CD3DFont(CHAT_FONT,CHAT_FONT_SIZE+4,0);
			m_pD3DFont->InitDeviceObjects(m_pD3DDevice);
			m_pD3DFont->RestoreDeviceObjects();
		}
	}
}

//----------------------------------------------------

void CCmdWindow::AddChar(char Char)
{
	if(FALSE == m_bEnabled) return;

	if(Char < ' ') return;

	int iCurBufferLen = strlen(m_szInputBuffer);
	if(iCurBufferLen < MAX_CMD_INPUT) { 
		m_szInputBuffer[iCurBufferLen] = Char;
		m_szInputBuffer[iCurBufferLen+1] = '\0';
	}
}

//----------------------------------------------------

void CCmdWindow::BackSpace()
{
	if(FALSE == m_bEnabled) return;
	int iCurBufferLen = strlen(m_szInputBuffer);
	m_szInputBuffer[iCurBufferLen - 1] = '\0';
}		

//----------------------------------------------------

void CCmdWindow::ProcessInput()
{
	PCHAR szCmdEndPos;
	CMDPROC cmdHandler;

	if(m_szInputBuffer[0] != '\0')
	{
		if(m_szInputBuffer[0] != CMD_CHARACTER) { 
			// chat type message	
			if(m_pDefaultCmd) {
				m_pDefaultCmd(m_szInputBuffer);
			}
		}
		else 
		{// possible valid command
			// find the end of the name
			szCmdEndPos = &m_szInputBuffer[1];
			while(*szCmdEndPos && *szCmdEndPos != ' ') szCmdEndPos++;
			if(*szCmdEndPos == '\0') {
				// Possible command with no params.
				cmdHandler = GetCmdHandler(&m_szInputBuffer[1]);
				// If valid then call it.
				if(cmdHandler) {
					cmdHandler("");
				}
				else {
					pChatWindow->AddDebugMessage("I don't know that command.");
				}
			}
			else {
				*szCmdEndPos='\0'; // null terminate it
				szCmdEndPos++; // rest is the parameters.
				cmdHandler = GetCmdHandler(&m_szInputBuffer[1]);
				// If valid then call it with the param string.
				if(cmdHandler) {
					cmdHandler(szCmdEndPos);
				}
				else {
					pChatWindow->AddDebugMessage("I don't know that command.");
				}
			}
		}
	}
	m_szInputBuffer[0]='\0';
	
	if(m_bEnabled) Disable();
}

//----------------------------------------------------

CMDPROC CCmdWindow::GetCmdHandler(PCHAR szCmdName)
{
	int x=0;
	while(x!=m_iCmdCount) {
		if(!stricmp(szCmdName,m_szCmdNames[x])) {
			return m_pCmds[x];
		}
		x++;
	}
	return NULL;
}

//----------------------------------------------------

void CCmdWindow::AddDefaultCmdProc(CMDPROC cmdDefault)
{
	m_pDefaultCmd = cmdDefault;	
}

//----------------------------------------------------

void CCmdWindow::AddCmdProc(PCHAR szCmdName, CMDPROC cmdHandler)
{
	if(m_iCmdCount < MAX_CMDS && (strlen(szCmdName) < MAX_CMD_STRLEN)) {
		m_pCmds[m_iCmdCount] = cmdHandler;
		strcpy(m_szCmdNames[m_iCmdCount],szCmdName);
		m_iCmdCount++;
	}
}

//----------------------------------------------------

void CCmdWindow::DeleteDeviceObjects()
{
	SAFE_DELETE(m_pD3DFont);
}

//----------------------------------------------------
// EOF
