#pragma once
#include "afxwin.h"


// CPathExtract ��ȭ �����Դϴ�.

class CPathExtract : public CDialog
{
	DECLARE_DYNAMIC(CPathExtract)

public:
	CPathExtract(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathExtract();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHEXTRACT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:	// Message Functions
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

private:	// User Functions
	void SetHorizontalScroll();

public:		// Control variable
	CListBox m_ListBox;

public:		// User variable
	list<PATH_INFO*>	m_PathInfoLst;

	
};
