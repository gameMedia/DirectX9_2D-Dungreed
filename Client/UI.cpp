#include "stdafx.h"
#include "UI.h"


CUI::CUI()
	:m_bIsInit(false)
	,m_bIsDead(false)
	, m_pDeviceMgr(CDeviceMgr::Get_Instance())
	, m_pTextureMgr(CTextureMgr::Get_Instance())
	, m_pTimeMgr(CTimeMgr::Get_Instance())
{
	ZeroMemory(&m_tUnit, sizeof(UNIT_INFO));

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


CUI::~CUI()
{
}



HRESULT CUI::LateInitialize()
{
	if (!m_bIsInit)
	{
		this->LateInitialize();
		m_bIsInit = true;
	}

	return S_OK;
}

void CUI::Release()
{
}

void CUI::UpdateRect()
{
	m_tRect.left = LONG(m_tUnit.vPos.x - (m_tUnit.vLength.x * 0.5f));
	m_tRect.top = LONG(m_tUnit.vPos.y - (m_tUnit.vLength.y * 0.5f));
	m_tRect.right = LONG(m_tUnit.vPos.x + (m_tUnit.vLength.x * 0.5f));
	m_tRect.bottom = LONG(m_tUnit.vPos.y + (m_tUnit.vLength.y * 0.5f));
}

void CUI::FrameMove(float _AniPlaySpeed)
{
	m_tFrame.fCurFrame += m_tFrame.fFrameCnt*m_pTimeMgr->GetDelta()*_AniPlaySpeed;
	if (m_tFrame.fCurFrame >= m_tFrame.fFrameCnt)
		m_tFrame.fCurFrame = 0.f;
}

void CUI::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x,
		m_tUnit.vPos.y, 0.f);

	m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matTrans;
}
