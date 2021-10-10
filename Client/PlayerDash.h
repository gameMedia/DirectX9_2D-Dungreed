#pragma once
#include "PlayerAct.h"
class CMouse;
class CPlayerDash :
	public CPlayerAct
{
public:
	CPlayerDash();
	virtual ~CPlayerDash();

public:
	// CPlayerAct을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Execute(clock_t & _currentTick) override;
	virtual void Offset() override;
public:
	// Set
	void Set_DashStop(bool _jumpstop) { DashStop = true; }

private:
	void Lerp(float _time, float _ratio);
private:

	CMouse* Mouse;
	D3DXVECTOR3 MousevDir;

	float DashPower; // 대쉬할 힘
	D3DXVECTOR3 Dir; // 대쉬할 방향
	D3DXVECTOR3 Total_Dash;

	bool InitOneTime;
	bool TimeCheck;
	bool DashStop;

	clock_t Current_tick; // 프로세스가 얼마나 진행했는지 계산해줄 clock_t 자료형 변수
	clock_t Next_tick; // 프로세스가 얼마나 진행했는지 계산해줄 clock_t 자료형 변수

	float CreateTimeAfterImage;
	float EndTimeAfterImage;
};

