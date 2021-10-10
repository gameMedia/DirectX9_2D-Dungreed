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
	// CObj��(��) ���� ��ӵ�
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
	list<CIcePillar*> m_IcePillar; // ������� 4���� �����ϰ� ��Ƽ� ��������
	int m_eCurState;
	int m_eNextState;

	bool EnterBoss; // ���� ����

	bool StunCanHit; // ���� �����϶��� �������ִ�.
	float FrameSpeed;
	float CreateStunTime;
	float EndStunTime;
	// ���� ���
	D3DXVECTOR3 m_vOrigin[4]; // 4���� ��������� ����
	D3DXVECTOR3 PillarTransPos; // ���� ����� ������ ��ǥ

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

