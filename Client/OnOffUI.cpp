#include "stdafx.h"
#include "OnOffUI.h"


COnOffUI::COnOffUI()
	:m_bIsInit(false)
	, m_bIsDead(false)
{
	ZeroMemory(&m_tUnit, sizeof(UNIT_INFO));

	// ������� �ʱ�ȭ
	// ������� = ������ * ���� * �̵� * ���� * �θ�
	D3DXMatrixIdentity(&(m_tUnit.matScale)); // ������ ���
	D3DXMatrixIdentity(&(m_tUnit.matRotZ)); // ���� ���
	D3DXMatrixIdentity(&(m_tUnit.matTrans));  // �̵� ���
	D3DXMatrixIdentity(&(m_tUnit.matPlanetZ)); // ���� ���
	D3DXMatrixIdentity(&(m_tUnit.matParent)); // �θ� ���
	D3DXMatrixIdentity(&(m_tUnit.matWorld));  // ���� ���

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 1.f;
}

COnOffUI::~COnOffUI()
{
}


HRESULT COnOffUI::LateInitialize()
{
	if (!m_bIsInit)
	{
		this->LateInitialize();
		m_bIsInit = true;
	}

	return S_OK;
}

void COnOffUI::Release()
{
}

void COnOffUI::UpdateRect()
{
	m_tRect.left = LONG(m_tUnit.vPos.x - (m_tUnit.vLength.x * 0.5f));
	m_tRect.top = LONG(m_tUnit.vPos.y - (m_tUnit.vLength.y * 0.5f));
	m_tRect.right = LONG(m_tUnit.vPos.x + (m_tUnit.vLength.x * 0.5f));
	m_tRect.bottom = LONG(m_tUnit.vPos.y + (m_tUnit.vLength.y * 0.5f));
}

void COnOffUI::FrameMove(float _AniPlaySpeed)
{
	m_tFrame.fCurFrame += m_tFrame.fFrameCnt*m_pTimeMgr->GetDelta()*_AniPlaySpeed;
	if (m_tFrame.fCurFrame >= m_tFrame.fFrameCnt)
		m_tFrame.fCurFrame = 0.f;
}

void COnOffUI::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x,
		m_tUnit.vPos.y, 0.f);

	m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matTrans;
}
