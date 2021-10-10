#include "stdafx.h"
#include "PlayerDash.h"
#include "Mouse.h"
#include "StateMgr.h"
#include "AlphaAnimationImpl.h"
#include "DashEffect.h"
#include "NoAnimationImpl.h"
#include "NormalEffect.h"
#include "Player.h"

CPlayerDash::CPlayerDash()
{
}


CPlayerDash::~CPlayerDash()
{
}

void CPlayerDash::Initialize()
{
	InitOneTime = false;
	TimeCheck = false;
	DashStop = false;
	CreateTimeAfterImage = 0.f;
	EndTimeAfterImage = 0.07f;
}

void CPlayerDash::Execute(clock_t & _currentTick)
{

	UNIT_INFO& playerPos = m_pPlayer->Get_Pos();
	float fY = 0.f;
	bool bIsColl = CLineMgr::Get_Instance()->LineCollisionDash((&playerPos.vCollPos), &fY);

	if (CPlayer::DashCount < 0)
	{
		m_pPlayer->Set_Dash(false);
		return;
	}

	if (InitOneTime == false&&CPlayer::DashCount > 0)
	{
		D3DXVECTOR3 vLook = { 1.f,0.f,0.f };
		D3DXMATRIX matRotZ;
		Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
		MousevDir.x += CScrollMgr::GetScrollPos(0);
		MousevDir.y += CScrollMgr::GetScrollPos(1);
		D3DXVec3Normalize(&MousevDir, &MousevDir);
		D3DXVec3Normalize(&vLook, &vLook);
		Dir = MousevDir;
		DashPower = 30.f;
		CreateTimeAfterImage = 0.f;
		DashStop = false;
		InitOneTime = true;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYERDASH);
		CSoundMgr::Get_Instance()->PlaySound(L"MyDash.wav", CSoundMgr::PLAYERDASH);
		CPlayer::DashCount -= 1;
	}
	if (InitOneTime == true)
	{
		Total_Dash = (Dir*DashPower); // 대쉬 벡터
		CreateTimeAfterImage += CTimeMgr::Get_Instance()->GetDelta();
		if (bIsColl)
		{
			playerPos.vCollPos.y = fY;
			playerPos.vPos.y = playerPos.vCollPos.y - 50.f;
			Total_Dash.y = 0.f;
		}

		playerPos.vPos += Total_Dash;
		playerPos.vCollPos += Total_Dash;

		if (EndTimeAfterImage <= CreateTimeAfterImage) // 대쉬 이펙트 생성
		{
			CAlphaAnimationImpl* pImpl = CAlphaAnimationImpl::Create(
				L"Normal", L"CharJump", FRAME(0.f, 1.f), 3.f);

			CDashEffect* pEffect = CDashEffect::Create(pImpl, playerPos.vPos, playerPos.vSize);
			
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);
			CreateTimeAfterImage = 0.f;
		}

		if (DashPower < 1.0)
		{
			Dir = {};
			DashPower = 0;
			TimeCheck = false;
			InitOneTime = false;
			m_pPlayer->Set_Dash(false);
			CStateMgr::Get_Instance()->Set_StateMgr(IDLE);
		}
		Lerp(float(0.05f), float(0.9f));

		//if()

		if (DashStop == true)
		{
			Dir = {};
			DashPower = 0;
			TimeCheck = false;
			InitOneTime = false;
			DashStop = false;
			m_pPlayer->Set_Dash(false);
			CStateMgr::Get_Instance()->Set_StateMgr(IDLE);
		}
	}
	Offset();
}

void CPlayerDash::Offset()
{
	UNIT_INFO& playerPos = m_pPlayer->Get_Pos();

	int iScrollX = (int)CScrollMgr::GetScrollPos(0);
	int iScrollY = (int)CScrollMgr::GetScrollPos(1);
	int iOffset_X = WINCX / 2;
	int iOffset_Y = WINCY / 2;
	if (iOffset_X + 50 + iScrollX < playerPos.vPos.x )
	{
	CScrollMgr::SetScrollPos({ Total_Dash.x,0.f,0.f});
	}
	else if (iOffset_X - 50 + iScrollX > playerPos.vPos.x)
	{
	CScrollMgr::SetScrollPos({ Total_Dash.x,0.f,0.f });
	}
	if (iOffset_Y + 50 + iScrollY < playerPos.vPos.y)
	{
		CScrollMgr::SetScrollPos({ 0.f,(Total_Dash.y),0.f });
	}
	else if (iOffset_Y - 50 + iScrollY > playerPos.vPos.y)
	{
		CScrollMgr::SetScrollPos({ 0.f,(Total_Dash.y),0.f });
	}
}

void CPlayerDash::Lerp(float _time, float _ratio)
{
	if (TimeCheck == false)
	{
		Current_tick = clock();
		TimeCheck = true;
	}

	if (TimeCheck == true)
	{
		float mytime = 1000 * _time;
		Next_tick = ((clock() - Current_tick) + (clock_t)mytime) / (clock_t)mytime;

		if (Next_tick > 1)
		{
			Next_tick = 1; // 정해진 시간이 지났으면 초기화
			DashPower *= _ratio;
		}
	}
	
}
