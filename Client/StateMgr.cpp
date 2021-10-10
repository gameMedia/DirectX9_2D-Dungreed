#include "stdafx.h"
#include "StateMgr.h"

INFLEMENT_SIGLETONE(CStateMgr)

CStateMgr::CStateMgr()
	:ActState(true)
	, m_eCurState(IDLE)
	, m_eNextState(IDLE)
{
}


CStateMgr::~CStateMgr()
{
}

void CStateMgr::Initialize()
{
}

bool CStateMgr::StateCheck(int _State)
{
	ActState = true; // 일단 ActState는 트루로 초기화

 	if (m_eCurState == JUMP && _State !=DASH) // 현재 상태가 점프일 시 
		m_eNextState = JUMP;
	else
		m_eNextState = _State;

	if ((m_eCurState == JUMP && _State == MOVE)
		||(m_eCurState == MOVE && _State == JUMP)) // 현재 상태가 점프일 시 
		m_eNextState = JUMP;
	else
		m_eNextState = _State;

	if (m_eCurState == DASH && _State == MOVE)
	{
		m_eNextState = DASH;
	}
	if (m_eCurState == IDLE)
	{ // IDLE일때는 무슨 행동이든 가능
		ActState = true;
	}
	if (m_eCurState == ATT && _State == MOVE)
	{// 공격상태일때 움직이기는 가능
		ActState = true;
	}
	if (_State == DASH)
	{
		ActState = true;
	}
	

	if (m_eCurState == STUN)
	{
		ActState = false; // 스턴 상태 라면은 행동을 막겠다.
	}
	if (m_eCurState == HIT)
	{
		ActState = false; // 피격 중에는 행동을 막겠다.
	}
	if (m_eCurState == JUMP && _State == JUMP)
	{// 점프 상태일때  다시 점프는 못해
		ActState = false;
	}
	if (m_eCurState == DASH && _State == DASH)
	{// 대쉬중 대쉬 불가?
		ActState = false;
	}
	if (m_eCurState == JUMP && _State == ATT)
	{// 점프 상태일때 
		ActState = true;
	}
	if (ActState == true)
	{ // 만약 ActState가 트루라면 현재 상태를 바꿔줘라
		m_eCurState = m_eNextState;
	}

	return ActState; // false를 넘겨준다는 것은 행동의 변화를 막겠다 이다
}

void CStateMgr::Set_StateMgr(int _State)
{
	m_eNextState = _State;
	if (m_eCurState != m_eNextState)
	{
		m_eCurState = m_eNextState;
	}
}
