// PopUp.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PopUp.h"
#include "afxdialogex.h"


// CPopUp ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPopUp, CDialog)

CPopUp::CPopUp(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_POPUP, pParent)
{

}

CPopUp::~CPopUp()
{
}

void CPopUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPopUp, CDialog)
END_MESSAGE_MAP()


// CPopUp �޽��� ó�����Դϴ�.


BOOL CPopUp::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if (nullptr == m_MySheet.GetSafeHwnd())
	{
		m_MySheet.Create(this, WS_CHILD | WS_VISIBLE);
		m_MySheet.MoveWindow(0, 0, 1000, 600);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
