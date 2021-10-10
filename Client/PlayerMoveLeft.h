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
	// CPlayerAct��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Execute(clock_t& _currentTick) override;
	virtual void Offset() override;
private:
	float MoveSpeed;
	float TotalJump;

	float CreateTimeAfterImage;
	float EndTimeAfterImage;

};

