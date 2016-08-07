//----------------------------------------------------
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
//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author: kyeman
//
//----------------------------------------------------

//#pragma twice <- that's a joke btw.

#pragma once

#define MAX_CMD_INPUT   64
#define MAX_CMDS		128
#define MAX_CMD_STRLEN  32
#define CMD_CHARACTER   '/'

typedef void (__cdecl *CMDPROC)(PCHAR);

//----------------------------------------------------

class CCmdWindow
{
private:

	CHAR		m_szInputBuffer[MAX_CMD_INPUT+1];
	BOOL		m_bEnabled;
	float		m_fDrawStartX;
	float		m_fDrawStartY;
	float		m_fDrawPadding;
	DWORD		m_dwDrawColor;
	DWORD		m_dwBackgroundColor;
	CD3DFont	*m_pD3DFont;
	IDirect3DDevice8 *m_pD3DDevice;

	// Command procedure stuff.
	CMDPROC		m_pDefaultCmd;	 // used when no command specifier was
								 // used (ie. a normal chat message)
	CMDPROC		m_pCmds[MAX_CMDS];
	CHAR        m_szCmdNames[MAX_CMDS][MAX_CMD_STRLEN+1];
	int			m_iCmdCount;

public:

	void Draw();

	void Enable();
	void Disable();

	BOOL isEnabled() { return m_bEnabled; };
	
	void DeleteDeviceObjects();

	void AddChar(char Char);
	void BackSpace();
	void ProcessInput();

	CMDPROC GetCmdHandler(PCHAR szCmdName);
	void AddDefaultCmdProc(CMDPROC cmdDefault);
	void AddCmdProc(PCHAR szCmdName, CMDPROC cmdHandler);

	CCmdWindow(IDirect3DDevice8 *pD3DDevice);
	~CCmdWindow();
};

//----------------------------------------------------
// EOF