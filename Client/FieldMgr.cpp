#include "stdafx.h"
#include "FieldMgr.h"
#include "Field.h"
INFLEMENT_SIGLETONE(CFieldMgr)

CFieldMgr::CFieldMgr()
{
}


CFieldMgr::~CFieldMgr()
{
	Release();
}

void CFieldMgr::Initialize()
{
}

int CFieldMgr::Update(FIELDTYPE::TYPE eType, FIELDID::ID eID)
{
	auto iter_find = m_mapField[eType].find(eID);
	if (iter_find == m_mapField[eType].end())
	{
		MSG_BOX(L"¸ÊÀÌ ¾ø¾î");
		return 0;
	}
	(iter_find->second)->Update();
	return 0;
}

void CFieldMgr::LateUpdate(FIELDTYPE::TYPE eType, FIELDID::ID eID)
{
	auto iter_find = m_mapField[eType].find(eID);
	if (iter_find == m_mapField[eType].end())
	{
		MSG_BOX(L"¸ÊÀÌ ¾ø¾î");
		return;
	}
	(iter_find->second)->LateUpdate();
}

void CFieldMgr::Render(FIELDTYPE::TYPE eType, FIELDID::ID eID)
{
	auto iter_find = m_mapField[eType].find(eID);
	if (iter_find == m_mapField[eType].end())
	{
		MSG_BOX(L"¸ÊÀÌ ¾ø¾î");
		return;
	}
	(iter_find->second)->Render();
}

void CFieldMgr::Release()
{
}

HRESULT CFieldMgr::Add_Field(FIELDTYPE::TYPE eType, FIELDID::ID eID, CField * pField)
{
	NULL_CHECK_RETURN(pField, E_FAIL);
	m_mapField[eType][eID] = pField;

	return S_OK;
}
