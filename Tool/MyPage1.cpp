// MyPage1.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyPage1.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "MyTestObj.h"
// CMyPage1 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMyPage1, CPropertyPage)

CMyPage1::CMyPage1()
	: CPropertyPage(IDD_MYPAGE1)
	, m_ObjPosX(0)
	, m_ObjPosY(0)
	, m_ObjSizeX(0)
	, m_ObjSizeY(0)
	, m_iDrawID(0)
	, iOption(0)
{

}

CMyPage1::~CMyPage1()
{
}

void CMyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOU1, m_ObjectKey);
	DDX_Control(pDX, IDC_LISTU1, m_StateKey);
	DDX_Control(pDX, IDC_LISTU2, m_NumberBox);
	DDX_Control(pDX, IDC_PICTUREU1, m_PictureCtrlU1);
	DDX_Text(pDX, IDC_EDITU1, m_ObjPosX);
	DDX_Text(pDX, IDC_EDITU2, m_ObjPosY);
	DDX_Text(pDX, IDC_EDITU3, m_ObjSizeX);
	DDX_Text(pDX, IDC_EDITU4, m_ObjSizeY);
	DDX_Control(pDX, IDC_CHECKU1, m_CheckPickObj);
	DDX_Control(pDX, IDC_COMBOU2, m_OptionSet);
}


BEGIN_MESSAGE_MAP(CMyPage1, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBOU1, &CMyPage1::OnCbnSelchangeComboObjectKey)
	ON_LBN_SELCHANGE(IDC_LISTU1, &CMyPage1::OnLbnSelchangeListu1)
	ON_LBN_SELCHANGE(IDC_LISTU2, &CMyPage1::OnLbnSelchangeListu2)
	ON_BN_CLICKED(IDC_CHECKU1, &CMyPage1::OnBnClickedChecku1)
	ON_EN_CHANGE(IDC_EDITU3, &CMyPage1::OnEnChangeEditObjSizeX)
	ON_EN_CHANGE(IDC_EDITU4, &CMyPage1::OnEnChangeEditObjSizeY)
	ON_CBN_SELCHANGE(IDC_COMBOU2, &CMyPage1::OnCbnSelchangeCombou2)
	ON_BN_CLICKED(IDC_BUTTONU1, &CMyPage1::OnBnClickedButtonuSave)
	ON_BN_CLICKED(IDC_BUTTONU2, &CMyPage1::OnBnClickedButtonuLoad)
END_MESSAGE_MAP()


// CMyPage1 �޽��� ó�����Դϴ�.


BOOL CMyPage1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_OptionSet.AddString(L"0.Normal");
	m_OptionSet.AddString(L"1.Event");

	m_OptionSet.SetCurSel(0);

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
#ifdef _UNICODE
	wifstream	fin;
#else 
	ifstream	fin;
#endif

	fin.open(L"../Data/ObjImgPath.txt");

	TCHAR szObjectKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";

	TCHAR szImgCount[MAX_STR] = L"";
	TCHAR szRelativePath[MAX_STR] = L"";
	
	while (true)
	{
		fin.getline(szObjectKey, MAX_STR, '|');
		fin.getline(szStateKey, MAX_STR, '|');
		fin.getline(szImgCount, MAX_STR, '|');
		fin.getline(szRelativePath, MAX_STR);

		if (fin.eof())
			break;

		auto iter_find = m_mapTexture.find(szObjectKey);
			m_mapTexture[szObjectKey].emplace_back(szStateKey);

		auto iter_find1 = m_StateCount.find(szStateKey);
			m_StateCount[szStateKey] = _ttoi(szImgCount);
	}

	fin.close();


	for (auto iter : m_mapTexture)
	{
		m_ObjectKey.AddString((iter.first).c_str());

	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}



void CMyPage1::PreInitDialog()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);
	pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitt.GetPane(0, 1));
	NULL_CHECK(pToolView);
	
	HRESULT hr = CTextureMgr::Get_Instance()->LoadFromImgPath(L"../Data/ObjImgPath.txt");
	FAILED_CHECK_MSG(hr, L"ImgPath Load Failed");




	CPropertyPage::PreInitDialog();
}


