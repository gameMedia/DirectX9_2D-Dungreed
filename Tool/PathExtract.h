#pragma once
#include "afxwin.h"


// CPathExtract 대화 상자입니다.

class CPathExtract : public CDialog
{
	DECLARE_DYNAMIC(CPathExtract)

public:
	CPathExtract(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathExtract();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHEXTRACT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
