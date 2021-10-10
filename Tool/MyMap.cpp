#include "stdafx.h"
#include "MyMap.h"

#include "ToolView.h"
#include "MiniView.h"

CMyMap::CMyMap()
	:m_pToolView(nullptr)
	,ShowMap(false)
	, m_MapSizeX(0)
	, m_MapSizeY(0)
{
}


CMyMap::~CMyMap()
{
}

void CMyMap::SetToolView(CToolView * pToolView)
{
	m_pToolView = pToolView;
}

void CMyMap::Render()
{
	D3DXMATRIX matScale,matTrans;
	const TEX_INFO* pTexInfo = nullptr;
	TCHAR szIndex[MIN_STR] = L"";

	D3DXMatrixScaling(&matScale, 
		m_MyMap->vSize.x, m_MyMap->vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans
		, m_MyMap->vPos.x - m_pToolView->GetScrollPos(0)
		, m_MyMap->vPos.y - m_pToolView->GetScrollPos(1),
		0.f);
	m_MyMap->matWorld = matScale*matTrans;

	pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
		L"Terrain", L"Map", m_MyMap->byDrawID);
	NULL_CHECK(pTexInfo);


	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&m_MyMap->matWorld);
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, nullptr,
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMyMap::MiniRender()
{
	D3DXMATRIX matScale, matTrans;
	const TEX_INFO* pTexInfo = nullptr;
	TCHAR szIndex[MIN_STR] = L"";

	pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
		L"Terrain", L"Map", m_MyMap->byDrawID);
	NULL_CHECK(pTexInfo);


	D3DXMatrixScaling(&matScale,
		m_MyMap->vSize.x * 0.5f,
		m_MyMap->vSize.y * 0.5f,
		0.f);
	D3DXMatrixTranslation(&matTrans
		, m_MyMap->vPos.x * 0.5f - m_pToolView->GetScrollPos(0)*0.5f
		, m_MyMap->vPos.y * 0.5f,
		0.f);
	m_MyMap->matWorld = matScale*matTrans;



	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&m_MyMap->matWorld);
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, nullptr,
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMyMap::MapChange(const BYTE & byDrawID ,
	const int & MapSizeX,
	const int & MapSizeY)
{
	m_MyMap->byDrawID = byDrawID;
	m_MapSizeX = MapSizeX;
	m_MapSizeY = MapSizeY;

	const TEX_INFO* pTexInfo = nullptr;

	pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
		L"Terrain", L"Map", m_MyMap->byDrawID);
	NULL_CHECK(pTexInfo);

	m_MyMap->vSize.x = ((float)m_MapSizeX / pTexInfo->tImgInfo.Width);
	m_MyMap->vSize.y = ((float)m_MapSizeY / pTexInfo->tImgInfo.Height);

	m_pToolView->Invalidate(FALSE);
}

HRESULT CMyMap::SaveMap(const TCHAR * pFilePath)
{
	return S_OK;
}

HRESULT CMyMap::LoadMap(const TCHAR * pFilePath)
{
	return S_OK;
}

HRESULT CMyMap::Initialize()
{
	m_MyMap = new MAP_INFO;
	m_MyMap->byDrawID = 0;
	m_MyMap->vSize = { 1.f, 1.f, 0.f };
	m_MyMap->vPos = { 0.f,0.f,0.f };
	return S_OK;
}

void CMyMap::Release()
{
}

CMyMap * CMyMap::Create(CToolView * pToolView)
{
	NULL_CHECK_RETURN(pToolView, nullptr);
	CMyMap* m_pInstance = new CMyMap;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	m_pInstance->SetToolView(pToolView);

	return m_pInstance;
}
