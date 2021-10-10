#pragma once
#include "afxwin.h"


// CUnitTool ��ȭ �����Դϴ�.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnitTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
	// Edit Control�κ��� ���ڿ� �Է� ���� ��
	CString m_strInput;
	// Edit Control�� ���ڿ��� ����� ��
	CString m_strOutput;
	
	// ĳ���� �̸�
	CString m_strName;
	// ĳ���� ���ݷ�
	int m_iAttack;
	// ĳ���� ����
	int m_iDefence;

	// �˻� ������ ���ڿ� �˻��� ��
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
