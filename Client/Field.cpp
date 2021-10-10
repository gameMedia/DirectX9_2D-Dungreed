#include "stdafx.h"
#include "Field.h"


CField::CField()
	:m_pDeviceMgr(CDeviceMgr::Get_Instance())
	,m_pTextureMgr(CTextureMgr::Get_Instance())
	,m_pTimeMgr(CTimeMgr::Get_Instance())
	,m_bIsInit(false)
{
	ZeroMemory(&m_tMap, sizeof(UNIT_INFO));

	// ������� �ʱ�ȭ
	// ������� = ������ * ���� * �̵� * ���� * �θ�
	D3DXMatrixIdentity(&(m_tMap.matScale)); // ������ ���
	D3DXMatrixIdentity(&(m_tMap.matRotZ)); // ���� ���
	D3DXMatrixIdentity(&(m_tMap.matTrans));  // �̵� ���
	D3DXMatrixIdentity(&(m_tMap.matPlanetZ)); // ���� ���
	D3DXMatrixIdentity(&(m_tMap.matParent)); // �θ� ���
	D3DXMatrixIdentity(&(m_tMap.matWorld));  // ���� ���

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
