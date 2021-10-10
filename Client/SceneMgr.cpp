#include "stdafx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "Stage.h"
#include "MainMenu.h"
#include "Dungeon.h"
INFLEMENT_SIGLETONE(CSceneMgr)

CSceneMgr::CSceneMgr()
	:m_ePreScene(END)
	,m_eCurScene(END)
	,m_pScene(nullptr)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

HRESULT CSceneMgr::SceneChange(SCENE_TYPE eType)
{
	m_eCurScene = eType;

	if (m_ePreScene != m_eCurScene)
	{
		SafeDelete(m_pScene);

		switch (m_eCurScene)
		{
		case CSceneMgr::LOGO:
			m_pScene = CLogo::Create();
			break;
		case CSceneMgr::STAGE:
			m_pScene = CStage::Create();
			break;
		case CSceneMgr::MAINMENU:
			m_pScene = CMainMenu::Create();
			break;
		case CSceneMgr::DUNGEON:
			m_pScene = CDungeon::Create();
		}
		NULL_CHECK_RETURN(m_pScene, E_FAIL);
		m_ePreScene = m_eCurScene;
	}
	return S_OK;
}

void CSceneMgr::Update()
{
	NULL_CHECK(m_pScene);
	m_pScene->Update();
}

void CSceneMgr::LateUpdate()
{
	NULL_CHECK(m_pScene);
	m_pScene->LateUpdate();
}

void CSceneMgr::Reneder()
{
	NULL_CHECK(m_pScene);
	m_pScene->Render();
}

void CSceneMgr::Release()
{
	SafeDelete(m_pScene);
}
