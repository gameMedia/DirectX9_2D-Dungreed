// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool 대화 상자입니다.

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


// CUnitTool 메시지 처리기입니다.


void CUnitTool::OnBnClickedPush()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// DoDataExChange를 실행한다.
	// 인자가 TRUE일 때 컨트롤 -> 변수로 데이터 교환.
	UpdateData(TRUE);

	m_strOutput = m_strInput;

	// 인자 FALSE일 때 변수 -> 컨트롤로 데이터 교환.
	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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

	// listbox 목록에 문자열을 추가.
	m_ListBox.AddString(m_strName);

	UpdateData(FALSE);
}


void CUnitTool::OnLbnSelchangeList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	int iIndex = m_ListBox.GetCurSel(); // 선택한 인덱스 얻어옴.

	if (-1 == iIndex)
		return;

	CString strName = L"";
	m_ListBox.GetText(iIndex, strName); // 인덱스 위치에 해당하는 문자열을 얻어옴.

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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Radio[0].SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUnitTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 저장, 열기 대화상자를 생성해주는 MFC 클래스.
	CFileDialog Dlg(
		FALSE,	/* 저장 대화상자 */
		L"dat", /* 파일명만 기재했을 때 기본 확장자 */
		L"제목없음.dat", /* 파일이름 란에 보여질 문구 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", /* 파일형식 란에 보여질 문구*/
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 경로 상의 파일명을 제거하는 함수.
	// 단, 파일명이 없을 경우 말단 폴더명을 제거한다.
	PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	//MSG_BOX(szCurrentPath);


	// 초기 경로 설정 (상대 경로 X)
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(
		TRUE,	/* 열기 대화상자 */
		L"dat", /* 파일명만 기재했을 때 기본 확장자 */
		L"제목없음.dat", /* 파일이름 란에 보여질 문구 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", /* 파일형식 란에 보여질 문구*/
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 경로 상의 파일명을 제거하는 함수.
	// 단, 파일명이 없을 경우 말단 폴더명을 제거한다.
	PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	//MSG_BOX(szCurrentPath);


	// 초기 경로 설정 (상대 경로 X)
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath;

	if (IDOK == Dlg.DoModal())
	{
		CString path = Dlg.GetPathName();
		//MSG_BOX(path);

		HANDLE hFile = CreateFile(path, GENERIC_READ, 0, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		// 리스트박스 목록 전체 비움.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	auto iter_find = m_mapUnitData.find(m_strFindName);

	if (m_mapUnitData.end() == iter_find)
		return;

	// listbox 목록 중에서 0번 인덱스부터 m_strFindName를 찾아라!
	// 찾은 인덱스를 반환함.
	int iIndex = m_ListBox.FindString(0, m_strFindName);

	if (-1 == iIndex)
		return;

	// 해당 목록 선택
	m_ListBox.SetCurSel(iIndex);
	OnLbnSelchangeList();

	UpdateData(TRUE);
}
