#include "stdafx.h"
#include "Scene.h"
#include "ActMgr.h"

CScene::CScene()
	:m_pSceneMgr(CSceneMgr::Get_Instance())
	,m_pObjectMgr(CObjMgr::Get_Instance())
	,m_pTextureMgr(CTextureMgr::Get_Instance())
{
}


CScene::~CScene()
{
}

void CScene::InitKey()
{
}
