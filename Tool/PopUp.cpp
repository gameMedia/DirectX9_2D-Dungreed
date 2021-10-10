// PopUp.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PopUp.h"
#include "afxdialogex.h"


// CPopUp 대화 상자입니다.

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


// CPopUp 메시지 처리기입니다.


BOOL CPopUp::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if (nullptr == m_MySheet.GetSafeHwnd())
	{
		m_MySheet.Create(this, WS_CHILD | WS_VISIBLE);
		m_MySheet.MoveWindow(0, 0, 1000, 600);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
