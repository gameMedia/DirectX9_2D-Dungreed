// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strInput(_T(""))
	, m_strOutput(_T(""))
	, m_strName(_T(""))
	, m_iAttack(0)
	, m_iDefence(0)
	, m_strFindName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
	for_each(m_mapUnitData.begin(), m_mapUnitData.end(),
		[](auto& MyPair)
	{
		SafeDelete(MyPair.second);
	});

	m_mapUnitData.clear();
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strInput);
	DDX_Text(pDX, IDC_EDIT2, m_strOutput);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iAttack);
	DDX_Text(pDX, IDC_EDIT5, m_iDefence);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBox[0]);
	DDX_Control(pDX, IDC_CHECK2, m_CheckBox[1]);
	DDX_Control(pDX, IDC_CHECK3, m_CheckBox[2]);
	DDX_Text(pDX, IDC_EDIT6, m_strFindName);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedPush)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnBnClickedAdd)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnBnClickedDelete)
	ON_EN_CHANGE(IDC_EDIT6, &CUnitTool::OnEnChangeFindName)
END_MESSAGE_MAP()


// CUnitTool �޽��� ó�����Դϴ�.


void CUnitTool::OnBnClickedPush()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// DoDataExChange�� �����Ѵ�.
	// ���ڰ� TRUE�� �� ��Ʈ�� -> ������ ������ ��ȯ.
	UpdateData(TRUE);

	m_strOutput = m_strInput;

	// ���� FALSE�� �� ���� -> ��Ʈ�ѷ� ������ ��ȯ.
	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	auto iter_find = m_mapUnitData.find(m_strName);

	if (m_mapUnitData.end() != iter_find)
		return;

	UNIT_DATA* pUnitData	= new UNIT_DATA;
	pUnitData->strName		= m_strName;
	pUnitData->iAttack		= m_iAttack;
	pUnitData->iDefence		= m_iDefence;

	for (BYTE i = 0; i < 3; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			pUnitData->byJobIndex = i;
			break;
		}
	}

	/*if (m_CheckBox[0].GetCheck())
		pUnitData->byItem |= RUBY;
	if (m_CheckBox[1].GetCheck())
		pUnitData->byItem |= SAPPHIRE;
	if (m_CheckBox[2].GetCheck())
		pUnitData->byItem |= DIAMOND;*/

	for (BYTE i = 0; i < 3; ++i)
	{
		if (m_CheckBox[i].GetCheck())
			pUnitData->byItem |= m_byItem[i];
	}

	m_mapUnitData.insert({ m_strName, pUnitData });

	// listbox ��Ͽ� ���ڿ��� �߰�.
	m_ListBox.AddString(m_strName);

	UpdateData(FALSE);
}


void CUnitTool::OnLbnSelchangeList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	int iIndex = m_ListBox.GetCurSel(); // ������ �ε��� ����.

	if (-1 == iIndex)
		return;

	CString strName = L"";
	m_ListBox.GetText(iIndex, strName); // �ε��� ��ġ�� �ش��ϴ� ���ڿ��� ����.

	auto iter_find = m_mapUnitData.find(strName);

	if (m_mapUnitData.end() == iter_find)
		return;

	m_strName = iter_find->second->strName;
	m_iAttack = iter_find->second->iAttack;
	m_iDefence = iter_find->second->iDefence;

	for (BYTE i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_CheckBox[i].SetCheck(FALSE);
	}

	BYTE byJobIndex = iter_find->second->byJobIndex;
	m_Radio[byJobIndex].SetCheck(TRUE);

	BYTE byItem = iter_find->second->byItem;
	for (BYTE i = 0; i < 3; ++i)
	{
		if (byItem & m_byItem[i])
			m_CheckBox[i].SetCheck(TRUE);
	}

	UpdateData(FALSE);
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_Radio[0].SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CUnitTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ����, ���� ��ȭ���ڸ� �������ִ� MFC Ŭ����.
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
		//MSG_BOX(path);

		HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwBytes = 0;		
		UNIT_DATA* pUnitData = nullptr;
		size_t iStringLen = 0;

		for (auto& MyPair : m_mapUnitData)
		{
			pUnitData = MyPair.second;
			iStringLen = pUnitData->strName.GetLength() + 1;

			WriteFile(hFile, &iStringLen, sizeof(size_t), &dwBytes, nullptr);
			WriteFile(hFile, pUnitData->strName.GetString(), sizeof(TCHAR) * iStringLen, &dwBytes, nullptr);
			WriteFile(hFile, &(pUnitData->iAttack), sizeof(int), &dwBytes, nullptr);
			WriteFile(hFile, &(pUnitData->iDefence), sizeof(int), &dwBytes, nullptr);
			WriteFile(hFile, &(pUnitData->byJobIndex), sizeof(BYTE), &dwBytes, nullptr);
			WriteFile(hFile, &(pUnitData->byItem), sizeof(BYTE), &dwBytes, nullptr);
		}

		CloseHandle(hFile);
	}
}


