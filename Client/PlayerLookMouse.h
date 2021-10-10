#pragma once
#include "PlayerAct.h"
class CMouse;
class CPlayerLookMouse :
	public CPlayerAct
{
public:
	CPlayerLookMouse();
	virtual ~CPlayerLookMouse();

	// CPlayerAct을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Execute(clock_t& _currentTick) override;
	virtual void Offset() override;
private:
	CMouse* Mouse;
	D3DXVECTOR3 MousevDir;
};

