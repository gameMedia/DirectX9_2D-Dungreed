#include "stdafx.h"
#include "KeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
	:m_dwCurKey(0),m_dwKeyDown(0),m_dwKeyUp(0)
{
	

}


CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::UpdateKey()
{
	m_dwCurKey = 0;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwCurKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwCurKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwCurKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwCurKey |= KEY_RIGHT;
	
	// ����
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwCurKey |= KEY_RETURN;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwCurKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwCurKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwCurKey |= KEY_RBUTTON;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwCurKey |= KEY_A;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwCurKey |= KEY_D;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwCurKey |= KEY_S;
	if (GetAsyncKeyState('V') & 0x8000)
		m_dwCurKey |= KEY_V;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwCurKey |= KEY_Q;
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwCurKey |= KEY_W;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwCurKey |= KEY_E;
	if (GetAsyncKeyState('R') & 0x8000)
		m_dwCurKey |= KEY_R;
	if (GetAsyncKeyState('T') & 0x8000)
		m_dwCurKey |= KEY_T;
	if (GetAsyncKeyState('P') & 0x8000)
		m_dwCurKey |= KEY_P;
	if (GetAsyncKeyState('K') & 0x8000)
		m_dwCurKey |= KEY_K;
	if (GetAsyncKeyState('M') & 0x8000)
		m_dwCurKey |= KEY_M;
	if (GetAsyncKeyState(0x31) & 0x8000)
		m_dwCurKey |= KEY_ONE;
	if (GetAsyncKeyState(0x32) & 0x8000)
		m_dwCurKey |= KEY_TWO;
}

bool CKeyMgr::KeyDown(DWORD dwCurKey)
{
	// ���� ������ �ʾҰ� ���� ������ �� true
	if (!(m_dwKeyDown & dwCurKey) && (m_dwCurKey & dwCurKey))
	{
		m_dwKeyDown |= dwCurKey;
		return true;
	}

	// ���� ������ �ְ� ���� ������ �ʾ��� �� m_dwKeyDown ����
	else if ((m_dwKeyDown & dwCurKey) && !(m_dwCurKey & dwCurKey))
	{
		m_dwKeyDown ^= dwCurKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyUp(DWORD dwCurKey)
{
	// ���� ������ �ְ� ���� ������ �ʾ��� �� true
	if ((m_dwKeyUp & dwCurKey) && !(m_dwCurKey & dwCurKey))
	{
		m_dwKeyUp ^= dwCurKey;
		return true;
	}

	// ���� ������ ���� ���� ������ �� false
	else if (!(m_dwKeyUp & dwCurKey) && (m_dwCurKey & dwCurKey))
	{
		m_dwKeyUp |= dwCurKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyPressing(DWORD dwCurKey)
{
	if (m_dwCurKey & dwCurKey)
		return true;

	return false;
}

bool CKeyMgr::KeyCombined(DWORD iKeyPressing, DWORD iKeyDown)
{
	return false;
}

