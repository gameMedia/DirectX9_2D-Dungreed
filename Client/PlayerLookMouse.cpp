#include "stdafx.h"
#include "PlayerLookMouse.h"
#include "Mouse.h"

CPlayerLookMouse::CPlayerLookMouse()
{
}


CPlayerLookMouse::~CPlayerLookMouse()
{
}

void CPlayerLookMouse::Initialize()
{

	ZeroMemory(&MousevDir,sizeof(D3DXVECTOR3));
}

void CPlayerLookMouse::Execute(clock_t& _currentTick)
{
	UNIT_INFO& playerPos = m_pPlayer->Get_Pos();
	Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());

	MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
	MousevDir.x += CScrollMgr::GetScrollPos(0);
	//MousevDir.y += CScrollMgr::GetScrollPos(1);
	float MouseAngle = D3DXVec3Dot(&MousevDir, &playerPos.vLook);
	if (MouseAngle < 0.f&&playerPos.vSize.x>0)
	{
		playerPos.vSize.x *= -1.f;
	}
	if (MouseAngle >= 0.f&&playerPos.vSize.x <= 0)
	{
		playerPos.vSize.x *= -1.f;
	}
}

void CPlayerLookMouse::Offset()
{
}
