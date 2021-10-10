// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "MiniView.h"
#include "Terrain.h"
#include "MyMap.h"


// CMapTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent), m_iDrawID(0)
	, pToolView(nullptr)
	, m_iTileCX(0)
	, m_iTileCY(0)
	, m_TileX(0)
	, m_TIleY(0)
	, iOption(0)
	, m_iTileSizeX(0)
	, m_iTileSizeY(0)
	, m_MapDrawID(0)
	, m_PickTileSizeX(0)
	, m_PickTileSizeY(0)
	, m_iAlpha(0)
	, m_iRed(0)
	, m_iGreen(0)
	, m_iBlue(0)
	, m_MapSizeX(0)
	, m_MapSizeY(0)
{

}

CMapTool::~CMapTool()
{
	
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTH1, m_ListBox);
	DDX_Control(pDX, IDC_PICTUREH, m_PictureCtrl);
	DDX_Control(pDX, IDC_PICTUREH2, m_PictureCtrlH2);
	DDX_Control(pDX, IDC_LISTH2, m_AllTileSetListBox);
	DDX_Text(pDX, IDC_EDITH2, m_TileX);
	DDX_Text(pDX, IDC_EDITH3, m_TIleY);
	DDX_Control(pDX, IDC_CHECKH1, m_RecCheckBox);
	DDX_Control(pDX, IDC_CHECKH2, m_ShowLineCheck);

	DDX_Control(pDX, IDC_COMBOH1, m_ComboOptionList);
	DDX_Text(pDX, IDC_EDITH4, m_iTileSizeX);
	DDX_Text(pDX, IDC_EDITH5, m_iTileSizeY);

	DDX_Control(pDX, IDC_PICTUREH3, m_PictureCtrlH3);
	DDX_Control(pDX, IDC_COMBOH2, m_ComboMap);

	DDX_Text(pDX, IDC_EDITH6, m_PickTileSizeX);
	DDX_Text(pDX, IDC_EDITH7, m_PickTileSizeY);
	DDX_Control(pDX, IDC_PICTUREH4, m_OptionTilePicture);
	DDX_Control(pDX, IDC_CHECKH4, m_ShowOptionTile);
	DDX_Control(pDX, IDC_CHECKH5, m_PickTile_Check);
	DDX_Control(pDX, IDC_CHECKH6, m_PickLine_Check);
	DDX_Text(pDX, IDC_EDITH8, m_iAlpha);
	DDX_Text(pDX, IDC_EDITH9, m_iRed);
	DDX_Text(pDX, IDC_EDITH10, m_iGreen);
	DDX_Text(pDX, IDC_EDITH11, m_iBlue);
	DDX_Text(pDX, IDC_EDIT1, m_MapSizeX);
	DDX_Text(pDX, IDC_EDIT2, m_MapSizeY);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LISTH1, &CMapTool::OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_BUTTONH1, &CMapTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTONH2, &CMapTool::OnBnClickedSetButton)
	ON_BN_CLICKED(IDC_BUTTONH3, &CMapTool::OnBnClickedAddButton)
	ON_BN_CLICKED(IDC_BUTTONH6, &CMapTool::OnBnClickedLoad)
	ON_LBN_SELCHANGE(IDC_LISTH2, &CMapTool::OnLbnSelchangeList2)

	ON_BN_CLICKED(IDC_CHECKH1, &CMapTool::OnBnClickedCheckh1)
	ON_BN_CLICKED(IDC_CHECKH2, &CMapTool::OnBnClickedCheckh3)
	ON_CBN_SELCHANGE(IDC_COMBOH1, &CMapTool::OnCbnSelchangeCombo1)

	ON_CBN_SELCHANGE(IDC_COMBOH2, &CMapTool::OnCbnSelchangeComboh2)
	ON_BN_CLICKED(IDC_BUTTONH4, &CMapTool::OnBnClickedButtonh4)

	ON_WM_ACTIVATEAPP()

	ON_EN_CHANGE(IDC_EDITH6, &CMapTool::OnEnChangeEdith6)
	ON_EN_CHANGE(IDC_EDITH7, &CMapTool::OnEnChangeEdith7)
	ON_BN_CLICKED(IDC_CHECKH4, &CMapTool::OnBnClickedCheckh4)
	ON_BN_CLICKED(IDC_CHECKH5, &CMapTool::OnBnClickedCheckh5)
	ON_BN_CLICKED(IDC_CHECKH6, &CMapTool::OnBnClickedCheckh6)
	ON_EN_CHANGE(IDC_EDITH8, &CMapTool::OnEnChangeEdith8)
	ON_EN_CHANGE(IDC_EDITH9, &CMapTool::OnEnChangeEdith9)
	ON_EN_CHANGE(IDC_EDITH10, &CMapTool::OnEnChangeEdith10)
	ON_EN_CHANGE(IDC_EDITH11, &CMapTool::OnEnChangeEdith11)
	ON_BN_CLICKED(IDC_BUTTONH7, &CMapTool::OnBnClickedButtonSaveLine)
	ON_BN_CLICKED(IDC_BUTTONH8, &CMapTool::OnBnClickedButtonLoadLine)
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnDropFiles(hDropInfo);

	//MSG_BOX(L"�巡�� �� ���");

	TCHAR szFullPath[MAX_STR] = L"";

	//DragQueryFile(hDropInfo, 0, szFullPath, MAX_STR);
	//MSG_BOX(szFullPath);

	// �ι�° ���ڰ� -1�� �� �巡�� �� ��ӵ� ��ü ���� ������ ���� �� �ִ�.
	int iCount = DragQueryFile(hDropInfo, -1, nullptr, 0);

	CString strRelativePath = L"";
	CString strFileName = L"";
	TCHAR szFileName[MAX_STR] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_STR);

		// ��� ��� ��ȯ.
		strRelativePath = CFileInfo::ConvertRelativePath(szFullPath);

		// ���ϸ� ���ܺ���.
		strFileName = PathFindFileName(strRelativePath);

		// Ȯ���� ����.
		lstrcpy(szFileName, strFileName.GetString());
		PathRemoveExtension(szFileName);

		m_ListBox.AddString(szFileName);
	}

	SetHorizontalScroll();
}

