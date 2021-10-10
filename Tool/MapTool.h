#pragma once
#include "afxwin.h"


// CMapTool ��ȭ �����Դϴ�.
class CToolView;
class CMainFrame;
class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:	// Message Functions
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

private:	// User Functions
	void SetHorizontalScroll();


public:	// Control variable
	CListBox m_ListBox;
	CStatic m_PictureCtrl;
	CStatic m_PictureCtrlH2;

public:	// User variable
	int m_iDrawID;
	int iIndex;
	int iOption;
	int m_AllDrawID; // All 
	int m_MapDrawID;
	list<PATH_INFO*>	m_PathInfoLst;

	int m_iTileCX;
	int m_iTileCY;
	CString m_strName;
	PATH_INFO m_pPath;
private:
	CToolView* pToolView;
	CMainFrame* pFrameWnd;


public:
	afx_msg void OnLbnSelchangeList2();
	// ��� Ÿ���� �ٲܶ� ������
	CListBox m_AllTileSetListBox;
	CButton m_RecCheckBox;
	CButton m_ShowLineCheck;
	virtual void PreInitDialog();

	afx_msg void OnBnClickedCheckh1();
	afx_msg void OnBnClickedAddButton();
	afx_msg void OnBnClickedSetButton();
	//int m_TileX1;
	//int m_TileY1;
	int m_TileX;
	int m_TIleY;
	map<CString, int>	m_mapTileData;

	// // üũ���·� On�ϋ� ��� Off�϶� ��� ���ϱ�
	afx_msg void OnBnClickedCheckh3();
	// Line�� ���� ������
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_ComboOptionList;
	virtual BOOL OnInitDialog();
	int m_iTileSizeX;
	int m_iTileSizeY;
	
	
	CStatic m_PictureCtrlH3;
	afx_msg void OnCbnSelchangeComboh2();
	CComboBox m_ComboMap;
	afx_msg void OnBnClickedButtonh4();


	int m_PickTileSizeX;
	int m_PickTileSizeY;
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	afx_msg void OnEnChangeEdith6();
	afx_msg void OnEnChangeEdith7();
	CStatic m_OptionTilePicture;
	afx_msg void OnBnClickedCheckh4();
	CButton m_ShowOptionTile;
	afx_msg void OnBnClickedCheckh5();
	CButton m_PickTile_Check;
	afx_msg void OnBnClickedCheckh6();
	CButton m_PickLine_Check;
	afx_msg void OnEnChangeEdith8();
	afx_msg void OnEnChangeEdith9();
	afx_msg void OnEnChangeEdith10();
	afx_msg void OnEnChangeEdith11();
	int m_iAlpha;
	int m_iRed;
	int m_iGreen;
	int m_iBlue;
	afx_msg void OnBnClickedButtonSaveLine();
	afx_msg void OnBnClickedButtonLoadLine();
	int m_MapSizeX;
	int m_MapSizeY;
};