void CUnitTool::OnBnClickedLoad()
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
		//MSG_BOX(path);

		HANDLE hFile = CreateFile(path, GENERIC_READ, 0, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		// ����Ʈ�ڽ� ��� ��ü ���.
		m_ListBox.ResetContent();

		if (!m_mapUnitData.empty())
		{
			for_each(m_mapUnitData.begin(), m_mapUnitData.end(),
				[](auto& MyPair)
			{
				SafeDelete(MyPair.second);
			});

			m_mapUnitData.clear();
		}

		DWORD dwBytes = 0;
		UNIT_DATA* pUnitData = nullptr;
		size_t iStringLen = 0;
		TCHAR* pName = nullptr;

		while (true)
		{
			ReadFile(hFile, &iStringLen, sizeof(size_t), &dwBytes, nullptr);

			pName = new TCHAR[iStringLen];
			ReadFile(hFile, pName, sizeof(TCHAR) * iStringLen, &dwBytes, nullptr);

			pUnitData = new UNIT_DATA;
			pUnitData->strName = pName;
			SafeDelete_Array(pName);

			ReadFile(hFile, &(pUnitData->iAttack), sizeof(int), &dwBytes, nullptr);
			ReadFile(hFile, &(pUnitData->iDefence), sizeof(int), &dwBytes, nullptr);
			ReadFile(hFile, &(pUnitData->byJobIndex), sizeof(BYTE), &dwBytes, nullptr);
			ReadFile(hFile, &(pUnitData->byItem), sizeof(BYTE), &dwBytes, nullptr);

			if (0 == dwBytes)
			{
				SafeDelete(pUnitData);
				break;
			}

			m_mapUnitData.insert({ pUnitData->strName, pUnitData });
			m_ListBox.AddString(pUnitData->strName);
		}

		CloseHandle(hFile);
	}
}


void CUnitTool::OnBnClickedDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int iIndex = m_ListBox.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strName = L"";

	m_ListBox.GetText(iIndex, strName);

	auto iter_find = m_mapUnitData.find(strName);

	if (m_mapUnitData.end() == iter_find)
		return;

	SafeDelete(iter_find->second);
	m_mapUnitData.erase(strName);
	m_ListBox.DeleteString(iIndex);
}


void CUnitTool::OnEnChangeFindName()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	auto iter_find = m_mapUnitData.find(m_strFindName);

	if (m_mapUnitData.end() == iter_find)
		return;

	// listbox ��� �߿��� 0�� �ε������� m_strFindName�� ã�ƶ�!
	// ã�� �ε����� ��ȯ��.
	int iIndex = m_ListBox.FindString(0, m_strFindName);

	if (-1 == iIndex)
		return;

	// �ش� ��� ����
	m_ListBox.SetCurSel(iIndex);
	OnLbnSelchangeList();

	UpdateData(TRUE);
}
