#pragma once


// CPopUp ��ȭ �����Դϴ�.

#include "MySheet.h"

class CPopUp : public CDialog
{
	DECLARE_DYNAMIC(CPopUp)

public:
	CPopUp(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPopUp();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POPUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CMySheet m_MySheet;
	virtual BOOL OnInitDialog();
};
