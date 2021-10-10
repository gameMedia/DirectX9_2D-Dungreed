#pragma once



// CMyForm �� ���Դϴ�.
#include "UnitTool.h"
#include "MapTool.h"
#include "PopUp.h"
#include "PathExtract.h"

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate();

	DECLARE_MESSAGE_MAP()	

public:	// Message Function
	afx_msg void OnBnClickedUnitTool();
	afx_msg void OnBnClickedMapTool();
	afx_msg void OnBnClickedPopUp();
	afx_msg void OnBnClickedPathExtract();

public:
	// MFC���� �����ϴ� Font ��ü.
	CFont			m_Font;
	CUnitTool		m_UnitTool;
	CMapTool		m_MapTool;
	CPopUp			m_PopUp;
	CPathExtract	m_PathExtract;
};


