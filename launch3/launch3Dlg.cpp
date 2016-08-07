// launch3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "launch3.h"
#include "launch3Dlg.h"

#include "htmlhelp.h"
#pragma comment(lib,"htmlhelp.lib ")

#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLaunch3Dlg dialog

CLaunch3Dlg::CLaunch3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLaunch3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLaunch3Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLaunch3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLaunch3Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLaunch3Dlg, CDialog)
	//{{AFX_MSG_MAP(CLaunch3Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_NICK, OnChangeNick)
	ON_BN_CLICKED(IDC_LAUNCH, OnLaunch)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_HELP_BUTTON, OnHelpButton)
	ON_BN_CLICKED(IDC_START_SERVER, OnStartServer)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CString GetAppPath ()
{
TCHAR app_path[_MAX_PATH];

GetModuleFileName((HMODULE)AfxGetApp()->m_hInstance, app_path, MAX_PATH);
CString app_str = app_path;
app_str = app_str.Left(app_str.ReverseFind('\\')+1);

return app_str;
}

/////////////////////////////////////////////////////////////////////////////
// CLaunch3Dlg message handlers

BOOL CLaunch3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	HKEY hKey=0;
	char szBuf[256];

	RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\VCMP",0,KEY_READ,&hKey);

	if(hKey) {

		DWORD dwSize=256;
		DWORD dwType=REG_SZ;

		RegQueryValueEx(hKey,"nick",0,&dwType,(PUCHAR)szBuf,&dwSize);
		if(dwSize) {
			SetDlgItemText(IDC_NICK,szBuf);
		}

		dwSize=256;
		dwType=REG_SZ;

		RegQueryValueEx(hKey,"server",0,&dwType,(PUCHAR)szBuf,&dwSize);
		if(dwSize) {
			SetDlgItemText(IDC_SERVER_IP,szBuf);
		}
		
		dwSize=256;
		dwType=REG_SZ;

		RegQueryValueEx(hKey,"port",0,&dwType,(PUCHAR)szBuf,&dwSize);
		if(dwSize) {
			SetDlgItemText(IDC_PORT,szBuf);
		}

		RegCloseKey(hKey);
	}
	
	GetDlgItemText(IDC_NICK,szBuf,256);
	if(strlen(szBuf) == 0) {
		SetDlgItemText(IDC_NICK,"Player");
	}

	GetDlgItemText(IDC_SERVER_IP,szBuf,256);
	if(strlen(szBuf) == 0) {
		SetDlgItemText(IDC_SERVER_IP,"127.0.0.1");
	}

	GetDlgItemText(IDC_PORT,szBuf,256);
	if(strlen(szBuf) == 0) {
		SetDlgItemText(IDC_PORT,"5192");
	}

	SetDlgItemText(IDC_SERVER_CONFIG,"vcmp\\vanilla.ini");

	if(strlen((char *)GetCommandLine())) {
		CString csCmdLine((char *)GetCommandLine());
		int vcmp_pos = csCmdLine.Find("vcmp://");

		if(vcmp_pos != (-1)) {
			csCmdLine = csCmdLine.Right(csCmdLine.GetLength() - (vcmp_pos+7));
			
			if(csCmdLine.Right(1) == "/") {
				csCmdLine = csCmdLine.Left(csCmdLine.GetLength() - 1);
			}

			int colon_pos = csCmdLine.Find(':');
			
			CString csHost;
			CString csPort;

			if(colon_pos != (-1)) {
				csHost = csCmdLine.Left(colon_pos);
				csPort = csCmdLine.Right(csCmdLine.GetLength() - (colon_pos+1));
				SetDlgItemText(IDC_SERVER_IP,csHost);
				SetDlgItemText(IDC_PORT,csPort);
			}
		}
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLaunch3Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CLaunch3Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLaunch3Dlg::OnChangeNick() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CLaunch3Dlg::OnLaunch() 
{
	char szParams[1024];
	char szNick[32];
	char szServerIP[256];
	char szPort[32];
	char szPassword[32];

	GetDlgItemText(IDC_NICK,szNick,256);
	GetDlgItemText(IDC_SERVER_IP,szServerIP,256);
	GetDlgItemText(IDC_PORT,szPort,256);

	HKEY hKey=0;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\VCMP",0,KEY_WRITE,&hKey);

	if(!hKey) {
		RegCreateKey(HKEY_LOCAL_MACHINE,"Software\\VCMP",&hKey);
	}
	
	if(hKey) {
		char szNick[256];
		char szServerIP[256];
		char szPort[256];

		DWORD dwSize=256;
		DWORD dwType=REG_SZ;

		GetDlgItemText(IDC_NICK,szNick,32);
		RegSetValueEx(hKey,"nick",0,dwType,(PUCHAR)szNick,dwSize);

		dwSize=256;
		dwType=REG_SZ;

		GetDlgItemText(IDC_SERVER_IP,szServerIP,256);
		RegSetValueEx(hKey,"server",0,dwType,(PUCHAR)szServerIP,dwSize);
			
		dwSize=256;
		dwType=REG_SZ;

		GetDlgItemText(IDC_PORT,szPort,256);
		RegSetValueEx(hKey,"port",0,dwType,(PUCHAR)szPort,dwSize);

		RegCloseKey(hKey);
	}

	sprintf(szParams,"-c -h %s -p %s -n %s",szServerIP,szPort,szNick);
	
	GetDlgItemText(IDC_SERVER_PASSWORD,szPassword,32);

	if(strlen(szPassword) > 0) {
		strcat(szParams," -z ");
		strcat(szParams,szPassword);
	}

	char szGtaExe[1024];
	sprintf(szGtaExe,"%sgta-vc.exe",GetAppPath());

	int x = (int)::ShellExecute(0,"open",szGtaExe,szParams,GetAppPath(),NULL);

	if(x==ERROR_FILE_NOT_FOUND || x==ERROR_PATH_NOT_FOUND) {
		MessageBox("Couldn't launch gta-vc.exe.\nDid you install vc-mp to your Vice City directory?");
	}

}

void CLaunch3Dlg::OnButton2() 
{
	this->OnCancel();	
}

void CLaunch3Dlg::OnHelpButton() 
{
	//ShellExecute(m_hWnd, "open","vcmp\\vc-mp.chm", NULL, NULL, SW_SHOWNORMAL);	
	HtmlHelp(m_hWnd,"vcmp\\vc-mp.chm",HH_DISPLAY_TOPIC,0);
}

void CLaunch3Dlg::OnStartServer()
{
	char szServerConfig[256];
	GetDlgItemText(IDC_SERVER_CONFIG,szServerConfig,256);
	ShellExecute(m_hWnd, "open","vcmp-svr.exe", szServerConfig, NULL, SW_SHOWNORMAL);
}

void CLaunch3Dlg::OnButton3() 
{
	char szRConHost[256];
	char szRConPort[256];
	char szRConPass[256];
	char szCated[1024];

	GetDlgItemText(IDC_SERVER_IP_ADMIN,szRConHost,256);
	GetDlgItemText(IDC_SERVER_PORT_ADMIN,szRConPort,256);
	GetDlgItemText(IDC_SERVER_PASS_ADMIN,szRConPass,256);

	sprintf(szCated,"%s %s %s",szRConHost,szRConPort,szRConPass);
	ShellExecute(m_hWnd, "open","rcon.exe", szCated, NULL, SW_SHOWNORMAL);
}