void CMapTool::SetHorizontalScroll()
{
	int iCX = 0;
	CString strName = L"";
	CSize size;

	CDC* pDC = m_ListBox.GetDC();

	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// ���ڿ��� �ȼ� ������ ��ȯ.
		size = pDC->GetTextExtent(strName);

		if (iCX < size.cx)
			iCX = size.cx;
	}

	if (iCX > m_ListBox.GetHorizontalExtent())
	{
		// ���η� ��ũ�� �� �� �ִ� ���� ����.
		m_ListBox.SetHorizontalExtent(iCX);
	}

	m_ListBox.ReleaseDC(pDC);
}


void CMapTool::OnLbnSelchangeList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//m_ListBox.SetCurSel()
	int iIndex = m_ListBox.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strName = L"";
	m_ListBox.GetText(iIndex, strName);

	// ex) strName = L"Tile24"
	m_strName = strName;// ����ٰ�  �̸��� ����
	int i = 0;

	for (; i < strName.GetLength(); ++i)
	{
		// ���� ���ڰ� �������� �������� �Ǻ�.
		// ���ڶ�� true ����.
		if (isdigit(strName[i]))
			break;
	}

	// Delete(index, count)
	strName.Delete(0, i);

	// ���ڿ��� ����Ÿ������ ��ȯ.
	// _ttoi -> atoi, wtoi
	m_iDrawID = _ttoi(strName.GetString());

	const TEX_INFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
		L"Terrain", L"ImgTile", m_iDrawID);
	NULL_CHECK(pTexInfo);


	CDeviceMgr::Get_Instance()->Render_Begin();
	float fRatioX = WINCX / (float)pTexInfo->tImgInfo.Width;
	float fRatioY = WINCY / (float)pTexInfo->tImgInfo.Height;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, fRatioX, fRatioY, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

	matWorld = matScale * matTrans;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDeviceMgr::Get_Instance()->Render_End(m_PictureCtrl.m_hWnd);
}


void CMapTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(
		FALSE,	/* ���� ��ȭ���� */
		L"dat", /* ���ϸ� �������� �� �⺻ Ȯ���� */
		L"�������.dat", /* �����̸� ���� ������ ���� */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", /* �������� ���� ������ ����*/
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ��� ���� ���ϸ��� �����ϴ� �Լ�.
	// ��, ���ϸ��� ���� ��� ���� �������� �����Ѵ�.
	PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	//MSG_BOX(szCurrentPath);


	// �ʱ� ��� ���� (��� ��� X)
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath;

	if (IDOK == Dlg.DoModal())
	{

		CString path = Dlg.GetPathName();
		pToolView->m_pTerrain->SaveTile(path.GetString());

	}
}


void CMapTool::OnBnClickedLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(
		TRUE,	/* ���� ��ȭ���� */
		L"dat", /* ���ϸ� �������� �� �⺻ Ȯ���� */
		L"�������.dat", /* �����̸� ���� ������ ���� */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", /* �������� ���� ������ ����*/
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ��� ���� ���ϸ��� �����ϴ� �Լ�.
	// ��, ���ϸ��� ���� ��� ���� �������� �����Ѵ�.
	PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	//MSG_BOX(szCurrentPath);


	// �ʱ� ��� ���� (��� ��� X)
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath;

	if (IDOK == Dlg.DoModal())
	{

		CString path = Dlg.GetPathName();
		
		pToolView->m_pTerrain->LoadTile(path.GetString());
		
		pToolView->Invalidate(FALSE);

		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
		NULL_CHECK(pMiniView);

		pMiniView->Invalidate(FALSE);
	}
}




void CMapTool::PreInitDialog()
{// �ʱ�ȭ
 // TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);
	pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitt.GetPane(0, 1));
	NULL_CHECK(pToolView);




	CDialog::PreInitDialog();

}


void CMapTool::OnLbnSelchangeList2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	iIndex = m_AllTileSetListBox.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strName = L"";
	m_AllTileSetListBox.GetText(iIndex, strName);

	int i = 0;

	for (; i < strName.GetLength(); ++i)
	{
		// ���� ���ڰ� �������� �������� �Ǻ�.
		// ���ڶ�� true ����.
		if (isdigit(strName[i]))
			break;
	}

	// Delete(index, count)
	strName.Delete(0, i);


	m_AllDrawID = _ttoi(strName.GetString());

	const TEX_INFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
		L"Terrain", L"ImgTile", m_AllDrawID);
	NULL_CHECK(pTexInfo);
	
	m_iTileCX = pTexInfo->tImgInfo.Width;
	m_iTileCY = pTexInfo->tImgInfo.Height;

	CDeviceMgr::Get_Instance()->Render_Begin();
	float fRatioX = WINCX / (float)pTexInfo->tImgInfo.Width;
	float fRatioY = WINCY / (float)pTexInfo->tImgInfo.Height;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, fRatioX, fRatioY, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

	matWorld = matScale * matTrans;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDeviceMgr::Get_Instance()->Render_End(m_PictureCtrlH2.m_hWnd);

}


void CMapTool::OnBnClickedAddButton()
{
	//if()
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	auto iter_find = m_mapTileData.find(m_strName);

	if (m_mapTileData.end() != iter_find)
		return;

	m_mapTileData.insert({ m_strName,1 });


	m_AllTileSetListBox.AddString(m_strName);

}


void CMapTool::OnBnClickedSetButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TileAllChange(Ÿ�� x����, y����,
	// �̹��� ���α���, �̹��� ���α���, ID��, Option�� 
	UpdateData(TRUE);
	pToolView->m_pTerrain->TileAllChange(
		m_TileX, m_TIleY, m_iTileSizeX, m_iTileSizeY, m_AllDrawID,iOption);
	UpdateData(FALSE);

	pToolView->ReScroll();
}


