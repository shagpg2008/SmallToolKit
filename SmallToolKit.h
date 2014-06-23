// SmallToolKit.h : main header file for the SMALLTOOLKIT application
//

#if !defined(AFX_SMALLTOOLKIT_H__CB3A8751_F9F9_4BD9_AABD_E893E67DEC56__INCLUDED_)
#define AFX_SMALLTOOLKIT_H__CB3A8751_F9F9_4BD9_AABD_E893E67DEC56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSmallToolKitApp:
// See SmallToolKit.cpp for the implementation of this class
//

class CSmallToolKitApp : public CWinApp
{
public:
	CSmallToolKitApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmallToolKitApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSmallToolKitApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMALLTOOLKIT_H__CB3A8751_F9F9_4BD9_AABD_E893E67DEC56__INCLUDED_)
