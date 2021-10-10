#pragma once
#include "Obj.h"
class CMagicShield :
	public CObj
{
private:
	explicit CMagicShield();
public:
	virtual ~CMagicShield();

public:
	// CObj��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	// �۾�
	void RectColl(CObj* pObj = nullptr); // collisionMgr�� �浹������ ȣ�� �� �Լ�

	void GravityLine();
	void GravityTile();


public:
	static CMagicShield* Create();

private:
	bool m_bJump;
	float iJumpPower;
	float JumpTotal;

	ITEM_DATA itemdata;
	bool m_bJumpCheck;
	bool JumpStop;
};
