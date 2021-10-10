#pragma once
#include "Obj.h"
class CSkullSoldier :
	public CObj
{
private:
	explicit CSkullSoldier();
public:
	virtual ~CSkullSoldier();

private:
	enum STATE { IDLE = 1, MOVE, HIT, ATTACK, END };
public:
	// Get
	const int Get_eCurState() const { return m_eCurState; }
public:
	// Set

public:
	// CObj을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize();
	virtual void Release() override;

public:
	// 작업
	void MakeLine();
	// 충돌이벤트 함수
	virtual void RectColl(CObj* pObj = nullptr);
	// GRAVITY
	void GravityLine();
	void GravityTile();

	// STATE
	void StateCheck();
	void FrameChange();
	void ActionStart();
	void Idle();
	void Move();
	void Attack(); // 개는 점프할 때만 공격 한다
	void CheckReset();
public:
	static CSkullSoldier* Create();

private:

	int m_eCurState;
	int m_eNextState;
	D3DXVECTOR2 m_vOrigin[5];
	float FrameSpeed;
	D3DXVECTOR3 PlayerDir; // 플레이어 방향
	float PlayerLength; // 플레이어 거리
						// 점프
	float iJumpPower;
	float JumpTotal;
	bool JumpStop;
	bool SetOneTime;
	bool m_bJumpCheck;

	clock_t Jump_Current_Tick;
	clock_t Jump_Next_Tick;

	float CreateTimeTarget; // 플레이어의 위치를 갱신할 시간
	float EndTimeTarget; // 플레이어의 위치를 갱신할 시간

	float CreateTimeJump; // 점프를 실행할 시간
	float EndTimeJump;// 점프를 실행할 시간


					  // 충격이벤트
	bool HitCheck;
	int Red;
	int Green;
	int Blue;

	float CreateTimeReset;
	float EndTimeReset;

};