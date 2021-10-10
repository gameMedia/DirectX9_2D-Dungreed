#include "stdafx.h"
#include "PlayerDownJump.h"
#include "StateMgr.h"

CPlayerDownJump::CPlayerDownJump()
{
}


CPlayerDownJump::~CPlayerDownJump()
{
}

void CPlayerDownJump::Initialize()
{
	JumpStop = false;
	m_bJumpCheck = false;
	iJumpPower = 9.f;
	
}

void CPlayerDownJump::Execute(clock_t& _currentTick)
{
	UNIT_INFO& playerPos = m_pPlayer->Get_Pos();

	float fY = 0.f;
	bool bIsColl = CLineMgr::Get_Instance()->LineCollisionMove((&playerPos.vCollPos), &fY);


	if (m_bJumpCheck == false)
	{
		Current_tick = 0.f;
		Next_tick = 0.f;
		m_bJumpCheck = true;
		JumpStop = false;

	}
	if (m_bJumpCheck == true)
	{
		if (bIsColl)
		{
			Current_tick = clock();
		Next_tick = ((clock() - Current_tick) + 100) / 100;
		JumpTotal =  -(GRAVITY*Next_tick*Next_tick*0.5f)*2;
		playerPos.vPos.y -= JumpTotal;
		playerPos.vCollPos.y -= JumpTotal;
		Offset();
		//playerPos.vPos.y += 40;



			m_bJumpCheck = false;
			m_pPlayer->Set_DownJump(false);
			//JumpTotal = 0.f;
			CStateMgr::Get_Instance()->Set_StateMgr(IDLE);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYERJUMP);
			CSoundMgr::Get_Instance()->PlaySound(L"MyJump.wav", CSoundMgr::PLAYERJUMP);

		}
		else if (JumpStop == true)
		{
			m_bJumpCheck = false;
			m_pPlayer->Set_DownJump(false);
			CStateMgr::Get_Instance()->Set_StateMgr(IDLE);
			JumpStop = false;
		}
	}
}

void CPlayerDownJump::Offset()
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
	if (iOffset_Y + 50 + iScrollY < playerPos.vPos.y)
	{
		CScrollMgr::SetScrollPos({ 0.f,-JumpTotal,0.f });
	}
	else if (iOffset_Y - 50 + iScrollY > playerPos.vPos.y)
	{
		CScrollMgr::SetScrollPos({ 0.f,+JumpTotal,0.f });
	}
}
