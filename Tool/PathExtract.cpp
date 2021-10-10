// PathExtract.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathExtract.h"
#include "afxdialogex.h"

#include "FileInfo.h"


// CPathExtract 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathExtract, CDialog)

CPathExtract::CPathExtract(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHEXTRACT, pParent)
{

}

CPathExtract::~CPathExtract()
{
	for_each(m_PathInfoLst.begin(), m_PathInfoLst.end(), SafeDelete<PATH_INFO*>);
	m_PathInfoLst.clear();
}

void CPathExtract::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathExtract, CDialog)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CPathExtract::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON6, &CPathExtract::OnBnClickedLoad)
END_MESSAGE_MAP()


// CPathExtract 메시지 처리기입니다.


void CPathExtract::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDropFiles(hDropInfo);

	int iCount = DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFullPath[MAX_STR] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_STR);		
		CFileInfo::DirInfoExtraction(szFullPath, m_PathInfoLst);
	}

	m_ListBox.ResetContent();

	wstring wstrCombined = L"";
	TCHAR szCount[MIN_STR] = L"";

	for (auto& pPathInfo : m_PathInfoLst)
	{
		// 정수 -> 문자열
		_itow_s(pPathInfo->iImgCnt, szCount, 10);

		wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|"
			+ szCount + L"|" + pPathInfo->wstrRelativePath;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	SetHorizontalScroll();
}

void CPathExtract::SetHorizontalScroll()
{
	int iCX = 0;
	CString strName = L"";
	CSize size;

	CDC* pDC = m_ListBox.GetDC();

	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// 문자열을 픽셀 단위로 변환.
		size = pDC->GetTextExtent(strName);

		if (iCX < size.cx)
			iCX = size.cx;
	}

	if (iCX > m_ListBox.GetHorizontalExtent())
	{
		// 가로로 스크롤 할 수 있는 범위 지정.
		m_ListBox.SetHorizontalExtent(iCX);
	}

	m_ListBox.ReleaseDC(pDC);
}


void CPathExtract::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(
		FALSE,	/* 저장 대화상자 */
		L"txt", /* 파일명만 기재했을 때 기본 확장자 */
		L"제목없음.txt", /* 파일이름 란에 보여질 문구 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||", /* 파일형식 란에 보여질 문구*/
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

#ifdef _UNICODE
		wofstream fout;
#else
		ofstream fout;
#endif

		fout.open(path.GetString());

		if (fout.fail())
			return;

		wstring wstrCombined = L"";
		TCHAR szCount[MIN_STR] = L"";

		for (auto& pPathInfo : m_PathInfoLst)
		{
			// 정수 -> 문자열
			
			_itow_s(pPathInfo->iImgCnt, szCount, 10);

			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|"
				+ szCount + L"|" + pPathInfo->wstrRelativePath;

			fout << wstrCombined << endl;
		}

		fout.close();

		// 윈도우즈의 기본 응용프로그램 실행.
		WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
	}
}


void CPathExtract::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(
		TRUE,	/* 저장 대화상자 */
		L"txt", /* 파일명만 기재했을 때 기본 확장자 */
		L"제목없음.txt", /* 파일이름 란에 보여질 문구 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||", /* 파일형식 란에 보여질 문구*/
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

#ifdef _UNICODE
		wifstream fin;
#else
		ifstream fin;
#endif

		fin.open(path.GetString());

		if (fin.fail())
			return;

		if (!m_PathInfoLst.empty())
		{
			for_each(m_PathInfoLst.begin(), m_PathInfoLst.end(), SafeDelete<PATH_INFO*>);
			m_PathInfoLst.clear();
		}

		m_ListBox.ResetContent();

		TCHAR szObjectKey[MAX_STR] = L"";
		TCHAR szStateKey[MAX_STR] = L"";
		TCHAR szImgCount[MAX_STR] = L"";
		TCHAR szRelativePath[MAX_STR] = L"";
		int iImgCount = 0;

		PATH_INFO* pPathInfo = nullptr;
		wstring wstrCombined = L"";

		while (true)
		{
			fin.getline(szObjectKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szImgCount, MAX_STR, '|');
			fin.getline(szRelativePath, MAX_STR);

			if (fin.eof())
				break;

			pPathInfo = new PATH_INFO;
			pPathInfo->wstrObjectKey = szObjectKey;
			pPathInfo->wstrStateKey = szStateKey;
			pPathInfo->iImgCnt = _ttoi(szImgCount);
			pPathInfo->wstrRelativePath = szRelativePath;

			m_PathInfoLst.push_back(pPathInfo);			

			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|"
				+ szImgCount + L"|" + pPathInfo->wstrRelativePath;

			m_ListBox.AddString(wstrCombined.c_str());
		}

		fin.close();	
		SetHorizontalScroll();
	}
}
