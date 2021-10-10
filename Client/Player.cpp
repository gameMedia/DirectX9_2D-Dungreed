#include "stdafx.h"
#include "Player.h"

#include "StateMgr.h"
#include "AbstractFactory.h"
#include "KeyMgr.h"

#include "PlayerAct.h" // 플레이어의 행동을 제어하는 Act들을 담아
#include "PlayerJump.h"
#include "PlayerDownJump.h"
#include "PlayerMoveLeft.h"
#include "PlayerMoveRight.h"
#include "PlayerLookMouse.h"
#include "PlayerDash.h"
#include "MainHand.h"
#include "SubHand.h"

#include "WarningEffect.h"
#include "AnimationImpl.h"

#include "Inventory.h"
#include "BanshBullet.h"
#include "PlayerHpBar.h"

#include "DamageEffect.h"
#include "DamageEffectImpl.h"
#include "PlayerHungryGauge.h"
#include "PlayerDashCount.h"

int CPlayer::Money = 0; // 플레이어의 돈
int CPlayer::Hungry = 0; //포만도
int CPlayer::HungryFull = 5;
int CPlayer::PlusAtt = 0; // 추가 공격력
int CPlayer::DashCount = 6;
int CPlayer::DashFullCount = 6;
CPlayer::CPlayer()
	:m_bJump(false)
	, m_bAtt(false)
	, m_bDownJump(false)
	, m_bDash(false)
	, HitCheck(false)
{
	Red = 255;
	Green = 255;
	Blue = 255;
	CreateTimeReset = 0.f;
	EndTimeReset = 0.1f;
	EndRegenTime = 1.f;
	CreateRegenTime = 0.f;
	CreateRegenDash = 0.f;
	EndRegenDash = 1.f;
}


CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize()
{


	HRESULT hr = COnOffUIMgr::Get_Instance()->Add_Object(ONOFFUIID::ONINVENTORY, CAbstractFactory<CInventory>::CreateUI());

	m_eCurState = IDLE;
	m_eNextState = IDLE;
	ACT_OK = false;

	m_fSpeed = Player_Speed;
	m_fAngle = 0.f;

	m_tUnit.vPos = { 400.f,300.f,0.f };
	m_tUnit.vLength = { 100.f,100.f,0.f };
	m_tUnit.vCollPos = { 400.f,m_tUnit.vPos.y + m_tUnit.vLength.y*0.5f,0.f };
	m_tUnit.vLook = { 1.f,0.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"Normal";
	m_tUnit.Texture.wstrStateKey = L"CharIdle";


	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 4.f;
	m_tBat.fCurFrame = 0.f;
	m_tBat.fFrameCnt = 6.f;
	FrameSpeed = 1.f;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);


	m_tUnit.vSize.x = m_tUnit.vLength.x / pTexInfo->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / pTexInfo->tImgInfo.Height;

	Current_tick = clock();
	Next_tick = (clock_t)0.f;
	ScrollSpeed = 0.f;


	ActCheck = false;

	// 행동
	PlayMoveLeft = CAbstractFactory<CPlayerMoveLeft>::CreateAct();
	PlayMoveLeft->Set_Player(this);

	PlayMoveRight = CAbstractFactory<CPlayerMoveRight>::CreateAct();
	PlayMoveRight->Set_Player(this);

	PlayJump = CAbstractFactory<CPlayerJump>::CreateAct();
	PlayJump->Set_Player(this);

	PlayDownJump = CAbstractFactory<CPlayerDownJump>::CreateAct();
	PlayDownJump->Set_Player(this);

	PlayLookMouse = CAbstractFactory<CPlayerLookMouse>::CreateAct();
	PlayLookMouse->Set_Player(this);

	PlayerDash = CAbstractFactory<CPlayerDash>::CreateAct();
	PlayerDash->Set_Player(this);

	ChangeStateKey = L""; // 스테이트 키를 바꾸자

	CObj* pHand = CAbstractFactory<CMainHand>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MAINHAND, pHand);
	dynamic_cast<CMainHand*>(pHand)->Set_Player(this);

	CObj* pSubHand = CAbstractFactory<CSubHand>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::SUBHAND, pSubHand);
	dynamic_cast<CSubHand*>(pSubHand)->Set_Player(this);

	hr = CUIMgr::Get_Instance()->Add_Object(UIID::LIFEBAR, CAbstractFactory<CPlayerHpBar>::Create());
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = CUIMgr::Get_Instance()->Add_Object(UIID::HUNGRYGAUGE, CAbstractFactory<CPlayerHungryGauge>::Create());
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = CUIMgr::Get_Instance()->Add_Object(UIID::DASHGAUGE, CAbstractFactory<CPlayerDashCount>::Create());
	FAILED_CHECK_RETURN(hr, E_FAIL);

	// 플레이어의 정보
	m_tInfo.iMaxHp = 100.f;
	m_tInfo.iHp = 100;

	return S_OK;
}