void CMapTool::OnBnClickedCheckh1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_RecCheckBox.GetCheck())
	{
		pToolView->m_pTerrain->RectTileCheck = true;
	}
	else if (!(m_RecCheckBox.GetCheck()))
	{
		pToolView->m_pTerrain->RectTileCheck = false;
	}
}


void CMapTool::OnBnClickedCheckh3()
{
	//pToolView->UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_ShowLineCheck.GetCheck())
	{
		pToolView->m_pTerrain->ShowLine = true;
	}
	else if (!(m_ShowLineCheck.GetCheck()))
	{
		pToolView->m_pTerrain->ShowLine = false;
	}
	pToolView->Invalidate(FALSE); // invalidate �Ҷ��� backbuffer�� �ִ� ���� �ҷ��´�
	
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
	NULL_CHECK(pMiniView);

	pMiniView->Invalidate(FALSE);
}


void CMapTool::OnCbnSelchangeCombo1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	iOption = m_ComboOptionList.GetCurSel();
	const TEX_INFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
		L"Terrain", L"OptionTile", iOption);
	NULL_CHECK(pTexInfo);

	//m_iTileCX = pTexInfo->tImgInfo.Width;
	//m_iTileCY = pTexInfo->tImgInfo.Height;

	CDeviceMgr::Get_Instance()->Render_Begin();
	float fRatioX = WINCX / (float)pTexInfo->tImgInfo.Width;
	float fRatioY = WINCY / (float)pTexInfo->tImgInfo.Height;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, fRatioX, fRatioY, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

	matWorld = matScale * matTrans;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDeviceMgr::Get_Instance()->Render_End(m_OptionTilePicture.m_hWnd);
}


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.


	m_ComboOptionList.AddString(L"0.Normal");
	m_ComboOptionList.AddString(L"1.Collision");
	m_ComboOptionList.AddString(L"2.Dungeon1");
	m_ComboOptionList.AddString(L"3.Dungeon2");
	m_ComboOptionList.AddString(L"4.WeaponShop");
	m_ComboOptionList.AddString(L"5.Dungeon3");
	m_ComboOptionList.AddString(L"6.Restaurant");
	m_ComboOptionList.AddString(L"7.Dungeon4");
	m_ComboOptionList.AddString(L"8.ReinForce");
	m_ComboOptionList.AddString(L"9.Boss");

	m_ComboMap.AddString(L"0.����");
	m_ComboMap.AddString(L"1. ���� ���� ���̾�1");
	m_ComboMap.AddString(L"2. ���� ���� ���̾�2");
	m_ComboMap.SetCurSel(0);
	m_ComboOptionList.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}






void CMapTool::OnCbnSelchangeComboh2()
{
	m_MapDrawID = m_ComboMap.GetCurSel();
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	const TEX_INFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
		L"Terrain", L"Map", m_MapDrawID);
	NULL_CHECK(pTexInfo);

	//m_iTileCX = pTexInfo->tImgInfo.Width;
	//m_iTileCY = pTexInfo->tImgInfo.Height;

	CDeviceMgr::Get_Instance()->Render_Begin();
	float fRatioX = WINCX / (float)pTexInfo->tImgInfo.Width;
	float fRatioY = WINCY / (float)pTexInfo->tImgInfo.Height;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, fRatioX, fRatioY, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

	matWorld = matScale * matTrans;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDeviceMgr::Get_Instance()->Render_End(m_PictureCtrlH3.m_hWnd);
}


void CMapTool::OnBnClickedButtonh4()
{
	UpdateData(TRUE);
	pToolView->m_pMap->MapChange(m_MapDrawID,m_MapSizeX,m_MapSizeY);
	pToolView->ReScroll();
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}




void CMapTool::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CDialog::OnActivateApp(bActive, dwThreadID);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}




void CMapTool::OnEnChangeEdith6()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	pToolView->m_pTerrain->PickTileSizeX = m_PickTileSizeX;
}


