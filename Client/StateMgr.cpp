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
	ActState = true; // �ϴ� ActState�� Ʈ��� �ʱ�ȭ

 	if (m_eCurState == JUMP && _State !=DASH) // ���� ���°� ������ �� 
		m_eNextState = JUMP;
	else
		m_eNextState = _State;

	if ((m_eCurState == JUMP && _State == MOVE)
		||(m_eCurState == MOVE && _State == JUMP)) // ���� ���°� ������ �� 
		m_eNextState = JUMP;
	else
		m_eNextState = _State;

	if (m_eCurState == DASH && _State == MOVE)
	{
		m_eNextState = DASH;
	}
	if (m_eCurState == IDLE)
	{ // IDLE�϶��� ���� �ൿ�̵� ����
		ActState = true;
	}
	if (m_eCurState == ATT && _State == MOVE)
	{// ���ݻ����϶� �����̱�� ����
		ActState = true;
	}
	if (_State == DASH)
	{
		ActState = true;
	}
	

	if (m_eCurState == STUN)
	{
		ActState = false; // ���� ���� ����� �ൿ�� ���ڴ�.
	}
	if (m_eCurState == HIT)
	{
		ActState = false; // �ǰ� �߿��� �ൿ�� ���ڴ�.
	}
	if (m_eCurState == JUMP && _State == JUMP)
	{// ���� �����϶�  �ٽ� ������ ����
		ActState = false;
	}
	if (m_eCurState == DASH && _State == DASH)
	{// �뽬�� �뽬 �Ұ�?
		ActState = false;
	}
	if (m_eCurState == JUMP && _State == ATT)
	{// ���� �����϶� 
		ActState = true;
	}
	if (ActState == true)
	{ // ���� ActState�� Ʈ���� ���� ���¸� �ٲ����
		m_eCurState = m_eNextState;
	}

	return ActState; // false�� �Ѱ��شٴ� ���� �ൿ�� ��ȭ�� ���ڴ� �̴�
}

void CStateMgr::Set_StateMgr(int _State)
{
	m_eNextState = _State;
	if (m_eCurState != m_eNextState)
	{
		m_eCurState = m_eNextState;
	}
}