int CPlayer::Update()
{
	UpdateRect();

	
	GravityTile();
	GravityLine();

	CObj::MakeWorldMatrix();
	PlayerAct();
	StateCheck();
	//cout << "x좌표:" << m_tUnit.vPos.x << "y좌표 : " << m_tUnit.vPos.y << endl;

	return OBJ_NOEVENT;
}

void CPlayer::LateUpdate()
{
	FrameChange();
	CObj::FrameMove(FrameSpeed);
	FrameMove1(1.f);
	OffSet();
	if (ActCheck == false)
	{
		CStateMgr::Get_Instance()->Set_StateMgr(IDLE);
	}
	if (ActCheck == true)
	{
		ActCheck = false;
	}
	if (CKeyMgr::Get_Instance()->KeyDown(KEY_DOWN))
	{
		m_tInfo.iHp -= 5.f;
	}
	if (m_tInfo.iHp <= 1)
	{
		m_tInfo.iHp = 1;
	}
	m_bColl = false; // 충돌처리를 다시 하자

	Artifact();




}

void CPlayer::Render()
{
	UpdateRect();


	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);



	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height*0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, Red, Green, Blue));

	if (Art1.szName == L"BatItem" ||
		Art2.szName == L"BatItem" ||
		Art3.szName == L"BatItem" ||
		Art4.szName == L"BatItem")
	{
		const TEX_INFO* FireSword = nullptr;
		FireSword = CTextureMgr::Get_Instance()->GetTexInfo(
			Bat.Texture.wstrObjectKey,
			Bat.Texture.wstrStateKey, (int)m_tBat.fCurFrame);
		NULL_CHECK(FireSword);

		float CenterX1 = FireSword->tImgInfo.Width * 0.5f;
		float CenterY1 = FireSword->tImgInfo.Height * 0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(Bat.matWorld));
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			FireSword->pTexture, nullptr, &D3DXVECTOR3(CenterX1, CenterY1, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
	// 행동초기화
	CreateTimeReset += CTimeMgr::Get_Instance()->GetDelta();
	if (EndTimeReset <= CreateTimeReset)
	{
		CheckReset();
	}
}

void CPlayer::Release()
{

}

void CPlayer::UpdateRect()
{
	m_tRect.left = LONG(m_tUnit.vPos.x - (m_tUnit.vLength.x * 0.2f));
	m_tRect.top = LONG(m_tUnit.vPos.y - (m_tUnit.vLength.y * 0.1f));
	m_tRect.right = LONG(m_tUnit.vPos.x + (m_tUnit.vLength.x * 0.2f));
	m_tRect.bottom = LONG(m_tUnit.vPos.y + (m_tUnit.vLength.y * 0.5f));

	
}

