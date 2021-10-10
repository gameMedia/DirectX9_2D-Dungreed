#include "stdafx.h"
#include "DeviceMgr.h"

INFLEMENT_SIGLETONE(CDeviceMgr)

CDeviceMgr::CDeviceMgr()
{
}


CDeviceMgr::~CDeviceMgr()
{
	Release();
}

LPDIRECT3DDEVICE9 CDeviceMgr::GetDevice()
{
	return m_pGraphicDev;
}

LPD3DXSPRITE CDeviceMgr::GetSprite()
{
	return m_pSprite;
}

LPD3DXFONT CDeviceMgr::GetFont()
{
	return m_pFont;
}
LPD3DXLINE CDeviceMgr::GetLine()
{
	return m_pLine;
}


HRESULT CDeviceMgr::InitDevice(
	HWND hWnd, 
	const DWORD & dwWinCX, 
	const DWORD & dwWinCY, 
	WIN_MODE eMode)
{
	// IDirect3D9 객체 생성.
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	NULL_CHECK_MSG_RETURN(m_pSDK, L"Direct3DCreate9 Failed", E_FAIL);

	// 장치 검증 및 조사 진행.
	D3DCAPS9	d3dcaps;
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	// HAL (Hardware Abstraction Layer, 하드웨어 추상 계층)
	HRESULT hr = m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps);
	FAILED_CHECK_MSG_RETURN(hr, L"GetDeviceCaps Failed", E_FAIL);

	// 하드웨어 버텍스 프로세싱 지원 유무를 조사.
	// 버텍스 프로세싱: 정점 처리 + 조명 처리
	DWORD vp = 0;

	if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// 렌더링 환경을 구성.
	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = dwWinCX;	// 후면버퍼 가로너비
	d3dpp.BackBufferHeight = dwWinCY;	// 후면버퍼 세로너비
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;	// 후면버퍼 픽셀포맷
	d3dpp.BackBufferCount = 1;	// 후면버퍼 개수

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 스왑체인 방식으로 깜빡임을 해소.
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eMode;

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// IDirect3DDevice9 객체 생성
	hr = m_pSDK->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_pGraphicDev);
	FAILED_CHECK_MSG_RETURN(hr, L"CreateDevice Failed", E_FAIL);

	// ID3DXSprite 객체 생성
	hr = D3DXCreateSprite(m_pGraphicDev, &m_pSprite);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateSprite Failed", E_FAIL);

	// ID3DXFont 객체 생성
	D3DXFONT_DESC tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESC));

	tFontInfo.Width = 10;
	tFontInfo.Height = 20;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");

	hr = D3DXCreateFontIndirect(m_pGraphicDev, &tFontInfo, &m_pFont);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateFontIndirect Failed", E_FAIL);

	// ID3DXLine 객체 생성
	hr = D3DXCreateLine(m_pGraphicDev, &m_pLine);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateLine Failed", E_FAIL);

	return S_OK;
}

void CDeviceMgr::Render_Begin()
{
	// 렌더링 과정
	// 1.후면버퍼를 비운다.
	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);

	// 2.후면버퍼에 그린다.
	m_pGraphicDev->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDeviceMgr::Render_End(HWND hWnd/* = nullptr*/)
{
	// 렌더링 과정
	// 3.후면버퍼에 그리기를 마친다.
	m_pSprite->End();
	m_pGraphicDev->EndScene();

	// 4.후면버퍼를 전면버퍼로 교체하여 화면에 렌더링한다.
	m_pGraphicDev->Present(nullptr, nullptr, hWnd, nullptr);
}

void CDeviceMgr::Release()
{
	// 모든 Com객체는 Reference Count(참조 횟수)를 갖고 있다.
	// 따라서, Reference Count가 비로소 0이어야 해당 Com객체는 완전히 소멸된다.
	// Com객체의 Release함수는 Reference Count를 하나씩 감소 시킨다.
	// 반대로 Com객체의 AddRef함수는 Reference Count를 하나씩 증가 시킨다.
	if (m_pLine->Release())
		MSG_BOX(L"m_pLine Release Failed");

	if(m_pFont->Release())
		MSG_BOX(L"m_pFont Release Failed");

	if(m_pSprite->Release())
		MSG_BOX(L"m_pSprite Release Failed");

	if (m_pGraphicDev->Release())
		MSG_BOX(L"m_pGraphicDev Release Failed");

	if (m_pSDK->Release())
		MSG_BOX(L"m_pSDK Release Failed");
}
