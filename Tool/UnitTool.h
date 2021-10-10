#pragma once
#include "afxwin.h"


// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:	// Message Function
	afx_msg void OnBnClickedPush();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedDelete();

public: // Value variable
	// Edit Control로부터 문자열 입력 받을 것
	CString m_strInput;
	// Edit Control에 문자열을 출력할 것
	CString m_strOutput;
	
	// 캐릭터 이름
	CString m_strName;
	// 캐릭터 공격력
	int m_iAttack;
	// 캐릭터 방어력
	int m_iDefence;

	// 검색 란에서 문자열 검색할 것
	CString m_strFindName;

public:	// Control variable
	CListBox m_ListBox;
	CButton m_Radio[3];
	CButton m_CheckBox[3];

public:	// User variable
	map<CString, UNIT_DATA*>	m_mapUnitData;	
	BYTE	m_byItem[3] = { RUBY, SAPPHIRE, DIAMOND };		
	afx_msg void OnEnChangeFindName();
};
