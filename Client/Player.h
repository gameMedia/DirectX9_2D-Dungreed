#pragma once
#include "Obj.h"
#include "ICommandActor.h"
class CPlayerAct;

class CPlayer :
	public CObj
	, public ICommandActor
{
public:
	ENUM_STATE;
	ENUM_DIR;
private:
	explicit CPlayer();
public:	
	virtual ~CPlayer();
public:// Get
	UNIT_INFO& Get_Pos() { return m_tUnit; }
	UNIT_INFO& Get_Bat() { return Bat; }
public:// Set
	void Set_Jump(bool bJump) { m_bJump = bJump; }
	void Set_Dash(bool bDash) { m_bDash = bDash; }
	void Set_DownJump(bool bDownJump) { m_bDownJump = bDownJump; }
public:
	// CObj을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	void UpdateRect();
	void Artifact();
	void FrameMove1(float _aniSpeed = 1.f);
private:
	void PlayerAct();

	// GRAVITY
	void GravityLine();
	void GravityTile();

	// STATE
	void StateCheck();
	void FrameChange();
	void Idle();
	void Move();
	void Jump();

	void CheckReset();
public:
	virtual void ActStart(int _State,int _Dir =0);
	void AttStart();
	// 충돌이벤트 함수
	virtual void RectColl(CObj* pObj = nullptr);
	void OffSet();
public:
	static CPlayer* Create();
	static int Money;
	static int Hungry;
	static int PlusAtt;
	static int HungryFull;
	static int DashCount;
	static int DashFullCount;
private:
	float ScrollSpeed;
	int m_eCurState;
	int m_eNextState;

	bool ACT_OK;
	bool m_bJump;
	bool m_bDownJump;
	bool m_bAtt;
	bool m_bDash;

	CPlayerAct* PlayJump;
	CPlayerAct* PlayDownJump;
	CPlayerAct* PlayMoveLeft;
	CPlayerAct* PlayMoveRight;
	CPlayerAct* PlayLookMouse;
	CPlayerAct* PlayerDash;

	float FrameSpeed;
	float TotalJump;
	bool ActCheck;

	wstring ChangeStateKey;
	float Dir;

	D3DXVECTOR2 m_vOrigin[5];
	D3DXVECTOR2 m_vPoint[4];

	ONOFFUIID::ID eID;


	// 충격이벤트
	bool HitCheck;
	int Red;
	int Green;
	int Blue;

	float CreateTimeReset;
	float EndTimeReset;


	float CreateRegenTime;
	float EndRegenTime;

	float CreateRegenDash;
	float EndRegenDash;

	D3DXMATRIX matScale, matTrans, matWorld;
	UNIT_INFO Bat;
	FRAME m_tBat;

	ITEM_DATA Art1;
	ITEM_DATA Art2;
	ITEM_DATA Art3;
	ITEM_DATA Art4;

};


