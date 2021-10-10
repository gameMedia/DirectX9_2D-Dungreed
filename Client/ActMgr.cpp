#include "stdafx.h"
#include "ActMgr.h"
#include "ActCommand.h"
#include "KeyMgr.h"


CActMgr::CActMgr()
	: m_pPlayer(nullptr)
	, lateInit(false)
{

}
CActMgr::~CActMgr()
{
	Release();
}

HRESULT CActMgr::Initialize()
{
	return S_OK;
}

void CActMgr::Handleler()
{
	if (lateInit == false)
	{ // 커맨드 선언 


		lateInit = true;
	}

	for (auto j : map_Command_Down)
	{
		if (CKeyMgr::Get_Instance()->KeyDown(j.first))
		{
			j.second->Execute(m_pPlayer);
		}
	}
	for (auto i : map_Command)
	{
		if (CKeyMgr::Get_Instance()->KeyPressing(i.first))
		{
			i.second->Execute(m_pPlayer);
		}
	}




}


void CActMgr::Release()
{
	for (auto j : map_Command_Down)
	{
		if (j.second)
		{
			SafeDelete(j.second);
		}
	}
	map_Command_Down.clear();

	for (auto i : map_Command)
	{
		if (i.second)
		{
			SafeDelete(i.second);
		}
	
	}
	map_Command.clear();

}

void CActMgr::Set_KeyRegist(int _Key, CActCommand * _Command)
{
	auto& findIter = map_Command.find(_Key);

	if (findIter != map_Command.end())
	{
		MessageBox(g_hWND, L"이미 키가 있습니다.", L"KeyInupt FAIL", MB_OK);
		return;
	}

	map_Command[_Key] = _Command;
}

void CActMgr::Set_KeyRegist_Down(int _Key, CActCommand * _Command)
{
	auto& findIter = map_Command_Down.find(_Key);

	if (findIter != map_Command_Down.end())
	{
		MessageBox(g_hWND, L"이미 키가 있습니다.", L"KeyInupt FAIL", MB_OK);
		return;
	}

	map_Command_Down[_Key] = _Command;
}

CActMgr * CActMgr::Create()
{
	CActMgr* m_pInstance = new CActMgr;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;

	}
	return m_pInstance;
}