void CMyPage1::OnCbnSelchangeComboObjectKey()
{
	m_StateKey.ResetContent();
	int iCount = m_ObjectKey.GetCurSel();
	wstring m_Text = L"";
	

	m_ObjectKey.GetLBText(iCount, m_Obj);
	
	auto iter_find = m_mapTexture.find(m_Obj);
	if (iter_find == m_mapTexture.end())
	{
		return;
	}
	else
	{
		for (auto iter2 : iter_find->second)
		{
			m_StateKey.AddString(iter2.c_str());
		}
	}

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMyPage1::OnLbnSelchangeListu1()
{
	 //TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_NumberBox.ResetContent();
	int iIndex = m_StateKey.GetCurSel();
	
	if (-1 == iIndex)
		return;

	
	CString Count;

	m_StateKey.GetText(iIndex, m_State);

	for (auto iter : m_StateCount)
	{
		if (iter.first == m_State)
		{
			for (int i = 0; i < iter.second; ++i)
			{
				Count.Format(L"%d", i);
				CString StateKey = m_State + Count;
				m_NumberBox.AddString(StateKey);
			}
		}
	}
	//auto iter_find = m_StateCount.find(m_State);

	//if (iter_find == m_StateCount.end())
	//{
	//	return;
	//}
	//else
	//{
	//	for (int i = 0; i < iter_find->second; ++i)
	//	{
	//		Count.Format(L"%d", i);
	//		CString StateKey = m_State + Count;
	//		m_NumberBox.AddString(StateKey);

	//	}
	//}
	//Count.Format(L"%d", 1);
}


void CMyPage1::OnLbnSelchangeListu2()
{
	int iIndex = m_NumberBox.GetCurSel();
	if (-1 == iIndex)
		return;
	CString strName = L"";
	m_NumberBox.GetText(iIndex, strName);
	
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
		m_Obj, m_State, m_iDrawID);
	NULL_CHECK(pTexInfo);
	pToolView->m_pTestObj->m_Obj = m_Obj;
	pToolView->m_pTestObj->m_State = m_State;
	pToolView->m_pTestObj->m_iDrawID = m_iDrawID;
	UpdateData(TRUE);

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

	CDeviceMgr::Get_Instance()->Render_End(m_PictureCtrlU1.m_hWnd);
	

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMyPage1::OnBnClickedChecku1()
{
	if (m_CheckPickObj.GetCheck())
	{
		pToolView->m_pTestObj->TestObjCheck = true;
	}
	else if (!(m_CheckPickObj.GetCheck()))
	{
		pToolView->m_pTestObj->TestObjCheck = false;
	}
	pToolView->Invalidate(FALSE);
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMyPage1::OnEnChangeEditObjSizeX()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CPropertyPage::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	pToolView->m_pTestObj->TestObjSizeX = m_ObjSizeX;
}


void CMyPage1::OnEnChangeEditObjSizeY()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CPropertyPage::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	pToolView->m_pTestObj->TestObjSizeY = m_ObjSizeY;
}


void CMyPage1::OnCbnSelchangeCombou2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	iOption = m_OptionSet.GetCurSel();

	
}


void CMyPage1::OnBnClickedButtonuSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(
		FALSE,	/* ���� ��ȭ���� */
		L"txt", /* ���ϸ� �������� �� �⺻ Ȯ���� */
		L"�������.txt", /* �����̸� ���� ������ ���� */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Text Files(*.dat)|*.dat||", /* �������� ���� ������ ����*/
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
		pToolView->m_pTestObj->SaveObjlist(path.GetString());

	}
}


void CMyPage1::OnBnClickedButtonuLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(
		TRUE,	/* ���� ��ȭ���� */
		L"txt", /* ���ϸ� �������� �� �⺻ Ȯ���� */
		L"�������.txt", /* �����̸� ���� ������ ���� */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||", /* �������� ���� ������ ����*/
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
		pToolView->m_pTestObj->LoadObjlist(path.GetString());
	}
}