void CPlayer::Artifact()
{
	Art1 = dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[16];
	Art2 = dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[17];
	Art3 = dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[18];
	Art4 = dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[19];
	
	if (Art1.szName == L"Wing")
	{
		EndRegenDash = 0.3f;
	}
	else if (Art2.szName == L"Wing")
	{
		EndRegenDash = 0.3f;
	}
	else if (Art3.szName == L"Wing")
	{
		EndRegenDash = 0.3f;
	}
	else if (Art4.szName == L"Wing")
	{
		EndRegenDash = 0.3f;
	}


	


	if (Art1.szName == L"BatItem")
	{
		matScale, matTrans, matWorld;
		Bat.vPos = { -20.f,-20.f,0.f };
		Bat.vSize = { 1.f,1.f,0.f };
		Bat.Texture.wstrObjectKey = L"BatItem";
		Bat.Texture.wstrStateKey = L"BatMove";
		Bat.Texture.iIndex = 0;
		

		D3DXMatrixScaling(&Bat.matScale, Bat.vSize.x, Bat.vSize.y, 0.f);
		D3DXMatrixTranslation(&Bat.matTrans,
			Bat.vPos.x ,
			Bat.vPos.y , 0.f);

		Bat.matWorld = Bat.matScale * Bat.matTrans*m_tUnit.matWorld;

	}
	if (Art2.szName == L"BatItem")
	{
		matScale, matTrans, matWorld;
		Bat.vPos = { -20.f,-20.f,0.f };
		Bat.vSize = { 1.f,1.f,0.f };
		Bat.Texture.wstrObjectKey = L"BatItem";
		Bat.Texture.wstrStateKey = L"BatMove";
		Bat.Texture.iIndex = 0;


		D3DXMatrixScaling(&Bat.matScale, Bat.vSize.x, Bat.vSize.y, 0.f);
		D3DXMatrixTranslation(&Bat.matTrans,
			Bat.vPos.x,
			Bat.vPos.y, 0.f);

		Bat.matWorld = Bat.matScale * Bat.matTrans*m_tUnit.matWorld;
	}
	if (Art3.szName == L"BatItem")
	{
		matScale, matTrans, matWorld;
		Bat.vPos = { -20.f,-20.f,0.f };
		Bat.vSize = { 1.f,1.f,0.f };
		Bat.Texture.wstrObjectKey = L"BatItem";
		Bat.Texture.wstrStateKey = L"BatMove";
		Bat.Texture.iIndex = 0;


		D3DXMatrixScaling(&Bat.matScale, Bat.vSize.x, Bat.vSize.y, 0.f);
		D3DXMatrixTranslation(&Bat.matTrans,
			Bat.vPos.x,
			Bat.vPos.y, 0.f);

		Bat.matWorld = Bat.matScale * Bat.matTrans*m_tUnit.matWorld;
	}
	if (Art4.szName == L"BatItem")
	{
		matScale, matTrans, matWorld;
		Bat.vPos = { -20.f,-20.f,0.f };
		Bat.vSize = { 1.f,1.f,0.f };
		Bat.Texture.wstrObjectKey = L"BatItem";
		Bat.Texture.wstrStateKey = L"BatMove";
		Bat.Texture.iIndex = 0;


		D3DXMatrixScaling(&Bat.matScale, Bat.vSize.x, Bat.vSize.y, 0.f);
		D3DXMatrixTranslation(&Bat.matTrans,
			Bat.vPos.x,
			Bat.vPos.y, 0.f);

		Bat.matWorld = Bat.matScale * Bat.matTrans*m_tUnit.matWorld;
	}


	CreateRegenTime += m_pTimeMgr->GetDelta();
	if (EndRegenTime <= CreateRegenTime)
	{
		{// 리젠링이냐
			if (Art1.szName == L"RegenRing")
			{
				if (m_tInfo.iHp < m_tInfo.iMaxHp)
				{
					m_tInfo.iHp += 1;
				}
			}
			if (Art2.szName == L"RegenRing")
			{
				if (m_tInfo.iHp < m_tInfo.iMaxHp)
				{
					m_tInfo.iHp += 1;
				}

			}
			if (Art3.szName == L"RegenRing")
			{
				if (m_tInfo.iHp < m_tInfo.iMaxHp)
				{
					m_tInfo.iHp += 1;
				}

			}
			if (Art4.szName == L"RegenRing")
			{
				if (m_tInfo.iHp < m_tInfo.iMaxHp)
				{
					m_tInfo.iHp += 1;
				}

			}
			if (m_tInfo.iHp > m_tInfo.iMaxHp)
			{
				m_tInfo.iHp = m_tInfo.iMaxHp;
			}
		}
		CreateRegenTime = 0.f;

	}

	CreateRegenDash += m_pTimeMgr->GetDelta();
	if (EndRegenDash <= CreateRegenDash)
	{
		if (CPlayer::DashCount < CPlayer::DashFullCount)
		{
			++DashCount;
		}
		CreateRegenDash = 0.f;
	}

}

