#pragma once
#include "PlayerAct.h"
class CPlayerDownJump :
	public CPlayerAct
{
public:
	CPlayerDownJump();
	virtual ~CPlayerDownJump();

	// CPlayerAct을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Execute(clock_t& _currentTick) override;
	virtual void Offset() override;

public:
	// Set
	void Set_JumpStop(bool _jumpstop) { JumpStop = true; }
private:

	// 점프 클래스로 옮기자 나중에
	float iJumpPower;
	float JumpTotal; // 점프 토탈이 -값이 되면 추락중
	
	bool SetOneTime; // 한번만 하자
	bool m_bJumpCheck; // 점프를 시켜줄 bool값
	bool JumpStop;

	clock_t Current_tick; // 프로세스가 얼마나 진행했는지 계산해줄 clock_t 자료형 변수
	clock_t Next_tick;

	
	// 프로세스가 얼마나 진행했는지 계산해줄 clock_t 자료형 변수
};

