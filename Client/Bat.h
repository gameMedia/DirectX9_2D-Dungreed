#pragma once
#include "Obj.h"
class CBat :
	public CObj
{
private:
	explicit CBat();
public:
	virtual ~CBat();

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


	// STATE
	void StateCheck();
	void FrameChange();
	void ActionStart();
	void Idle();
	void Move();
	void Attack(); // 개는 점프할 때만 공격 한다
	void CheckReset();
public:
	static CBat* Create();

private:

	int m_eCurState;
	int m_eNextState;
	D3DXVECTOR2 m_vOrigin[5];
	float FrameSpeed;
	D3DXVECTOR3 PlayerDir; // 플레이어 방향
	float PlayerLength; // 플레이어 거리
						// 점프
	

	clock_t Jump_Current_Tick;
	clock_t Jump_Next_Tick;

	float CreateTimeTarget; // 플레이어의 위치를 갱신할 시간
	float EndTimeTarget; // 플레이어의 위치를 갱신할 시간

	float CreateTimeAttack; // 공격으로 전환할 시간
	float EndTimeAttack;// 공격으로 전환할 시간

	float m_FireTerm; // 공격 모션 들어가고서의 텀?
	float m_FireAngle; // 공격을 발사할 각도 한 30도에 하나씩


					  // 충격이벤트
	bool HitCheck;
	int Red;
	int Green;
	int Blue;

	float CreateTimeReset;
	float EndTimeReset;

};
