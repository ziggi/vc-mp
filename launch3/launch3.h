// launch3.h : main header file for the LAUNCH3 application
//

#if !defined(AFX_LAUNCH3_H__C979018B_7D67_4515_B74B_E15907F5B617__INCLUDED_)
#define AFX_LAUNCH3_H__C979018B_7D67_4515_B74B_E15907F5B617__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLaunch3App:
// See launch3.cpp for the implementation of this class
//

class CLaunch3App : public CWinApp
{
public:
	CLaunch3App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaunch3App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLaunch3App)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUNCH3_H__C979018B_7D67_4515_B74B_E15907F5B617__INCLUDED_)
