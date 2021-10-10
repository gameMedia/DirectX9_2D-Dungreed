#include "stdafx.h"
#include "Obj.h"


CObj::CObj()
	:m_pDeviceMgr(CDeviceMgr::Get_Instance())
	,m_pTextureMgr(CTextureMgr::Get_Instance())
	,m_pTimeMgr(CTimeMgr::Get_Instance())
	, m_bIsInit(false)
	,m_bIsDead(false)
	,m_bColl(false)
	,iAttack(0)
{
	ZeroMemory(&m_tUnit, sizeof(UNIT_INFO));
	ZeroMemory(&m_tInfo, sizeof(INFO));
	// 월드행렬 초기화
	// 월드행렬 = 스케일 * 자전 * 이동 * 공전 * 부모
	D3DXMatrixIdentity(&(m_tUnit.matScale)); // 스케일 행렬
	D3DXMatrixIdentity(&(m_tUnit.matRotZ)); // 자전 행렬
	D3DXMatrixIdentity(&(m_tUnit.matTrans));  // 이동 행렬
	D3DXMatrixIdentity(&(m_tUnit.matPlanetZ)); // 공전 행렬
	D3DXMatrixIdentity(&(m_tUnit.matParent)); // 부모 행렬
	D3DXMatrixIdentity(&(m_tUnit.matWorld));  // 월드 행렬

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 1.f;
}


CObj::~CObj()
{
}

HRESULT CObj::LateInitialize()
{
	if (!m_bIsInit)
	{
		this->LateInitialize();
		m_bIsInit = true;
	}

	return S_OK;
}

void CObj::UpdateRect()
{

	m_tRect.left = LONG(m_tUnit.vPos.x - (m_tUnit.vLength.x * 0.5f));
	m_tRect.top = LONG(m_tUnit.vPos.y - (m_tUnit.vLength.y * 0.5f));
	m_tRect.right = LONG(m_tUnit.vPos.x + (m_tUnit.vLength.x * 0.5f));
	m_tRect.bottom = LONG(m_tUnit.vPos.y + (m_tUnit.vLength.y * 0.5f));
}

void CObj::FrameMove(float _AniPlaySpeed)
{
	m_tFrame.fCurFrame += m_tFrame.fFrameCnt*m_pTimeMgr->GetDelta()*_AniPlaySpeed;
	if (m_tFrame.fCurFrame >= m_tFrame.fFrameCnt)
		m_tFrame.fCurFrame = 0.f;
}

void CObj::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);

	m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matTrans;
}

void CObj::RectColl(CObj* pObj) // 충돌 이벤트
{
}

