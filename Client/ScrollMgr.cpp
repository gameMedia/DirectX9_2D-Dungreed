#include "stdafx.h"
#include "ScrollMgr.h"

D3DXVECTOR3 CScrollMgr::m_vScroll = {};

CScrollMgr::CScrollMgr()
{
}


CScrollMgr::~CScrollMgr()
{
}

float CScrollMgr::GetScrollPos(int iAxis)
{
	switch (iAxis)
	{
	case 0:
		return m_vScroll.x;
	case 1:
		return m_vScroll.y;
	}

	return 0.0f;
}

void CScrollMgr::SetScrollPos(const D3DXVECTOR3 & vMove)
{
	m_vScroll += vMove;
}

void CScrollMgr::Init_m_iScroll(D3DXVECTOR3 & PlayerPos)
{
	m_vScroll.x = (PlayerPos.x) - (WINCX / 2) ;
	m_vScroll.y = (PlayerPos.y) - (WINCY / 2)  ;
}

void CScrollMgr::ScrollLock(float MapSizeX, float MapSizeY)
{
	if (0 > m_vScroll.x)
		m_vScroll.x = 0;
	if (0 > m_vScroll.y)
		m_vScroll.y = 0;
	//-1120 -680 
	if (MapSizeX-WINCX < m_vScroll.x)
		m_vScroll.x = MapSizeX-WINCX;
	if (MapSizeY-WINCY < m_vScroll.y)
		m_vScroll.y = MapSizeY-WINCY;

}
