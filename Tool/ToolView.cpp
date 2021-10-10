
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "MyForm.h"
#include "MyMap.h"
#include "Terrain.h"
#include "MyTestObj.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

HWND g_hWnd;

CToolView::CToolView()
	: m_pTerrain(nullptr)
	, m_pMap(nullptr)
	, iMyindex(0)
	,iAlpha(255)
	,iRed(0)
	,iGreen(0)
	,iBlue(0)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CToolView::~CToolView()
{
	SafeDelete(m_pTerrain);
	SafeDelete(m_pMap);
	CLineMgr::Get_Instance()->Destory_Instance();
	CTextureMgr::Get_Instance()->Destory_Instance();
	CDeviceMgr::Get_Instance()->Destory_Instance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	CDeviceMgr::Get_Instance()->Render_Begin();

	// �� ���̿� ������Ʈ Render ���� ����.
	m_pMap->Render();
	m_pTerrain->Render();
	m_pTestObj->Render();
	m_pTestObj->TestObjRender();
	
	CLineMgr::Get_Instance()->Render(this);

	CDeviceMgr::Get_Instance()->Render_End();
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	g_hWnd = m_hWnd;

	m_pMap = CMyMap::Create(this);
	NULL_CHECK_MSG(m_pMap, L"m_pTMap is null");
	m_pTerrain = CTerrain::Create(this);
	NULL_CHECK_MSG(m_pTerrain, L"m_pTerrain is null");
	m_pTestObj = CMyTestObj::Create(this);
	NULL_CHECK_MSG(m_pTestObj, L"m_pTestObj is null");

	// ��ũ�� ���� ���� 

	int cx = (int)(m_pTerrain->m_vecTile.front()->TILEX1 * (m_pTerrain->m_vecTile.front()->TILECX))* m_pTerrain->m_vecTile.front()->vSize.x;
	int cy = (int)(m_pTerrain->m_vecTile.front()->TILEY1 * (m_pTerrain->m_vecTile.front()->TILECY))* m_pTerrain->m_vecTile.front()->vSize.y;


	CScrollView::SetScrollSizes(MM_TEXT, CSize(cx, cy));

	// �ĸ���� ũ��� ���� View�� ũ�⸦ 1:1�� �����.
	// View�� ũ��� FrameWnd�� ũ�⸦ ���󰣴�.
	// ����, �츮�� FrameWnd�� ũ�⸦ ����ָ� �ȴ�.

	// FrameWnd ������
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	// FrameWnd�� �簢�� ũ�⸦ ���´�.
	RECT rcFrame = {};

	// GetWindowRect: �׵θ��� ������ �������� ��ü ũ�⸦ ������ �Լ�.
	// �� �� RECT�� left, right, top, bottom�� ��� ��ü ��ũ�� ���� ���� ��ǥ�̴�.
	pFrameWnd->GetWindowRect(&rcFrame);

	// View�� �簢�� ũ�⸦ ���´�.
	RECT rcView = {};

	// GetClientRect: �׵θ��� �������� ���� ���� Ŭ���̾�Ʈ ���� ũ�⸦ ������ �Լ�.
	// �� �� RECT�� left, right, top, bottom�� ��� Ŭ���̾�Ʈ ���� ��ǥ�̴�.
	GetClientRect(&rcView);

	// rcFrame�� left, top�� 0, 0 �������� ����.
	::SetRect(&rcFrame, 0, 0,
		rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);

	int iGapX = rcFrame.right - rcView.right;
	int iGapY = rcFrame.bottom - rcView.bottom;

	// FrameWnd�� ũ�⸦ ����.
	pFrameWnd->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER);

	HRESULT hr = CDeviceMgr::Get_Instance()->InitDevice(
		g_hWnd, WINCX, WINCY, CDeviceMgr::MODE_WIN);
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");

	// �̱� �ؽ�ó
	hr = CTextureMgr::Get_Instance()->LoadTexture(
		CTexture::TEX_SINGLE,
		L"../Texture/Cube.png",
		L"Cube");
	FAILED_CHECK_MSG(hr, L"Cube Single Texture Load Failed");

	// ��Ƽ �ؽ�ó
	hr = CTextureMgr::Get_Instance()->LoadTexture(
		CTexture::TEX_MULTI,
		L"../Texture/Stage/Terrain/Map/Map%d.png",
		L"Terrain",
		L"Map",
		3); // ���� ��
	FAILED_CHECK_MSG(hr, L"Map Load Failed");
	hr = CTextureMgr::Get_Instance()->LoadTexture(
		CTexture::TEX_MULTI,
		L"../Texture/Stage/Terrain/OptionTile/OptionTile%d.png",
		L"Terrain",
		L"OptionTile",
		10); // ���� ��
	FAILED_CHECK_MSG(hr, L"OptionTile Load Failed");

	// ��Ƽ �ؽ�ó
	hr = CTextureMgr::Get_Instance()->LoadTexture(
		CTexture::TEX_MULTI,
		L"../Texture/Tool/ImgTile/ImgTile%.2d.png",
		L"Terrain",
		L"ImgTile",
		40); // ImgTile
	FAILED_CHECK_MSG(hr, L"Img-Tile Multi Texture Load Failed");




}

