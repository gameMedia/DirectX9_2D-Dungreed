#pragma once
class CScrollMgr
{
public:
	CScrollMgr();
	~CScrollMgr();

public:
	static float GetScrollPos(int iAxis);

public:
	static void SetScrollPos(const D3DXVECTOR3& vMove);
	static void Init_m_iScroll(D3DXVECTOR3 & PlayerPos);
public:
	static void ScrollLock(float MapSizeX=0.f, float MapSizeY=0.f);
private:
	static D3DXVECTOR3 m_vScroll;
};

