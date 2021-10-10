#pragma once
#include "Obj.h"
class CBansh :
	public CObj
{
private:
	explicit CBansh();
public:
	virtual ~CBansh();

private:
	enum STATE { IDLE = 1, MOVE, HIT, ATTACK, END };
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


	// STATE
	void StateCheck();
	void FrameChange();
	void ActionStart();
	void Idle();
	void Move();
	void Attack(); // ���� ������ ���� ���� �Ѵ�
	void CheckReset();
public:
	static CBansh* Create();

private:

	int m_eCurState;
	int m_eNextState;
	D3DXVECTOR2 m_vOrigin[5];
	float FrameSpeed;
	D3DXVECTOR3 PlayerDir; // �÷��̾� ����
	float PlayerLength; // �÷��̾� �Ÿ�
						// ����


	clock_t Jump_Current_Tick;
	clock_t Jump_Next_Tick;

	float CreateTimeTarget; // �÷��̾��� ��ġ�� ������ �ð�
	float EndTimeTarget; // �÷��̾��� ��ġ�� ������ �ð�

	float CreateTimeAttack; // �������� ��ȯ�� �ð�
	float EndTimeAttack;// �������� ��ȯ�� �ð�

	float m_FireTerm; // ���� ��� ������ ��?
	float m_FireAngle; // ������ �߻��� ���� �� 30���� �ϳ���

					  // ����̺�Ʈ
	bool HitCheck;
	int Red;
	int Green;
	int Blue;

	float CreateTimeReset;
	float EndTimeReset;

};

