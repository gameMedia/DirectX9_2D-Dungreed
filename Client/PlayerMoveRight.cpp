#include "stdafx.h"
#include "PlayerMoveRight.h"
#include "StateMgr.h"
#include "AnimationImpl.h"
#include "DashEffect.h"
CPlayerMoveRight::CPlayerMoveRight()
{
}


CPlayerMoveRight::~CPlayerMoveRight()
{
}

void CPlayerMoveRight::Initialize()
{
	MoveSpeed = Player_Speed;
	CreateTimeAfterImage = 0.f;
	EndTimeAfterImage = 0.1f;
}

void CPlayerMoveRight::Execute(clock_t& _currentTick)
{
	UNIT_INFO& playerPos = m_pPlayer->Get_Pos();

	float fY = 0.f;
	int CheckState = CStateMgr::Get_Instance()->Get_eCurState();
	bool bIsColl = CLineMgr::Get_Instance()->LineCollisionMove((&playerPos.vCollPos), &fY);

	playerPos.vPos.x += MoveSpeed;
	playerPos.vCollPos.x += MoveSpeed;
	//Offset();
	if (bIsColl&&CheckState != JUMP)
	{
		playerPos.vCollPos.y = fY;
		playerPos.vPos.y = playerPos.vCollPos.y-50.f;
		CreateTimeAfterImage += CTimeMgr::Get_Instance()->GetDelta();
		if (EndTimeAfterImage <= CreateTimeAfterImage) // 대쉬 이펙트 생성
		{
			D3DXVECTOR3 DustPos = { playerPos.vCollPos.x - 30.f,playerPos.vCollPos.y - 10.f,0.f };
			D3DXVECTOR3 DustSize = { fabs(playerPos.vSize.x),fabs(playerPos.vSize.y),0.f };
			CAnimationImpl* pImpl = CAnimationImpl::Create(
				L"Player", L"MoveDust", FRAME(0.f, 4.f), 3.f);

			CDashEffect* pEffect = CDashEffect::Create(pImpl, DustPos, DustSize);

			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);
			CreateTimeAfterImage = 0.f;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYERMOVE);
		CSoundMgr::Get_Instance()->PlaySound(L"MyStep.wav", CSoundMgr::PLAYERMOVE);
		}
	}

}

void CPlayerMoveRight::Offset()
{
	UNIT_INFO& playerPos = m_pPlayer->Get_Pos();

	int iScrollX = (int)CScrollMgr::GetScrollPos(0);
	int iScrollY = (int)CScrollMgr::GetScrollPos(1);
	int iOffset_X = WINCX / 2;
	int iOffset_Y = WINCY / 2;

	if (iOffset_X + 50 + iScrollX < playerPos.vPos.x)
	{
		CScrollMgr::SetScrollPos({ MoveSpeed,0.f,0.f });
	}

}
