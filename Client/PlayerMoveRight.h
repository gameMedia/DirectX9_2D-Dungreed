#pragma once
#include "PlayerAct.h"
class CPlayerMoveRight :
	public CPlayerAct
{
public:
	CPlayerMoveRight();
	virtual ~CPlayerMoveRight();

	// CPlayerAct을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Execute(clock_t& _currentTick) override;
	virtual void Offset() override;
private:
	float MoveSpeed;

	float CreateTimeAfterImage;
	float EndTimeAfterImage;
};

