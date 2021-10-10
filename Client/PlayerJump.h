#pragma once
#include "PlayerAct.h"
class CPlayerJump :
	public CPlayerAct
{
public:
	CPlayerJump();
	virtual ~CPlayerJump();

	// CPlayerAct��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Execute(clock_t& _currentTick) override;
	virtual void Offset() override;
public:
	//Set
	// Ʈ���Ͻ� ������ �����
	void Set_JumpStop(bool _jumpstop) { JumpStop = true; } 

public:
	//Get
	float& Get_JumpTotal() { return JumpTotal; }


private:
	// ���� Ŭ������ �ű��� ���߿�
	float iJumpPower;
	float JumpTotal; // ���� ��Ż�� -���� �Ǹ� �߶���

	bool SetOneTime; // �ѹ��� ����
	bool m_bJumpCheck; // ������ ������ bool��
	bool JumpStop; // 


	clock_t Current_tick; // ���μ����� �󸶳� �����ߴ��� ������� clock_t �ڷ��� ����
	clock_t Next_tick; // ���μ����� �󸶳� �����ߴ��� ������� clock_t �ڷ��� ����
};

