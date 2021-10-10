#include "stdafx.h"
#include "OnOffUIMgr.h"
#include "OnOffUI.h"

INFLEMENT_SIGLETONE(COnOffUIMgr)

COnOffUIMgr::COnOffUIMgr()
{
}


COnOffUIMgr::~COnOffUIMgr()
{
	Release();
}



void COnOffUIMgr::Initialize()
{
}

int COnOffUIMgr::Update(ONOFFUIID::ID eID)
{
	auto iter_find = m_listOnOffUI.find(eID);
	if (iter_find == m_listOnOffUI.end())
	{
	
		return OBJ_NOEVENT;
	}
	(iter_find->second)->Update();
	return 0;
}

void COnOffUIMgr::LateUpdate(ONOFFUIID::ID eID)
{
	auto iter_find = m_listOnOffUI.find(eID);
	if (iter_find == m_listOnOffUI.end())
	{
	
		return;
	}
	(iter_find->second)->LateUpdate();
	
}

void COnOffUIMgr::Render(ONOFFUIID::ID eID)
{
	auto iter_find = m_listOnOffUI.find(eID);
	if (iter_find == m_listOnOffUI.end())
	{
	
		return;
	}
	(iter_find->second)->Render();
	
}

void COnOffUIMgr::Release()
{
	for (auto& pUi : m_listOnOffUI)
	{
		if (pUi.second)
		{
			delete pUi.second;
			pUi.second = nullptr;
		}
	}
	m_listOnOffUI.clear();
}

HRESULT COnOffUIMgr::Add_Object(ONOFFUIID::ID eID, COnOffUI * pUi)
{
	NULL_CHECK_RETURN(pUi, E_FAIL);
	m_listOnOffUI[eID] = pUi;
	return S_OK;
}

HRESULT COnOffUIMgr::Delete_Object(ONOFFUIID::ID eID)
{
	return S_OK;
}
