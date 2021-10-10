#pragma once
#include "Observer.h"
class CMonsterObserver :
	public CObserver
{
private:
	CMonsterObserver();

public:
	virtual ~CMonsterObserver();

public:
	int GetMonsterHp();
	D3DXVECTOR3 GetMonsterPos();
public:
	// CObserver을(를) 통해 상속됨
	virtual void Update(int iMessage, void * pData) override;

public:
	static CMonsterObserver* Create();

private:
	int			m_iMonsterHp = 0;
	int			m_iMonsterMp = 0;
	D3DXVECTOR3 m_MonsterPos;
	D3DXMATRIX	m_matMonster;
};