void CPlayer::FrameMove1(float _aniSpeed)
{
	m_tBat.fCurFrame += m_tBat.fFrameCnt*m_pTimeMgr->GetDelta()*_aniSpeed;
	if (m_tBat.fCurFrame >= m_tBat.fFrameCnt)
		m_tBat.fCurFrame = 0.f;
}



void CPlayer::PlayerAct()
{
	PlayLookMouse->Execute(Current_tick); // 마우스 방향 바라보기

	if (m_bAtt == true)
	{
		AttStart();
	}
	if (m_bJump == true)
	{
		//Current_tick = clock(); // 중력 초기화?
		PlayJump->Execute(Current_tick);
		TotalJump = dynamic_cast<CPlayerJump*>(PlayJump)->Get_JumpTotal();
		dynamic_cast<CPlayerDash*>(PlayerDash)->Set_DashStop(true);
		Current_tick = clock();
	}
	if (m_bDownJump == true)
	{
		PlayDownJump->Execute(Current_tick);
	}
	if (m_bDash == true)
	{

		PlayerDash->Execute(Current_tick);
		dynamic_cast<CPlayerJump*>(PlayJump)->Set_JumpStop(true);
		dynamic_cast<CPlayerDownJump*>(PlayDownJump)->Set_JumpStop(true);
	}
}



void CPlayer::GravityLine()
{
	// 중력 처리를 해보자
	float fY = 0.f;
	bool bIsColl = CLineMgr::Get_Instance()->LineCollisionMove((&m_tUnit.vCollPos), &fY);


	if (Dir != UP
		|| CStateMgr::Get_Instance()->Get_eCurState() != DASH)
	{
		Next_tick = ((clock() - Current_tick) + 90) / 90;

		if (bIsColl)
		{// 만약 라인과 충돌 중이라면 플레이어 중력을 초기화 해줘야한다.
			Current_tick = clock();

		}
		else if (bIsColl == false)
		{
			ScrollSpeed = (0.3f*Next_tick*Next_tick*0.5f);
			m_tUnit.vPos.y += ScrollSpeed;
			m_tUnit.vCollPos.y += ScrollSpeed;
		}
	}
}

void CPlayer::GravityTile()
{
	if (m_bColl == true)
	{// 만약 타일과 충돌 중이라면
		Current_tick = clock();
		dynamic_cast<CPlayerJump*>(PlayJump)->Set_JumpStop(true);
	}
}

void CPlayer::StateCheck()
{

	switch (m_eCurState)
	{
	case STATE::IDLE:
		Idle();
		break;
	case STATE::MOVE:
		break;
	case STATE::ATT:
		break;
	case STATE::HIT:
		break;
	case STATE::STUN:
		break;
	default:
		break;
	}
}

void CPlayer::FrameChange()
{
	m_eNextState = CStateMgr::Get_Instance()->Get_eCurState();
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case STATE::IDLE:
			m_tUnit.Texture.wstrStateKey = L"CharIdle";
			m_tFrame.fFrameCnt = 4.f;
			FrameSpeed = 1.f;
			break;
		case STATE::MOVE:
			m_tUnit.Texture.wstrStateKey = L"CharRun";
			m_tFrame.fFrameCnt = 8.f;
			FrameSpeed = 1.4f;
			break;
		case STATE::ATT:
			break;
		case STATE::HIT:
			break;
		case STATE::STUN:
			break;
		case STATE::JUMP:
			m_tUnit.Texture.wstrStateKey = L"CharJump";
			m_tFrame.fFrameCnt = 1.f;
			FrameSpeed = 1.f;
			break;
		case STATE::DASH:
			m_tUnit.Texture.wstrStateKey = L"CharJump";
			m_tFrame.fFrameCnt = 1.f;
			FrameSpeed = 1.f;
			break;
		case STATE::END:
			break;
		default:
			break;
		}
		m_eCurState = m_eNextState;
	}
}

void CPlayer::Idle()
{

}

void CPlayer::Move()
{
}

void CPlayer::Jump()
{
}

