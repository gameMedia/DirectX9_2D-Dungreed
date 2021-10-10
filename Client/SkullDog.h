#pragma once
#include "Obj.h"
class CSkullDog :
	public CObj
{
private:
	explicit CSkullDog();
public:
	virtual ~CSkullDog();

private:
	enum STATE{IDLE=1,MOVE,HIT,FIRE,JUMP,END};
public:
	// Get
	const int Get_eCurState() const { return m_eCurState; }
public:
	// Set

public:
	// CObj��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize();
	virtual void Release() override;

public:
	// �۾�
	void MakeLine();
	// �浹�̺�Ʈ �Լ�
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
	void Jump(); // ���� ������ ���� ���� �Ѵ�
	void CheckReset();
public:
	static CSkullDog* Create();

private:

	int m_eCurState;
	int m_eNextState;
	D3DXVECTOR2 m_vOrigin[5];
	float FrameSpeed;
	D3DXVECTOR3 PlayerDir; // �÷��̾� ����
	float PlayerLength; // �÷��̾� �Ÿ�
	// ����
	float iJumpPower;
	float JumpTotal;
	bool JumpStop;
	bool SetOneTime;
	bool m_bJumpCheck;
	
	clock_t Jump_Current_Tick;
	clock_t Jump_Next_Tick; 

	float CreateTimeTarget; // �÷��̾��� ��ġ�� ������ �ð�
	float EndTimeTarget; // �÷��̾��� ��ġ�� ������ �ð�

	float CreateTimeJump; // ������ ������ �ð�
	float EndTimeJump;// ������ ������ �ð�


	// ����̺�Ʈ
	bool HitCheck;
	int Red;
	int Green;
	int Blue;

	float CreateTimeReset;
	float EndTimeReset;
	
};

