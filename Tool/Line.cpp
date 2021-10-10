#include "stdafx.h"
#include "Line.h"
#include "ToolView.h"

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

void CLine::Render(CToolView* pToolView)
{
	CDeviceMgr::Get_Instance()->GetSprite()->End();


	D3DXVECTOR2 vPos[2] =
	{
		{m_tLine.LPoint.Linepos.x-pToolView->GetScrollPos(0),
		m_tLine.LPoint.Linepos.y - pToolView->GetScrollPos(1) },
		{m_tLine.RPoint.Linepos.x - pToolView->GetScrollPos(0),
		m_tLine.RPoint.Linepos.y - pToolView->GetScrollPos(1) }
	};

	CDeviceMgr::Get_Instance()->GetLine()->SetWidth(5.f);
	CDeviceMgr::Get_Instance()->GetLine()->Draw(vPos, 2, D3DCOLOR_ARGB(m_tLine.Alpha, m_tLine.Red, m_tLine.Green, m_tLine.Blue));

	// LPD3DXLINE을 렌더링 코드는 SpriteBegin과 End 사이에 존재하면안된다.

	CDeviceMgr::Get_Instance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

}
