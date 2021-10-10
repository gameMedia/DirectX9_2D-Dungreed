#include "stdafx.h"
#include "UIMgr.h"

#include "UI.h"

INFLEMENT_SIGLETONE(CUIMgr)

CUIMgr::CUIMgr()
{
}


CUIMgr::~CUIMgr()
{
	Release();
}

void CUIMgr::Initialize()
{

}

int CUIMgr::Update()
{
	for (int i = 0; i < UIID::END; ++i)
	{
		for (auto& iter = m_listUI[i].begin(); iter != m_listUI[i].end();)
		{
			int iEvent = (*iter)->Update();
			if (OBJ_DEAD == iEvent)
			{
				if (*iter)
				{
					delete *iter;
					*iter = nullptr;
				}
				iter = m_listUI[i].erase(iter);
			}
			else
				++iter;
		}
	}
	return 0;
}

void CUIMgr::LateUpdate()
{
	for (int i = 0; i < UIID::END; ++i)
	{
		for (auto& pUi : m_listUI[i])
		{
			pUi->LateUpdate();
		}
	}
}

void CUIMgr::Render()
{
	for (int i = 0; i < UIID::END; ++i)
	{
		for (auto& pUi : m_listUI[i])
		{
			pUi->Render();
		}
	}
}

void CUIMgr::Release()
{
	for (int i = 0; i < UIID::END; ++i)
	{
		for (auto& pUi : m_listUI[i])
		{
			if (pUi)
			{
				delete pUi;
				pUi = nullptr;
			}
		}
		m_listUI[i].clear();
	}
}

HRESULT CUIMgr::Add_Object(UIID::ID eID, CUI * pUi)
{
	NULL_CHECK_RETURN(pUi, E_FAIL);
	m_listUI[eID].emplace_back(pUi);
	return S_OK;
}



HRESULT CUIMgr::Delete_Object(UIID::ID eID)
{
	for (auto& pUi : m_listUI[eID])
	{
		if (pUi)
		{
			delete pUi;
			pUi = nullptr;
		}
	}
	m_listUI[eID].clear();

	return S_OK;
}
