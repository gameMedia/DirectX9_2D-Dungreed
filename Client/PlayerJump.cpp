#include "stdafx.h"
#include "PlayerJump.h"
#include "StateMgr.h"


CPlayerJump::CPlayerJump()
	:SetOneTime(false)
	, JumpStop(false)
{
}


CPlayerJump::~CPlayerJump()
{
}

void CPlayerJump::Initialize()
{
	
	// 나중에 점프 클래스로 옮기자
	m_bJumpCheck = false;
	iJumpPower = 9.f;
}

void CPlayerJump::Execute(clock_t& _currentTick)
{
	UNIT_INFO& playerPos = m_pPlayer->Get_Pos();
	
	float fY = 0.f;
	bool bIsColl = CLineMgr::Get_Instance()->LineCollision((&playerPos.vCollPos), &fY);


		if (m_bJumpCheck == false)
		{
			Current_tick = clock();
			m_bJumpCheck = true;
			JumpStop = false;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYERJUMP);
			CSoundMgr::Get_Instance()->PlaySound(L"MyJump.wav", CSoundMgr::PLAYERJUMP);
		}
		if (m_bJumpCheck == true)
		{

			Next_tick = ((clock() - Current_tick) + 100) / 100;
			JumpTotal = (iJumpPower*Next_tick) - (GRAVITY*Next_tick*Next_tick*0.5f);
			playerPos.vPos.y -= JumpTotal;
			playerPos.vCollPos.y -= JumpTotal;
			CStateMgr::Get_Instance()->Set_StateMgr(JUMP);
			if (bIsColl && playerPos.vCollPos.y > fY)
			{
				m_bJumpCheck = false;
				m_pPlayer->Set_Jump(false);
				SetOneTime = false;
				playerPos.vCollPos.y = fY;
				playerPos.vPos.y = playerPos.vCollPos.y - 50.f;

				CStateMgr::Get_Instance()->Set_StateMgr(IDLE);
			}
			else if (JumpStop == true)
			{
				m_bJumpCheck = false;
				m_pPlayer->Set_Jump(false);
				SetOneTime = false;

				CStateMgr::Get_Instance()->Set_StateMgr(IDLE);
				JumpStop = false;
			}
		}
		Offset();
	
}

void CPlayerJump::Offset()
{
	UNIT_INFO& playerPos = m_pPlayer->Get_Pos();

	int iScrollX = (int)CScrollMgr::GetScrollPos(0);
	int iScrollY = (int)CScrollMgr::GetScrollPos(1);
	int iOffset_X = WINCX / 2;
	int iOffset_Y = WINCY / 2;
	/*if (iOffset_X + 50 + iScrollX < playerPos.vPos.x )
	{
	CScrollMgr::SetScrollPos({Player_Speed,0.f,0.f});
	}
	else if (iOffset_X - 50 + iScrollX > playerPos.vPos.x)
	{
	CScrollMgr::SetScrollPos({ -Player_Speed,0.f,0.f });
	}*/
	if (iOffset_Y + 10 + iScrollY < playerPos.vPos.y)
	{
		CScrollMgr::SetScrollPos({ 0.f,-JumpTotal,0.f });
	}
	else if (iOffset_Y - 10 + iScrollY > playerPos.vPos.y)
	{
		CScrollMgr::SetScrollPos({ 0.f,+JumpTotal,0.f });
	}
}


