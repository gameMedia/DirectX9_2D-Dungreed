#include "stdafx.h"
#include "BomerangAct.h"
#include "MainHand.h"

CBomerangAct::CBomerangAct()
	:m_tReturn(false)
	, m_bAttackCheck(false)
{
	m_fAttackPower = 10.6f;
	m_fAttackTotal = 0.f;
	CreateStopTime = 0.f;
	DeleteTime = 0.f;
}


CBomerangAct::~CBomerangAct()
{
}

const UNIT_INFO & CBomerangAct::GetInfo() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_tUnit;
}

void CBomerangAct::SetAngle(const float & Angle)
{
	m_fAngle = Angle;
}

void CBomerangAct::SetPos(const D3DXVECTOR3 & vPos)
{
	m_tItemData.vPos = vPos;
}

void CBomerangAct::SetData(const ITEM_DATA & _Main)
{
	m_tItemData = _Main;
}

void CBomerangAct::SetDir(const D3DXVECTOR3 & vDir)
{
	GoDir = vDir; // 가야할 방향
}

void CBomerangAct::RectColl(CObj * pObj)
{
	if (m_tReturn == true)
	{
		m_bIsDead = true;// 죽여
	}
}

int CBomerangAct::Update()
{
	CObj::LateInitialize();
	UpdateRect();

	MakeWorldMatrix();
	MoveBomerang();
	//MakeLine();

	if (m_tReturn == true)
	{
		DeleteTime += m_pTimeMgr->GetDelta();
		if (DeleteTime > 0.5f)
		{
			if (0.f >= m_tItemData.vPos.x - CScrollMgr::GetScrollPos(0) ||
				(float)WINCX <= m_tItemData.vPos.x - CScrollMgr::GetScrollPos(0) ||
				0.f >= m_tItemData.vPos.y - CScrollMgr::GetScrollPos(1) ||
				(float)WINCY <= m_tItemData.vPos.y - CScrollMgr::GetScrollPos(1))
			{
				m_bIsDead = true;
				/*return OBJ_DEAD;*/
			}
		}
	}

	if (m_bIsDead == true)
	{
		dynamic_cast<CMainHand*>(CObjMgr::Get_Instance()->Get_MainHand())->Set_AttackBomerang(false);
		return OBJ_DEAD;
	}

	return OBJ_NOEVENT;
}

void CBomerangAct::LateUpdate()
{

	FrameMove(10.f);
}

void CBomerangAct::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tItemData.Texture.wstrObjectKey,
		m_tUnit.Texture.wstrStateKey,
		(int)m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);

	

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tItemData.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	//CDeviceMgr::Get_Instance()->GetSprite()->End();



	//CDeviceMgr::Get_Instance()->GetLine()->SetWidth(5.f);
	//CDeviceMgr::Get_Instance()->GetLine()->Draw(m_vOrigin, 5, D3DCOLOR_ARGB(255, 0, 255, 0));

	//CDeviceMgr::Get_Instance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

HRESULT CBomerangAct::Initialize()
{
	
	// 부메랑의 공격력
	m_tInfo.iAtt = 10;
	m_tUnit.Texture.wstrStateKey = L"BomerangAttack";
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 2.f;
	return S_OK;
}

HRESULT CBomerangAct::LateInitialize()
{
	
	m_tItemData.vLength = { 150.f,150.f,0.f };
	m_tItemData.vSize = { 1.f,1.f,0.f };
	m_tItemData.Texture.wstrObjectKey = L"Bomerang";
	m_tItemData.Texture.wstrStateKey = L"BomerangAttack";


	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tItemData.Texture.wstrObjectKey,
		m_tItemData.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);


	m_tItemData.vSize.x = m_tItemData.vLength.x / pTexInfo->tImgInfo.Width;
	m_tItemData.vSize.y = m_tItemData.vLength.y / pTexInfo->tImgInfo.Height;

	return S_OK;
}

void CBomerangAct::Release()
{
}

void CBomerangAct::MoveBomerang()
{
	if (m_bAttackCheck == false)
	{
		Current_tick = clock();
		m_bAttackCheck = true;
		
	}
	if (m_bAttackCheck == true)
	{
		Next_tick = ((clock() - Current_tick) + 100) / 100;
		m_fAttackTotal = (m_fAttackPower*Next_tick) - (GRAVITY*Next_tick*Next_tick*0.5f);
		if (m_fAttackTotal <= 0)
		{
			CreateStopTime += m_pTimeMgr->GetDelta();
			if (CreateStopTime > 0.2f)
			{
				m_tReturn = true;
				//m_bAttackCheck = false;
			}
		}
		else
		{
			m_tItemData.vPos += GoDir * m_fAttackTotal;
		}
	}
	if (m_tReturn == true)
	{// 부메랑이 돌아오기 시작하면
		m_tItemData.vPos += (-GoDir)* 1200.f*m_pTimeMgr->GetDelta();
	}
}

void CBomerangAct::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tItemData.matScale, m_tItemData.vSize.x, m_tItemData.vSize.y, 0.f);


	D3DXMatrixTranslation(&m_tItemData.matTrans,
		m_tItemData.vPos.x - CScrollMgr::GetScrollPos(0),
		m_tItemData.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);
	//D3DXMatrixRotationZ(&m_tUnit.matPlanetZ, D3DXToRadian(-m_fAngle)); // 공전
	


	m_tItemData.matWorld = m_tItemData.matScale /** m_tUnit.matPlanetZ*/ * m_tItemData.matTrans;
}

void CBomerangAct::MakeLine()
{
	m_vOrigin[0] = {
		(-m_tItemData.vLength.x*0.5f) + m_tItemData.vPos.x - CScrollMgr::GetScrollPos(0),
		(-m_tItemData.vLength.y*0.5f) + m_tItemData.vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[1] = {
		(m_tItemData.vLength.x*0.5f) + m_tItemData.vPos.x - CScrollMgr::GetScrollPos(0),
		(-m_tItemData.vLength.y*0.5f) + m_tItemData.vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[2] = {
		(m_tItemData.vLength.x*0.5f) + m_tItemData.vPos.x - CScrollMgr::GetScrollPos(0),
		(m_tItemData.vLength.y*0.5f) + m_tItemData.vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[3] = {
		(-m_tItemData.vLength.x*0.5f) + m_tItemData.vPos.x - CScrollMgr::GetScrollPos(0),
		(m_tItemData.vLength.y*0.5f) + m_tItemData.vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[4] = {
		(-m_tItemData.vLength.x*0.5f) + m_tItemData.vPos.x - CScrollMgr::GetScrollPos(0),
		(-m_tItemData.vLength.y*0.5f) + m_tItemData.vPos.y - CScrollMgr::GetScrollPos(1) };
	
}

void CBomerangAct::UpdateRect()
{
	m_tRect.left = LONG(m_tItemData.vPos.x - (m_tItemData.vLength.x * 0.5f));
	m_tRect.top = LONG(m_tItemData.vPos.y - (m_tItemData.vLength.y * 0.5f));
	m_tRect.right = LONG(m_tItemData.vPos.x + (m_tItemData.vLength.x * 0.5f));
	m_tRect.bottom = LONG(m_tItemData.vPos.y + (m_tItemData.vLength.y * 0.5f));
}

CBomerangAct * CBomerangAct::Create(
	const D3DXVECTOR3 Dir, ITEM_DATA _Main)
{
	CBomerangAct* m_pInstance = new CBomerangAct;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}

	
	m_pInstance->SetDir(Dir);
	m_pInstance->SetData(_Main);
	return m_pInstance;
}
