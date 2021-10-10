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
	// CPlayerAct��(��) ���� ��ӵ�
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

	float DashPower; // �뽬�� ��
	D3DXVECTOR3 Dir; // �뽬�� ����
	D3DXVECTOR3 Total_Dash;

	bool InitOneTime;
	bool TimeCheck;
	bool DashStop;

	clock_t Current_tick; // ���μ����� �󸶳� �����ߴ��� ������� clock_t �ڷ��� ����
	clock_t Next_tick; // ���μ����� �󸶳� �����ߴ��� ������� clock_t �ڷ��� ����

	float CreateTimeAfterImage;
	float EndTimeAfterImage;
};

