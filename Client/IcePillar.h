#pragma once
#include "Obj.h"
class CBoss;
class CIcePillar :
	public CObj
{
private:
	CIcePillar();
public:
	virtual ~CIcePillar();
public:
	void Set_Boss(CBoss* _pBoss) { m_pBoss = _pBoss; }
	void Set_Angel(float _pAngle) { m_fAngle = _pAngle; }
	void Set_State(int _state) { m_eNextState = _state; }
	void Set_TransPos(D3DXVECTOR3 _trans) { TransPos = _trans; }
	void Set_Number(int _number) { MyNumber = _number; }
	void Set_PatternStart(bool _StartPatten) { PatternStart = _StartPatten; }
public:
	int Get_State() { return m_eCurState; }
	int Get_Number() { return MyNumber; }
public:
	enum STATE { IDLE = 1, ATT,
		PATTERN1, PATTERN2,
		HARDPATTERN1, HARDPATTERN2,
		ENTERPILLAR, END };
public:
	// CObj을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
public:
	virtual void RectColl(CObj* pObj = nullptr);
public:
////////////////////////////////	//IDLE
	void Idle();
	bool IdleSet;
	void IdleSetting();
////////////////////////////////	//PATTERN1
	void Pattern1();
	bool Pattern1Set;
	void Pattern1Setting();
	float CreateTimeBullet;
	float EndTimeBullet;
	float m_FireTermPattern1;

////////////////////////////////	//PATTERN2
	void Pattern2();
	bool Pattern2Set;
	void Pattern2Setting();
	float CreateTimePattern2;
	float EndTimePattern2;
	float m_FireTermPattern2;

/////////////////////////////////	//PATTERN3
	void HardPattern1();
	bool Pattern3Set;
	void Pattern3Setting();
	float CreateTimePattern3;
	float EndTimePattern3;
	float m_FireTermPattern3;
////////////////////////////////	//PATTERN4
	void HardPattern2();
	bool Pattern4Set;
	void Pattern4Setting();

	float CreateBulletSound;
	float EndBulletSound;
	///////////////////////
private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize();
	virtual void Release() override;
private:
	void SettingPattern();

	void MakeWorldMatrix();
	void StateCheck();
	void FrameChange();
public:
	
	void AttStart();


public:
	static CIcePillar* Create();
private:
	CBoss* m_pBoss;

	int m_eCurState;// 현재 상태
	int m_eNextState;// 바뀔 상태

	float m_fDist;
	float m_RotAngle;

	bool EnterPillar;
	bool lateInit;

	

	int MyNumber;

	D3DXVECTOR3 myPos;
	D3DXVECTOR3 TransPos; // 얼음 기둥은 패턴 시작시 이곳을 향해 이동한후 패턴을 시작한다.
	D3DXMATRIX TransMatrix;
	D3DXMATRIX test;


	D3DXVECTOR3 FirstPos; // 처음 생성되는 위치
	float FirstAngle;
	bool InitOneTime;
	bool PatternStart;

	UNIT_INFO TargetInfo;
	D3DXVECTOR3 TargetPos;
	D3DXVECTOR3 PlayerDir1;

	bool HitCheck;
	int Red;
	int Green;
	int Blue;
	
	float CreateTimeReset;
	float EndTimeReset;
	void CheckReset();
	void UpdateRect();

};

