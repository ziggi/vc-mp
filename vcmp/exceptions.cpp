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
// Portions taken from the GTA Exception Handler
// by kyeman and eAi. (Public Domain)
//
//----------------------------------------------------

#include "main.h"
#include "resource.h"

PCONTEXT pContextRecord;
extern	 HANDLE  hInstance;
extern   CGame * pGame;
extern   BOOL bDisableGTAWndProc;
//----------------------------------------------------

BOOL CALLBACK GuiDlgProcMain(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	
	CHAR szErrorString[1024];
	CHAR s[256];
	DWORD *pdwStack;
	int x=0;
		
	SetCursor(0);
	ShowCursor(TRUE);

	switch(uMsg)
	{
		case WM_MOUSEMOVE:
			ShowCursor(TRUE);
			break;

		case WM_INITDIALOG:		

			sprintf(szErrorString,
				"Exception At Address: 0x%08X\r\n\r\n"
				"Registers:\r\n"
				"EAX: 0x%08X\tEBX: 0x%08X\tECX: 0x%08X\tEDX: 0x%08X\r\n"
				"ESI: 0x%08X\tEDI: 0x%08X\tEBP: 0x%08X\tESP: 0x%08X\r\n"
				"EFLAGS: 0x%08X\r\n\r\nStack:\r\n",
				pContextRecord->Eip,
				pContextRecord->Eax,
				pContextRecord->Ebx,
				pContextRecord->Ecx,
				pContextRecord->Edx,
				pContextRecord->Esi,
				pContextRecord->Edi,
				pContextRecord->Ebp,
				pContextRecord->Esp,
				pContextRecord->EFlags);

			// grab the last 25 stack values
			pdwStack = (DWORD *)pContextRecord->Esp;
			while(x!=25) {
				sprintf(s,"0x%08X\r\n",pdwStack[x]);
				strcat(szErrorString,s);
				x++;
			}

			SetDlgItemText(hDlg,IDC_EDIT1,szErrorString);
			SetForegroundWindow(GetDlgItem(hDlg,IDD_DIALOG1));
			SetFocus(GetDlgItem(hDlg,IDC_BUTTON1));
			break;

		case WM_DESTROY:
			EndDialog(hDlg,TRUE);
			//ExitProcess(1);
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON1:
				EndDialog(hDlg,TRUE);
				//ExitProcess(1);
			}
			break;
	}

	return FALSE;
}

//----------------------------------------------------

LONG WINAPI exc_handler(_EXCEPTION_POINTERS* exc_inf)
{
	pContextRecord = exc_inf->ContextRecord;
	
	ShowWindow(pGame->GetMainWindowHwnd(),SW_HIDE);
	DialogBox((HINSTANCE)hInstance,MAKEINTRESOURCE(IDD_DIALOG1),pGame->GetMainWindowHwnd(),(DLGPROC)GuiDlgProcMain);

	return EXCEPTION_CONTINUE_SEARCH;
}

//----------------------------------------------------