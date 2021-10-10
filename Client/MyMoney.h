#pragma once
#include "Obj.h"
class CMyMoney :
	public CObj
{
private:
	explicit CMyMoney();
public:
	virtual ~CMyMoney();

public:
	// CObj을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	// 작업
	void RectColl(CObj* pObj = nullptr); // collisionMgr로 충돌했을때 호출 할 함수

	void GravityLine();
	void GravityTile();

	void UpdateRect();
public:
	static CMyMoney* Create();

private:
	bool m_bJump;
	float iJumpPower;
	float JumpTotal;

	ITEM_DATA itemdata;
	bool m_bJumpCheck;
	bool JumpStop;
};