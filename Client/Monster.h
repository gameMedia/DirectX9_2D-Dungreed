#pragma once
#include "Obj.h"
#include "AIActor.h"
#include "MonsterSubject.h"
class CMonster :
	public CObj, public AIActor
{
private:
	explicit CMonster();
public:
	virtual ~CMonster();

public:
	// CObj을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	
private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize();
	virtual void Release() override;

public:
	// 작업
	virtual void ActStart(int _State);

public:
	static CMonster* Create();
private:
	CMonsterSubject* m_pSubject;
	int m_iHp;
};