void CPlayer::CheckReset()
{
	HitCheck = false; // 다시 쳐맞기
	Red = 255;
	Green = 255;
	Blue = 255;
	CreateTimeReset = 0.f;
}

void CPlayer::ActStart(int _State, int _Dir)
{
	Dir = _Dir;
	ACT_OK = CStateMgr::Get_Instance()->StateCheck(_State);
	if (_State == MOVE&&ACT_OK == true)
	{
		if (_Dir == LEFT)
		{// 왼쪽

			PlayMoveLeft->Execute(Current_tick);
		}
		if (_Dir == RIGHT)
		{// 오른쪽
			PlayMoveRight->Execute(Current_tick);
		}
	}

	if (_State == ATT&&ACT_OK == true)
	{
		m_bAtt = true;
	}
	if (_State == JUMP&&ACT_OK == true)
	{
		if (_Dir == DOWN)
		{// 아래 점프
			m_bDownJump = true;
		}
		else if (_Dir == UP)
		{// 일상 점프
			m_bJump = true;

		}
	}

	if (_State == DASH&&ACT_OK == true&&CPlayer::DashCount>0)
	{
		m_bDash = true;
		Current_tick = clock();
		dynamic_cast<CPlayerDash*>(PlayerDash)->Set_DashStop(true);

	}
	ActCheck = true;

}

void CPlayer::AttStart()
{

}



void CPlayer::RectColl(CObj * pObj)
{
	if (HitCheck == false)
	{// 만약 충돌 전이면
		// 보조 무기 정보 가져와서
		ITEM_DATA SubItem = dynamic_cast<CSubHand*>(CObjMgr::Get_Instance()->Get_SubHand())->Get_Main();
		int Damage = pObj->Get_UnitData().iAtt- SubItem.iBlock;
		if (Damage >= 1)
		{
			m_tInfo.iHp -= Damage; //Hp를 차감해준다.
			CDamageEffectImpl* pDamageImpl = CDamageEffectImpl::Create(
				L"Font", L"DamageFont", Damage);

			CDamageEffect* pDamageEffect = CDamageEffect::Create(pDamageImpl, m_tUnit.vPos, D3DXVECTOR3(20.f, -10.f, 0.f));
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pDamageEffect);
			
			
			D3DXVECTOR3 HitPos = { WINCX*0.5f,WINCY*0.5f,0.f };
			D3DXVECTOR3 HitSize = { 2.f,2.f,0.f };
			CAnimationImpl* pImpl = CAnimationImpl::Create(
				L"UI", L"NormalHit", FRAME(0.f, 1.f), 1.f);

			CWarningEffect* pEffect = CWarningEffect::Create(pImpl, HitPos, HitSize);

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYERHIT);
			CSoundMgr::Get_Instance()->PlaySound(L"MyHitPlayer.wav", CSoundMgr::PLAYERHIT);

			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);

			Red = 255;
			Green = 0;
			Blue = 0;
			HitCheck = true;
		}
	}
}

void CPlayer::OffSet()
{

	int iScrollX = (int)CScrollMgr::GetScrollPos(0);
	int iScrollY = (int)CScrollMgr::GetScrollPos(1);
	int iOffset_X = WINCX / 2;
	int iOffset_Y = WINCY / 2;

	if (iOffset_X + 50 + iScrollX < m_tUnit.vPos.x)
	{
		CScrollMgr::SetScrollPos({ m_fSpeed,0.f,0.f });
	}
	else if (iOffset_X - 50 + iScrollX > m_tUnit.vPos.x)
	{
		CScrollMgr::SetScrollPos({ -m_fSpeed,0.f,0.f });
	}

	if (iOffset_Y + 50 + iScrollY < m_tUnit.vPos.y)
	{
		CScrollMgr::SetScrollPos({ 0.f,ScrollSpeed,0.f });
	}
	else if (iOffset_Y - 50 + iScrollY > m_tUnit.vPos.y)
	{
		CScrollMgr::SetScrollPos({ 0.f,-m_fSpeed * 2,0.f });
	}
}

CPlayer * CPlayer::Create()
{
	CPlayer* m_pInstance = new CPlayer;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}

	return m_pInstance;
}
