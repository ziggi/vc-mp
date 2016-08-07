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

extern CGame *pGame;

//----------------------------------------------------

CChatWindow::CChatWindow(IDirect3DDevice8 *pD3DDevice)
{
	int x=0;

	m_bEnabled			= TRUE;
	m_fDrawStartX		= 25.0f;
	m_fDrawStartY		= 30.0f;
	m_fDrawPadding		= 3.0f;
	m_pD3DFont			= NULL;

	if(pD3DDevice) {
		m_pD3DFont = new CD3DFont(CHAT_FONT,CHAT_FONT_SIZE+1,0);
		m_pD3DFont->InitDeviceObjects(pD3DDevice);
		m_pD3DFont->RestoreDeviceObjects();
	}

  	m_pD3DFont->GetTextExtent("W",&m_sizeText);

	// Init the chat window lines to 0
	while(x!=MAX_DISP_MESSAGES) {
		memset(m_szChatWindowBuffer[x],0,MAX_MESSAGE_LENGTH);
		x++;
	}

	m_dwChatTextColor = D3DCOLOR_ARGB(255,255,255,255);
	m_dwChatInfoColor = D3DCOLOR_ARGB(255,204,122,184);
	m_dwChatDebugColor = D3DCOLOR_ARGB(255,4,163,107);
	m_dwChatBackgroundColor = D3DCOLOR_ARGB(255,0,0,0);

	m_pD3DDevice = pD3DDevice;
}

//----------------------------------------------------

CChatWindow::~CChatWindow()
{
	if(m_pD3DFont) delete m_pD3DFont;
}

//----------------------------------------------------

void CChatWindow::Draw()
{
	float fDrawX=m_fDrawStartX;
	float fDrawY=m_fDrawStartY; 
	DWORD dwColorChat=0;

	int x=0;

	if(m_pD3DFont && m_bEnabled)
	{
		while(x!=(MAX_DISP_MESSAGES)) {

			switch(m_eChatType[x]) {
				case CHAT_TYPE_CHAT:
					dwColorChat = m_dwChatTextColor;
					break;
				case CHAT_TYPE_INFO:
					dwColorChat = m_dwChatInfoColor;
					break;
				case CHAT_TYPE_DEBUG:
					dwColorChat = m_dwChatDebugColor;
					break;
			}		

			// Draws a drop shadow on the text. Too CPU intense?
			m_pD3DFont->DrawText(fDrawX+1.0f,fDrawY+1.0f,m_dwChatBackgroundColor,
				m_szChatWindowBuffer[x],0L,TRUE);

			m_pD3DFont->DrawText(fDrawX,fDrawY,dwColorChat,
				m_szChatWindowBuffer[x]);

			fDrawY+=m_sizeText.cy+m_fDrawPadding;
		
			x++;
		}
	} else {
		if((NULL == m_pD3DFont) && m_pD3DDevice) { // Create/recreate it.
			m_pD3DFont = new CD3DFont(CHAT_FONT,CHAT_FONT_SIZE,0);
			m_pD3DFont->InitDeviceObjects(m_pD3DDevice);
			m_pD3DFont->RestoreDeviceObjects();
		}
	}
}

//----------------------------------------------------

void CChatWindow::AddChatMessage(CHAR *szNick, DWORD dwNickColor, CHAR *szMessage)
{
	char tmp_buf[512] = {0};
	char * t = tmp_buf;

	memset(tmp_buf,0,512);

	// NICKNAME
	*t = '\01'; t++; // color start
	memcpy(t,&dwNickColor,sizeof(DWORD)); t+=4; // nick color
	memcpy(t,szNick,strlen(szNick)); t+=strlen(szNick); // nick string
	*t = '\01'; t++; // color start
	memcpy(t,&m_dwChatTextColor,sizeof(DWORD)); t+=4; // chat color
	*t = ':'; t++;
	*t = ' '; t++;

	// MESSAGE
	FilterInvalidChars(szMessage);

	strcpy(t,szMessage);

	AddToChatWindowBuffer(tmp_buf,CHAT_TYPE_CHAT);
}

//----------------------------------------------------

void CChatWindow::AddInfoMessage(CHAR * szFormat, ...)
{
	char tmp_buf[512];
	memset(tmp_buf,0,512);

	va_list args;
	va_start(args, szFormat);
	vsprintf(tmp_buf, szFormat, args);
	va_end(args);

	FilterInvalidChars(tmp_buf);
	AddToChatWindowBuffer(tmp_buf,CHAT_TYPE_INFO);	
}

//----------------------------------------------------

void CChatWindow::AddDebugMessage(CHAR * szFormat, ...)
{
	char tmp_buf[512];
	memset(tmp_buf,0,512);

	va_list args;
	va_start(args, szFormat);
	vsprintf(tmp_buf, szFormat, args);
	va_end(args);

	FilterInvalidChars(tmp_buf);
	AddToChatWindowBuffer(tmp_buf,CHAT_TYPE_DEBUG);	
}

//----------------------------------------------------

void CChatWindow::FilterInvalidChars(PCHAR szString)
{
	while(*szString) {
		if(*szString < ' ' || *szString > 'z') {
			*szString = ' ';
		}
		szString++;
	}
}

//----------------------------------------------------

void CChatWindow::AddToChatWindowBuffer(PCHAR szString, eChatMessageType eType)
{
	int iBestLineLength;
	
	PushBack();

	if(strlen(szString) > MAX_LINE_LENGTH)
	{
		iBestLineLength = MAX_LINE_LENGTH;
		// see if we can locate a space.
		while(szString[iBestLineLength] != ' ' && iBestLineLength)
			iBestLineLength--;

		if((MAX_LINE_LENGTH - iBestLineLength) > 8) {
			// we should just take the whole line
			strncpy(m_szChatWindowBuffer[MAX_DISP_MESSAGES-1],szString,MAX_LINE_LENGTH);
			m_szChatWindowBuffer[MAX_DISP_MESSAGES-1][MAX_LINE_LENGTH] = '\0';
			m_eChatType[MAX_DISP_MESSAGES-1] = eType;
			PushBack();
			strcpy(m_szChatWindowBuffer[MAX_DISP_MESSAGES-1],szString+MAX_LINE_LENGTH);
			m_eChatType[MAX_DISP_MESSAGES-1] = eType;
		}
		else {
			// grab upto the found space.
			strncpy(m_szChatWindowBuffer[MAX_DISP_MESSAGES-1],szString,iBestLineLength);
			m_szChatWindowBuffer[MAX_DISP_MESSAGES-1][iBestLineLength] = '\0';
			m_eChatType[MAX_DISP_MESSAGES-1] = eType;
			PushBack();
			strcpy(m_szChatWindowBuffer[MAX_DISP_MESSAGES-1],szString+(iBestLineLength+1));
			m_eChatType[MAX_DISP_MESSAGES-1] = eType;
		}
	}
	else {
		strcpy(m_szChatWindowBuffer[MAX_DISP_MESSAGES-1],szString);
		m_eChatType[MAX_DISP_MESSAGES-1] = eType;
	}
	
}

//----------------------------------------------------

void CChatWindow::PushBack()
{
	int x=0;
	while(x!=(MAX_DISP_MESSAGES - 1)) {
		strcpy(m_szChatWindowBuffer[x],m_szChatWindowBuffer[x + 1]);
		m_eChatType[x] = m_eChatType[x + 1];
		x++;
	}
}

//----------------------------------------------------

void CChatWindow::DeleteDeviceObjects()
{
	SAFE_DELETE(m_pD3DFont);
}

//----------------------------------------------------
// EOF