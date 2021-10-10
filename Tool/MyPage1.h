#pragma once
#include "afxwin.h"


// CMyPage1 대화 상자입니다.
class CToolView;
class CMainFrame;
class CMyPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyPage1)

public:
	CMyPage1();
	virtual ~CMyPage1();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYPAGE1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void PreInitDialog();
	afx_msg void OnCbnSelchangeComboObjectKey();
	CComboBox m_ObjectKey;


private:
	typedef map<wstring, list<wstring>>	MAP_TEXTURE;
	MAP_TEXTURE						m_mapTexture;
	map<wstring, int>				m_StateCount;
	CToolView* pToolView;

	CMainFrame* pFrameWnd;
public:
	TCHAR m_Obj[MAX_STR];
	TCHAR m_State[MAX_STR];
	int m_iDrawID;
	int iOption;
public:
	CListBox m_StateKey;
	afx_msg void OnLbnSelchangeListu1();
	afx_msg void OnLbnSelchangeListu2();
	CListBox m_NumberBox;
	CStatic m_PictureCtrlU1;
	float m_ObjPosX;
	float m_ObjPosY;
	float m_ObjSizeX;
	float m_ObjSizeY;
	afx_msg void OnBnClickedChecku1();
	CButton m_CheckPickObj;
	afx_msg void OnEnChangeEditObjSizeX();
	afx_msg void OnEnChangeEditObjSizeY();
	afx_msg void OnCbnSelchangeCombou2();
	CComboBox m_OptionSet;
	afx_msg void OnBnClickedButtonuSave();
	afx_msg void OnBnClickedButtonuLoad();
};
