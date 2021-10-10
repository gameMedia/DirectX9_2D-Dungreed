
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CToolView 생성/소멸

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
	// TODO: 여기에 생성 코드를 추가합니다.
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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	CDeviceMgr::Get_Instance()->Render_Begin();

	// 이 사이에 오브젝트 Render 콜을 수행.
	m_pMap->Render();
	m_pTerrain->Render();
	m_pTestObj->Render();
	m_pTestObj->TestObjRender();
	
	CLineMgr::Get_Instance()->Render(this);

	CDeviceMgr::Get_Instance()->Render_End();
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_hWnd = m_hWnd;

	m_pMap = CMyMap::Create(this);
	NULL_CHECK_MSG(m_pMap, L"m_pTMap is null");
	m_pTerrain = CTerrain::Create(this);
	NULL_CHECK_MSG(m_pTerrain, L"m_pTerrain is null");
	m_pTestObj = CMyTestObj::Create(this);
	NULL_CHECK_MSG(m_pTestObj, L"m_pTestObj is null");

	// 스크롤 범위 설정 

	int cx = (int)(m_pTerrain->m_vecTile.front()->TILEX1 * (m_pTerrain->m_vecTile.front()->TILECX))* m_pTerrain->m_vecTile.front()->vSize.x;
	int cy = (int)(m_pTerrain->m_vecTile.front()->TILEY1 * (m_pTerrain->m_vecTile.front()->TILECY))* m_pTerrain->m_vecTile.front()->vSize.y;


	CScrollView::SetScrollSizes(MM_TEXT, CSize(cx, cy));

	// 후면버퍼 크기와 실제 View의 크기를 1:1로 맞춘다.
	// View의 크기는 FrameWnd의 크기를 따라간다.
	// 따라서, 우리는 FrameWnd의 크기를 잡아주면 된다.

	// FrameWnd 얻어오기
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	// FrameWnd의 사각형 크기를 얻어온다.
	RECT rcFrame = {};

	// GetWindowRect: 테두리를 포함한 윈도우의 전체 크기를 얻어오는 함수.
	// 이 때 RECT의 left, right, top, bottom은 모두 전체 스크린 영역 기준 좌표이다.
	pFrameWnd->GetWindowRect(&rcFrame);

	// View의 사각형 크기를 얻어온다.
	RECT rcView = {};

	// GetClientRect: 테두리를 포함하지 않은 순수 클라이언트 영역 크기를 얻어오는 함수.
	// 이 때 RECT의 left, right, top, bottom은 모두 클라이언트 기준 좌표이다.
	GetClientRect(&rcView);

	// rcFrame의 left, top을 0, 0 기준으로 설정.
	::SetRect(&rcFrame, 0, 0,
		rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);

	int iGapX = rcFrame.right - rcView.right;
	int iGapY = rcFrame.bottom - rcView.bottom;

	// FrameWnd의 크기를 조정.
	pFrameWnd->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER);

	HRESULT hr = CDeviceMgr::Get_Instance()->InitDevice(
		g_hWnd, WINCX, WINCY, CDeviceMgr::MODE_WIN);
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");

	// 싱글 텍스처
	hr = CTextureMgr::Get_Instance()->LoadTexture(
		CTexture::TEX_SINGLE,
		L"../Texture/Cube.png",
		L"Cube");
	FAILED_CHECK_MSG(hr, L"Cube Single Texture Load Failed");

	// 멀티 텍스처
	hr = CTextureMgr::Get_Instance()->LoadTexture(
		CTexture::TEX_MULTI,
		L"../Texture/Stage/Terrain/Map/Map%d.png",
		L"Terrain",
		L"Map",
		3); // 마을 맵
	FAILED_CHECK_MSG(hr, L"Map Load Failed");
	hr = CTextureMgr::Get_Instance()->LoadTexture(
		CTexture::TEX_MULTI,
		L"../Texture/Stage/Terrain/OptionTile/OptionTile%d.png",
		L"Terrain",
		L"OptionTile",
		10); // 마을 맵
	FAILED_CHECK_MSG(hr, L"OptionTile Load Failed");

	// 멀티 텍스처
	hr = CTextureMgr::Get_Instance()->LoadTexture(
		CTexture::TEX_MULTI,
		L"../Texture/Tool/ImgTile/ImgTile%.2d.png",
		L"Terrain",
		L"ImgTile",
		40); // ImgTile
	FAILED_CHECK_MSG(hr, L"Img-Tile Multi Texture Load Failed");




}

// WM_LBUTTONDOWN 메시지가 발생할 때 호출되는 메시지 함수.
void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
	// WM_PAINT 메시지를 발생.
	Invalidate(FALSE);

	// MiniView 갱신
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
	NULL_CHECK(pMiniView);

	pMiniView->Invalidate(FALSE);
}

// 마우스가 움직일 때 호출되는 함수.
void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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

		// WM_PAINT 메시지를 발생.
		Invalidate(FALSE);

		// MiniView 갱신
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

		// WM_PAINT 메시지를 발생.
		Invalidate(FALSE);

		// MiniView 갱신
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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

	// WM_PAINT 메시지를 발생.
	Invalidate(FALSE);

	// MiniView 갱신
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
	NULL_CHECK(pMiniView);

	pMiniView->Invalidate(FALSE);
}











BOOL CToolView::PreTranslateMessage(MSG* pMsg)
{
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMyForm* pFormView = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitt.GetPane(1, 0));


	int testiMyindex = pFormView->m_MapTool.m_iDrawID;
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());


	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitt.GetPane(0, 0));
	

	pMiniView->Invalidate(FALSE);

	return CScrollView::OnScrollBy(sizeScroll, bDoScroll);
}


void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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
	// WM_PAINT 메시지를 발생.

	Invalidate(FALSE);

}
