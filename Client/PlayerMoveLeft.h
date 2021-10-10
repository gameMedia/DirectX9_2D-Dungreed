#pragma once
#include "PlayerAct.h"
class CPlayerMoveLeft :
	public CPlayerAct
{


public:
	CPlayerMoveLeft();
	virtual ~CPlayerMoveLeft();

public:

public:
	// CPlayerAct을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Execute(clock_t& _currentTick) override;
	virtual void Offset() override;
private:
	float MoveSpeed;
	float TotalJump;

	float CreateTimeAfterImage;
	float EndTimeAfterImage;

};

