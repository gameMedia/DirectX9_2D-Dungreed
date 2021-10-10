#include "stdafx.h"
#include "MonsterObserver.h"
#include "MonsterSubject.h"

CMonsterObserver::CMonsterObserver()
{
	m_MonsterPos = {};
}


CMonsterObserver::~CMonsterObserver()
{
	CMonsterSubject::Get_Instance()->UnSubscribe(this);
}

int CMonsterObserver::GetMonsterHp()
{
	return m_iMonsterHp;
}

D3DXVECTOR3 CMonsterObserver::GetMonsterPos()
{
	return m_MonsterPos;
}

void CMonsterObserver::Update(int iMessage, void * pData)
{
	// Pull Observer ¹æ½Ä.
	const DATA_LIST* pDataLst = CMonsterSubject::Get_Instance()->GetDataLst(iMessage);
	NULL_CHECK(pDataLst);

	auto iter_find = find(pDataLst->begin(), pDataLst->end(), pData);

	if (pDataLst->end() == iter_find)
		return;

	switch (iMessage)
	{
	case CMonsterSubject::MONSTER_HP:
		m_iMonsterHp = *reinterpret_cast<int*>(*iter_find);
		break;
	case CMonsterSubject::MONSTER_POS:
		m_MonsterPos = *reinterpret_cast<D3DXVECTOR3*>(*iter_find);
		break;
	}
}

CMonsterObserver * CMonsterObserver::Create()
{
	CMonsterObserver* pObserver = new CMonsterObserver;
	CMonsterSubject::Get_Instance()->Subscribe(pObserver);
	
	return pObserver;
}
