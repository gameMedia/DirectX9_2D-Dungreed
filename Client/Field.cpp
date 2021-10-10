#include "stdafx.h"
#include "Field.h"


CField::CField()
	:m_pDeviceMgr(CDeviceMgr::Get_Instance())
	,m_pTextureMgr(CTextureMgr::Get_Instance())
	,m_pTimeMgr(CTimeMgr::Get_Instance())
	,m_bIsInit(false)
{
	ZeroMemory(&m_tMap, sizeof(UNIT_INFO));

	// 월드행렬 초기화
	// 월드행렬 = 스케일 * 자전 * 이동 * 공전 * 부모
	D3DXMatrixIdentity(&(m_tMap.matScale)); // 스케일 행렬
	D3DXMatrixIdentity(&(m_tMap.matRotZ)); // 자전 행렬
	D3DXMatrixIdentity(&(m_tMap.matTrans));  // 이동 행렬
	D3DXMatrixIdentity(&(m_tMap.matPlanetZ)); // 공전 행렬
	D3DXMatrixIdentity(&(m_tMap.matParent)); // 부모 행렬
	D3DXMatrixIdentity(&(m_tMap.matWorld));  // 월드 행렬

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 1.f;

}


CField::~CField()
{
}

HRESULT CField::LateInitialize()
{
	if (!m_bIsInit)
	{
		this->LateInitialize();
		m_bIsInit = true;
	}

	return S_OK;
}

void CField::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tMap.matScale, m_tMap.vSize.x, m_tMap.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tMap.matTrans,
		m_tMap.vPos.x - CScrollMgr::GetScrollPos(0),
		m_tMap.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);

	m_tMap.matWorld = m_tMap.matScale * m_tMap.matTrans;
}
