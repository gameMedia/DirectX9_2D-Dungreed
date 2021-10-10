#pragma once
#include "Subject.h"

typedef list<void*>			DATA_LIST;
typedef map<int, DATA_LIST>	MAP_DATA;
class CMonsterSubject :
	public CSubject
{
	DEFINE_SINGLETONE(CMonsterSubject)
public:
	enum DATA_MESSAGE {MONSTER_HP, MONSTER_POS,MSG_END};
private:
	CMonsterSubject();
	virtual ~CMonsterSubject();

public:
	const DATA_LIST* GetDataLst(int iMessage) const;

public:
	void AddData(int iMessage, void* pData);
	void RemoveData(int iMessage, void* pData);

private:
	MAP_DATA	m_mapData;
};

