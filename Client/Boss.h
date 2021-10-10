#pragma once
#include "Obj.h"
class CIcePillar;
class CBoss :
	public CObj
{
public:
	enum STATE{IDLE=1,MOVE,ATT,TELEPORT,DEAD,STUN,ENTERBOSS,REALDEAD,END};
	enum ICEATT{ICE_IDLE=0,FIRST,SECOND,THIRD,ICE_END};

public:
	const int Get_eCurState() const { return m_eCurState; }
	virtual void RectColl(CObj* pObj = nullptr);
private:
	explicit CBoss();
public:
	virtual ~CBoss();
public:
	void Delete_IcePillar(CIcePillar* _p);
public:
	// CObj을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize();
	virtual void Release() override;
private:

	void HardAttStart();
	void NormalAttStart();
	void Stun();
	void StateCheck();
public:
	static CBoss* Create();
	int BulletState;
private:
	list<CIcePillar*> m_IcePillar; // 얼음기둥 4개를 생성하고 담아서 관리하자
	int m_eCurState;
	int m_eNextState;

	bool EnterBoss; // 보스 등장

	bool StunCanHit; // 스턴 상태일때만 때릴수있다.
	float FrameSpeed;
	float CreateStunTime;
	float EndStunTime;
	// 얼음 기둥
	D3DXVECTOR3 m_vOrigin[4]; // 4개의 얼음기둥의 중점
	D3DXVECTOR3 PillarTransPos; // 얼음 기둥이 움직일 좌표

	bool HitCheck;
	int Red;
	int Green;
	int Blue;

	float CreateTimeReset;
	float EndTimeReset;

	void CheckReset();

	bool OneTimeCancle;

	float CreateTimeIceSpear;
	float EndTimeIceSpear;

	float CreateTimeNormalAttack;
	float EndTimeNormalAttack;

	
};