// WM_LBUTTONDOWN �޽����� �߻��� �� ȣ��Ǵ� �޽��� �Լ�.
void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CScrollView::OnLButtonDown(nFlags, point);

	RECT rcView = {};
	GetClientRect(&rcView);

	float fRatioX = rcView.right / (float)WINCX;
	float fRatioY = rcView.bottom / (float)WINCY;

	D3DXVECTOR3 vMouse = {};
	vMouse.x = float(point.x) / fRatioX + CScrollView::GetScrollPos(0);
	vMouse.y = float(point.y) / fRatioY + CScrollView::GetScrollPos(1);

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pFormView = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitt.GetPane(1, 0));
	NULL_CHECK(pFormView);
	int iOption = pFormView->m_MapTool.iOption;
	int iDrawID = pFormView->m_MapTool.m_iDrawID;
	int ObjOption = pFormView->m_PopUp.m_MySheet.m_Page1.iOption;
	m_pTerrain->TileChange(vMouse, iDrawID, iOption);
	m_pTestObj->PickObj(vMouse, ObjOption);
	CLineMgr::Get_Instance()->SetLPoint(
		vMouse, iAlpha, iRed, iGreen, iBlue,iOption);
	// WM_PAINT �޽����� �߻�.
	Invalidate(FALSE);

	// MiniView ����
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
	NULL_CHECK(pMiniView);

	pMiniView->Invalidate(FALSE);
}

// ���콺�� ������ �� ȣ��Ǵ� �Լ�.
void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CScrollView::OnMouseMove(nFlags, point);

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pFormView = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitt.GetPane(1, 0));
	NULL_CHECK(pFormView);

		RECT rcView = {};
		GetClientRect(&rcView);

		float fRatioX = rcView.right / (float)WINCX;
		float fRatioY = rcView.bottom / (float)WINCY;

		D3DXVECTOR3 vMouse = {};
		vMouse.x = float(point.x) / fRatioX + CScrollView::GetScrollPos(0);
		vMouse.y = float(point.y) / fRatioY + CScrollView::GetScrollPos(1);

	m_pTestObj->TestObjSet(vMouse);
	pFormView->m_PopUp.m_MySheet.m_Page1.m_ObjPosX = vMouse.x;
	pFormView->m_PopUp.m_MySheet.m_Page1.m_ObjPosY = vMouse.y;
	pFormView->m_PopUp.m_MySheet.m_Page1.UpdateData(FALSE);
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{

		int iOption = pFormView->m_MapTool.iOption;
		int iDrawID = pFormView->m_MapTool.m_iDrawID;
		
		
		

		m_pTerrain->TileChange(vMouse, iDrawID, iOption);

		// WM_PAINT �޽����� �߻�.
		Invalidate(FALSE);

		// MiniView ����
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
		NULL_CHECK(pMiniView);

		pMiniView->Invalidate(FALSE);
	}
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		RECT rcView = {};
		GetClientRect(&rcView);

		float fRatioX = rcView.right / (float)WINCX;
		float fRatioY = rcView.bottom / (float)WINCY;

		D3DXVECTOR3 vMouse = {};
		vMouse.x = float(point.x) / fRatioX + CScrollView::GetScrollPos(0);
		vMouse.y = float(point.y) / fRatioY + CScrollView::GetScrollPos(1);

		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CMyForm* pFormView = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitt.GetPane(1, 0));
		NULL_CHECK(pFormView);
		int iOption = pFormView->m_MapTool.iOption;
		int iDrawID = pFormView->m_MapTool.m_iDrawID;
		m_pTerrain->ImgTileDelete(vMouse,iOption);

		// WM_PAINT �޽����� �߻�.
		Invalidate(FALSE);

		// MiniView ����
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
		NULL_CHECK(pMiniView);

		pMiniView->Invalidate(FALSE);
	}
	Invalidate(FALSE);

}

