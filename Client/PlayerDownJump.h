#pragma once
#include "PlayerAct.h"
class CPlayerDownJump :
	public CPlayerAct
{
public:
	CPlayerDownJump();
	virtual ~CPlayerDownJump();

	// CPlayerAct��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Execute(clock_t& _currentTick) override;
	virtual void Offset() override;

public:
	// Set
	void Set_JumpStop(bool _jumpstop) { JumpStop = true; }
private:

	// ���� Ŭ������ �ű��� ���߿�
	float iJumpPower;
	float JumpTotal; // ���� ��Ż�� -���� �Ǹ� �߶���
	
	bool SetOneTime; // �ѹ��� ����
	bool m_bJumpCheck; // ������ ������ bool��
	bool JumpStop;

	clock_t Current_tick; // ���μ����� �󸶳� �����ߴ��� ������� clock_t �ڷ��� ����
	clock_t Next_tick;

	
	// ���μ����� �󸶳� �����ߴ��� ������� clock_t �ڷ��� ����
};

