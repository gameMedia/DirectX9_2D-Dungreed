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
	// CObj을(를) 통해 상속됨
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
	ITEM_DATA First; // 첫번째 창의 메인무기
	ITEM_DATA Second; // 두번째 창의 메인무기
	ITEM_DATA Main; // 최종 무기
	int m_eCurState; // 현재 상태
	int m_eNextState; // 바뀔 상태

	float m_fDist;
	float m_RotAngle;
	float m_RotAngle2;

	D3DXVECTOR3 HandSize;

	float AttackPower; // 공격할 힘
	float AttackPowerTotal; // 공격할 최종 힘
	D3DXVECTOR3 Dir; // 공격할 방향;
	D3DXVECTOR3 Total_Attack; // 힘과 방향

	clock_t Current_tick; // 프로세스가 얼마나 진행했는지 계산해줄 clock_t 자료형 변수
	clock_t Next_tick; // 프로세스가 얼마나 진행했는지 계산해줄 clock_t 자료형 변수

	bool m_bIsAttack;
	bool AttackStop;
	bool InitOneTimeHand; // 맨주먹 공격
	bool m_bAttackCheck;
	bool HandThrow;
	bool HandReturn;
	CMouse* Mouse;
	D3DXVECTOR3 MousevDir;

	bool OnOffInven;


	// 1번무기 근접공격
	int AttackCount;
};

