#include "stdafx.h"
#include "Line.h"


CLine::CLine()
{
	ZeroMemory(&m_tLine, sizeof(LINEINFO));
}

CLine::CLine(LINEINFO & rLine)
	:m_tLine(rLine)
{
}


CLine::~CLine()
{
}

void CLine::Render()
{
	CDeviceMgr::Get_Instance()->GetSprite()->End();


	D3DXVECTOR2 vPos[2] =
	{
		{m_tLine.LPoint.Linepos.x-CScrollMgr::GetScrollPos(0),
		m_tLine.LPoint.Linepos.y - CScrollMgr::GetScrollPos(1) },
		{m_tLine.RPoint.Linepos.x - CScrollMgr::GetScrollPos(0),
		m_tLine.RPoint.Linepos.y - CScrollMgr::GetScrollPos(1) }
	};

	CDeviceMgr::Get_Instance()->GetLine()->SetWidth(5.f);
	CDeviceMgr::Get_Instance()->GetLine()->Draw(vPos, 2, D3DCOLOR_ARGB(m_tLine.Alpha, m_tLine.Red, m_tLine.Green, m_tLine.Blue));

	// LPD3DXLINE�� ������ �ڵ�� SpriteBegin�� End ���̿� �����ϸ�ȵȴ�.

	CDeviceMgr::Get_Instance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

}
