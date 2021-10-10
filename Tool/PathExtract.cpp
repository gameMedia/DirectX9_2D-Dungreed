// PathExtract.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathExtract.h"
#include "afxdialogex.h"

#include "FileInfo.h"


// CPathExtract ��ȭ �����Դϴ�.

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


// CPathExtract �޽��� ó�����Դϴ�.


void CPathExtract::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
		// ���� -> ���ڿ�
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


void CPathExtract::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(
		FALSE,	/* ���� ��ȭ���� */
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
			// ���� -> ���ڿ�
			
			_itow_s(pPathInfo->iImgCnt, szCount, 10);

			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|"
				+ szCount + L"|" + pPathInfo->wstrRelativePath;

			fout << wstrCombined << endl;
		}

		fout.close();

		// ���������� �⺻ �������α׷� ����.
		WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
	}
}


void CPathExtract::OnBnClickedLoad()
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
