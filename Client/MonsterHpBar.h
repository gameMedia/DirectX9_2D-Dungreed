#pragma once
#include "Obj.h"

class CMonsterObserver;
class CMonsterHpBar :
	public CObj
{
private:
	CMonsterHpBar();

public:
	virtual ~CMonsterHpBar();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize() override;
	virtual void Release() override;

public:
	static CMonsterHpBar* Create();

private:
	CMonsterObserver*	m_pObserver;
	int				m_iMonsterHp;
};