void CMapTool::OnEnChangeEdith7()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	pToolView->m_pTerrain->PickTileSizeY = m_PickTileSizeY;
}


void CMapTool::OnBnClickedCheckh4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (m_ShowOptionTile.GetCheck())
	{
		pToolView->m_pTerrain->ShowOptionTile = true;
	}
	else if (!(m_ShowOptionTile.GetCheck()))
	{
		pToolView->m_pTerrain->ShowOptionTile = false;
	}
	pToolView->Invalidate(FALSE); // invalidate �Ҷ��� backbuffer�� �ִ� ���� �ҷ��´�
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
	NULL_CHECK(pMiniView);

	pMiniView->Invalidate(FALSE);
	
}


void CMapTool::OnBnClickedCheckh5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_PickTile_Check.GetCheck())
	{
		pToolView->m_pTerrain->PickTile_Check = true;
		m_PickLine_Check.SetCheck(FALSE);
		OnBnClickedCheckh6();
	}
	else if (!(m_PickTile_Check.GetCheck()))
	{
		pToolView->m_pTerrain->PickTile_Check = false;
	}
	pToolView->Invalidate(FALSE); // invalidate �Ҷ��� backbuffer�� �ִ� ���� �ҷ��´�
}



void CMapTool::OnBnClickedCheckh6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_PickLine_Check.GetCheck())
	{
		CLineMgr::Get_Instance()->DrawLineCheck = true;
		m_PickTile_Check.SetCheck(FALSE);
		OnBnClickedCheckh5();
	}
	else if (!(m_PickLine_Check.GetCheck()))
	{
		CLineMgr::Get_Instance()->DrawLineCheck = false;
	}
	pToolView->Invalidate(FALSE); // invalidate �Ҷ��� backbuffer�� �ִ� ���� �ҷ��´�
}


void CMapTool::OnEnChangeEdith8()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	pToolView->iAlpha = m_iAlpha;
}


void CMapTool::OnEnChangeEdith9()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	pToolView->iRed = m_iRed;
}


void CMapTool::OnEnChangeEdith10()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	pToolView->iGreen = m_iGreen;
}


void CMapTool::OnEnChangeEdith11()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	pToolView->iBlue = m_iBlue;
}


void CMapTool::OnBnClickedButtonSaveLine()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(
		FALSE,	/* ���� ��ȭ���� */
		L"dat", /* ���ϸ� �������� �� �⺻ Ȯ���� */
		L"�������.dat", /* �����̸� ���� ������ ���� */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", /* �������� ���� ������ ����*/
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ��� ���� ���ϸ��� �����ϴ� �Լ�.
	// ��, ���ϸ��� ���� ��� ���� �������� �����Ѵ�.
	PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	//MSG_BOX(szCurrentPath);


	// �ʱ� ��� ���� (��� ��� X)
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath;

	if (IDOK == Dlg.DoModal())
	{

		CString path = Dlg.GetPathName();
		CLineMgr::Get_Instance()->SaveData(path.GetString());
	}
}


void CMapTool::OnBnClickedButtonLoadLine()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(
		TRUE,	/* ���� ��ȭ���� */
		L"dat", /* ���ϸ� �������� �� �⺻ Ȯ���� */
		L"�������.dat", /* �����̸� ���� ������ ���� */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", /* �������� ���� ������ ����*/
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ��� ���� ���ϸ��� �����ϴ� �Լ�.
	// ��, ���ϸ��� ���� ��� ���� �������� �����Ѵ�.
	PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	//MSG_BOX(szCurrentPath);


	// �ʱ� ��� ���� (��� ��� X)
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath;

	if (IDOK == Dlg.DoModal())
	{

		CString path = Dlg.GetPathName();

		CLineMgr::Get_Instance()->LoadData(path.GetString());

		pToolView->Invalidate(FALSE);

		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
		NULL_CHECK(pMiniView);

		pMiniView->Invalidate(FALSE);
	}
}
