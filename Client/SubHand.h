#pragma once
#include "Obj.h"
class CInventory;
class CPlayer;
class CMouse;

class CSubHand :
	public CObj
{
private:
	explicit CSubHand();
public:
	virtual ~CSubHand();
public:
	// Set
	void Set_Player(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }
	void Set_Angle(float _pAngle) { m_fAngle = _pAngle; }
	void Set_FirstSubItem(ITEM_DATA _FirstInven);
	void Set_SecondSubItem(ITEM_DATA _SecondInven);
	void Set_State(int _State) { m_eNextState = _State; }
public:
	ITEM_DATA Get_Main() { return Main; }
public:
	enum STATE { UNEQUIPED, EQUIPEDFIRST, EQUIPEDSECOND, END };
public:
	// CObj��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize();
	virtual void Release() override;
private:
	void MakeWorldMatrix();
	void StateCheck();
	void UnEquiped();
	void EquipedFirst();
	void EquipedSecond();
	void Attack();
	void HandSetting();

public:
	// Set
	void Set_AttackStop(bool _attackstop) { AttackStop = _attackstop; }
public:
	static CSubHand* Create();
private:
	CPlayer* m_pPlayer;
	ITEM_DATA First; // ù��° â�� ���ι���
	ITEM_DATA Second; // �ι�° â�� ���ι���
	ITEM_DATA Main; // ���� ����
	int m_eCurState; // ���� ����
	int m_eNextState; // �ٲ� ����

	float m_fDist;
	float m_RotAngle;
	float m_RotAngle2;

	D3DXVECTOR3 HandSize;

	float AttackPower; // ������ ��
	float AttackPowerTotal; // ������ ���� ��
	D3DXVECTOR3 Dir; // ������ ����;
	D3DXVECTOR3 Total_Attack; // ���� ����

	clock_t Current_tick; // ���μ����� �󸶳� �����ߴ��� ������� clock_t �ڷ��� ����
	clock_t Next_tick; // ���μ����� �󸶳� �����ߴ��� ������� clock_t �ڷ��� ����

	bool m_bIsAttack;
	bool AttackStop;
	bool InitOneTimeHand; // ���ָ� ����
	bool m_bAttackCheck;
	bool HandThrow;
	bool HandReturn;
	CMouse* Mouse;
	D3DXVECTOR3 MousevDir;

	bool OnOffInven;


	// 1������ ��������
	int AttackCount;
};

