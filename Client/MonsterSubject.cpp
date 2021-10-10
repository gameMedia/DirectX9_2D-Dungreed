#include "stdafx.h"
#include "MonsterSubject.h"

INFLEMENT_SIGLETONE(CMonsterSubject)

CMonsterSubject::CMonsterSubject()
{
}


CMonsterSubject::~CMonsterSubject()
{
	m_mapData.clear();
}

const DATA_LIST * CMonsterSubject::GetDataLst(int iMessage) const
{
	auto iter_find = m_mapData.find(iMessage);

	if (m_mapData.end() == iter_find)
		return nullptr;

	return &(iter_find->second);
}

void CMonsterSubject::AddData(int iMessage, void * pData)
{
	NULL_CHECK(pData);

	m_mapData[iMessage].push_back(pData);
	CSubject::Notify(iMessage, pData);
}

void CMonsterSubject::RemoveData(int iMessage, void * pData)
{
	NULL_CHECK(pData);

	auto iter_find = find(m_mapData[iMessage].begin(), m_mapData[iMessage].end(), pData);

	if (m_mapData[iMessage].end() == iter_find)
		return;

	m_mapData[iMessage].erase(iter_find);
}