void CToolView::ReScroll()
{
	Invalidate(FALSE);
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
	NULL_CHECK(pMiniView);

	pMiniView->Invalidate(FALSE);


	int cx = (int)(m_pTerrain->m_vecTile.front()->TILEX1 * m_pTerrain->m_vecTile.front()->TILECX);
	int cy = (int)((m_pTerrain->m_vecTile.front()->TILEY1 * (m_pTerrain->m_vecTile.front()->TILECY)));

	CScrollView::SetScrollSizes(MM_TEXT, CSize(cx, cy));
}


void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnRButtonDown(nFlags, point);
	RECT rcView = {};
	GetClientRect(&rcView);

	float fRatioX = rcView.right / (float)WINCX;
	float fRatioY = rcView.bottom / (float)WINCY;

	D3DXVECTOR3 vMouse = {};
	vMouse.x = float(point.x) / fRatioX + CScrollView::GetScrollPos(0);
	vMouse.y = float(point.y) / fRatioY + CScrollView::GetScrollPos(1);

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pFormView = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitt.GetPane(1, 0));
	NULL_CHECK(pFormView);
	
	int iOption = pFormView->m_MapTool.iOption;


	m_pTerrain->ImgTileDelete(vMouse,iOption);

	// WM_PAINT �޽����� �߻�.
	Invalidate(FALSE);

	// MiniView ����
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
	NULL_CHECK(pMiniView);

	pMiniView->Invalidate(FALSE);
}











BOOL CToolView::PreTranslateMessage(MSG* pMsg)
{
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMyForm* pFormView = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitt.GetPane(1, 0));


	int testiMyindex = pFormView->m_MapTool.m_iDrawID;
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DOWN)
	{
		if (testiMyindex >= 39)
		{
			testiMyindex = 0;
		}
		else
		{
			testiMyindex++;
		}

		pFormView->m_MapTool.m_ListBox.SetCurSel(testiMyindex);
		pFormView->m_MapTool.OnLbnSelchangeList();

	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_UP)
	{
		if (testiMyindex <= 0)
		{
			testiMyindex = 39;
		}
		else
		{
			testiMyindex--;
		}

		pFormView->m_MapTool.m_ListBox.SetCurSel(testiMyindex);
		pFormView->m_MapTool.OnLbnSelchangeList();

	}
	return CScrollView::PreTranslateMessage(pMsg);
}


BOOL CToolView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());


	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
	

	pMiniView->Invalidate(FALSE);

	return CScrollView::OnScrollBy(sizeScroll, bDoScroll);
}


void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonUp(nFlags, point);

	RECT rcView = {};
	GetClientRect(&rcView);

	float fRatioX = rcView.right / (float)WINCX;
	float fRatioY = rcView.bottom / (float)WINCY;

	D3DXVECTOR3 vMouse = {};
	vMouse.x = float(point.x) / fRatioX + CScrollView::GetScrollPos(0);
	vMouse.y = float(point.y) / fRatioY + CScrollView::GetScrollPos(1);

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pFormView = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitt.GetPane(1, 0));
	NULL_CHECK(pFormView);

	int iOption = pFormView->m_MapTool.iOption;
	int iDrawID = pFormView->m_MapTool.m_iDrawID;


	CLineMgr::Get_Instance()->SetRPoint(
		vMouse, iAlpha, iRed, iGreen, iBlue, iOption);
	// WM_PAINT �޽����� �߻�.

	Invalidate(FALSE);

}
