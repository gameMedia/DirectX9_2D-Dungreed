#include "stdafx.h"
#include "MyMoney.h"
#include "Player.h"

CMyMoney::CMyMoney()
	:m_bJump(true)
	, iJumpPower(6.f)
	, m_bJumpCheck(false)
	, JumpStop(false)
{
}


CMyMoney::~CMyMoney()
{
	Release();
}

int CMyMoney::Update()
{
	CObj::MakeWorldMatrix();
	UpdateRect();
	GravityTile();
	float fY = 0.f;
	bool bIsColl = CLineMgr::Get_Instance()->LineCollision((&m_tUnit.vPos), &fY);


	if (m_bJumpCheck == false)
	{
		Current_tick = clock();
		m_bJumpCheck = true;
		JumpStop = false;
	}
	if (m_bJumpCheck == true && m_bJump == true)
	{
		Next_tick = ((clock() - Current_tick) + 100) / 100;
		JumpTotal = (iJumpPower*Next_tick) - (GRAVITY*Next_tick*Next_tick*0.5f);
		m_tUnit.vPos.y -= JumpTotal;

		if (bIsColl && m_tUnit.vPos.y > fY)
		{
			m_bJump = false;
			m_tUnit.vPos.y = fY;

		}
		else if (JumpStop == true)
		{
			m_bJump = false;
		}
	}

	if (m_bIsDead == true)
	{ // 씬이 넘거갈때 or 플레이어가 먹었을시 죽는다
		CPlayer::Money += m_tInfo.iGold;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::GETMONEY);
		CSoundMgr::Get_Instance()->PlaySound(L"MyDropMoney.wav", CSoundMgr::GETMONEY);
		return OBJ_DEAD;
	}



	return 0;
}

void CMyMoney::LateUpdate()
{
	CObj::FrameMove(1.f);
}

void CMyMoney::Render()
{
	const TEX_INFO* FireSword = nullptr;
	FireSword = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);
	NULL_CHECK(FireSword);

	float CenterX = FireSword->tImgInfo.Width * 0.5f;
	float CenterY = FireSword->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		FireSword->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CMyMoney::Initialize()
{
	m_tUnit.vPos = { 650.f,300.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"Money";
	m_tUnit.Texture.wstrStateKey = L"Bullion";
	m_tUnit.Texture.iIndex = 0;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 7.f;

	m_tUnit.vLength = { 60.f,27.f,0.f };
	D3DXVECTOR3 vRenderLength = { 60.f, 27.f,0.f };
	const TEX_INFO* NormalSword = nullptr;
	NormalSword = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);

	m_tUnit.vSize.x = vRenderLength.x / NormalSword->tImgInfo.Width;
	m_tUnit.vSize.y = vRenderLength.y / NormalSword->tImgInfo.Height;

	// item 정보
	
	m_tInfo.iAtt = 0;
	m_tInfo.iGold = 50;

	return S_OK;
}

void CMyMoney::Release()
{
}

void CMyMoney::RectColl(CObj * pObj)
{
	m_bIsDead = true;
}

void CMyMoney::GravityLine()
{
}

void CMyMoney::GravityTile()
{
	if (m_bColl == true)
	{// 만약 타일과 충돌중이라면
		Current_tick = clock();

	}
}

void CMyMoney::UpdateRect()
{
	m_tRect.left = LONG(m_tUnit.vPos.x - (m_tUnit.vLength.x * 0.5f));
	m_tRect.top = LONG(m_tUnit.vPos.y - (m_tUnit.vLength.y * 1.f));
	m_tRect.right = LONG(m_tUnit.vPos.x + (m_tUnit.vLength.x * 0.5f));
	m_tRect.bottom = LONG(m_tUnit.vPos.y + (m_tUnit.vLength.y * 1.f));
}

CMyMoney * CMyMoney::Create()
{
	CMyMoney* m_pInstance = new CMyMoney;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
