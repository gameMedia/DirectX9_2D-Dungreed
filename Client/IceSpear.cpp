#include "stdafx.h"
#include "IceSpear.h"


CIceSpear::CIceSpear()
{
	CreateTimeSpear = 0.f;
	EndTimeSpear = 3.f;
	WarningLineWidth = 100.f;
	InitOneTIme = false;
}


CIceSpear::~CIceSpear()
{
	Release();
}

const UNIT_INFO & CIceSpear::GetInfo() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_tUnit;
}

void CIceSpear::SetAngle(const float & Angle)
{
	m_fAngle = Angle;
}

void CIceSpear::SetPos(const D3DXVECTOR3 & vPos)
{
	m_tUnit.vPos = vPos;
}

void CIceSpear::SetDir(const D3DXVECTOR3 & vDir)
{
	m_tUnit.vDir = vDir;
}

void CIceSpear::SetParent(const D3DXMATRIX & _Parent)
{
	m_tUnit.matParent = _Parent;
}

void CIceSpear::RectColl(CObj * pObj)
{
	m_tInfo.iHp -= 6.f; // 죽어라
}

int CIceSpear::Update()
{
	CObj::LateInitialize();
	CObj::UpdateRect();
	if (m_bIsDead == true)
	{
		
		return OBJ_DEAD;
	}
	if (m_tUnit.Texture.wstrStateKey == L"Create")
	{
		if (m_tFrame.fCurFrame >= m_tFrame.fFrameCnt - 1.f)
		{
			m_tUnit.Texture.wstrStateKey = L"Ilde";
			m_tFrame.fFrameCnt = 0.f;
		}
	}
	if (m_tUnit.Texture.wstrStateKey == L"Ilde"
		&& WarningLineWidth <=0.f)
	{
		CreateTimeSpear += m_pTimeMgr->GetDelta();
		if (EndTimeSpear <= CreateTimeSpear)
		{
			if (0.f >= m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0) ||
				(float)WINCX <= m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0))
			{
				m_bIsDead = true;
				/*return OBJ_DEAD;*/
			}
			
		}
		MoveBullet();
	}

	MakeWorldMatrix();

	return OBJ_NOEVENT;
}

void CIceSpear::LateUpdate()
{
	FrameMove(3.f);
	if (MakeLinettt == true)
	{
		MakeLine();
	}
}

void CIceSpear::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey,
		m_tUnit.Texture.wstrStateKey,
		(int)m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);

	CDeviceMgr::Get_Instance()->GetSprite()->End();



	CDeviceMgr::Get_Instance()->GetLine()->SetWidth(WarningLineWidth);
	CDeviceMgr::Get_Instance()->GetLine()->Draw(WarningLine, 2, D3DCOLOR_ARGB(255, 255, 0, 0));

	CDeviceMgr::Get_Instance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	if (WarningLineWidth > 0.f)
	{
		--WarningLineWidth;
		--WarningLineWidth;
		--WarningLineWidth;
	}
	if (WarningLineWidth <= 0.f)
	{
		WarningLineWidth = 0.f;
		WarningLine[0] = { 5000 - CScrollMgr::GetScrollPos(0),
			m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
		WarningLine[1] = { 5001.f - CScrollMgr::GetScrollPos(0),
			m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
		MakeLinettt = false;
	}

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tUnit.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CIceSpear::Initialize()
{
	m_tUnit.vLook = { 0.f,-1.f,0.f };
	m_tUnit.vLength = { 300.f,50.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.vPos = { 0.f,0.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"IceSpear";
	m_tUnit.Texture.wstrStateKey = L"Create";

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 13.f;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey,
		m_tUnit.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);


	m_tUnit.vSize.x = m_tUnit.vLength.x / pTexInfo->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / pTexInfo->tImgInfo.Height;

	// 보스가 쏘는 얼음창의 정보
	m_tInfo.iAtt = 13.f;
	m_tInfo.iHp = 4.f;
	m_tInfo.iGold = 0;
	MakeLinettt = true;

	return S_OK;
}

HRESULT CIceSpear::LateInitialize()
{
	return S_OK;
}

void CIceSpear::Release()
{
}

void CIceSpear::MoveBullet()
{
	m_tUnit.vPos += m_tUnit.vDir * 2000.f * m_pTimeMgr->GetDelta();
}

void CIceSpear::MakeWorldMatrix()
{
	if (InitOneTIme == false)
	{
		if (m_tUnit.vDir.x < 0)
		{
			m_tUnit.vSize.x *= -1.f;
		}
		InitOneTIme = true;
	}

	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);


	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);

	m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matTrans;
}

void CIceSpear::MakeLine()
{
	WarningLine[0] = { 0 - CScrollMgr::GetScrollPos(0),
	m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
	WarningLine[1] = { 2100.f - CScrollMgr::GetScrollPos(0),
		m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
}

CIceSpear * CIceSpear::Create(
	const D3DXVECTOR3 vDir, const D3DXVECTOR3 vPos)
{

	CIceSpear* pInstance = new CIceSpear;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}


	pInstance->SetDir(vDir);
	pInstance->SetPos(vPos);

	return pInstance;
}
