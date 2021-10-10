#include "stdafx.h"
#include "TileMgr.h"

INFLEMENT_SIGLETONE(CTileMgr)

CTileMgr::CTileMgr()
{
}


CTileMgr::~CTileMgr()
{
}

void CTileMgr::SaveTile()
{
	HANDLE hFile = CreateFile(L"../TileData/Stage1_Tile.bat", GENERIC_WRITE,
		0, 0,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if ((INVALID_HANDLE_VALUE == hFile))
	{
		MSG_BOX(L"저장 실패");
		return;
	}
	//TILE_INFO myTile = (*m_VecTile);
	DWORD dwByte = 0;
	for (auto& pTile : (*m_VecTile))
	{
		WriteFile(hFile, pTile, sizeof(TILE_INFO), &dwByte, nullptr);
	}


	CloseHandle(hFile);
	MSG_BOX(L"저장 성공 일려나");
}

void CTileMgr::LoadTile()
{

	HANDLE hFile = CreateFile(L"../TileData/Stage1_Tile.bat",
		GENERIC_READ,
		0, 0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if ((INVALID_HANDLE_VALUE == hFile))
	{
		MSG_BOX(L"로드 실패");
		return;
	}
	if ((*m_VecTile).empty() == false)
	{
		Release();
	}
	
	TILE_INFO* myTile = nullptr;
	DWORD dwByte = 0;
	while (true)
	{
		myTile = new TILE_INFO;
		ReadFile(hFile, myTile, sizeof(TILE_INFO), &dwByte, nullptr);
		if (0 == dwByte)
		{
			SafeDelete(myTile);
			break;
		}
		(*m_VecTile).emplace_back(myTile);
	}

	CloseHandle(hFile);
	MSG_BOX(L"로드 성공 일려나");
}

void CTileMgr::Release()
{
	for (auto& pTile : (*m_VecTile))
	{
		SafeDelete(pTile);
	}
	(*m_VecTile).clear();
	(*m_VecTile).shrink_to_fit();
}

void CTileMgr::Set_Change(int index, int iDraw, int iOption)
{
	(*m_VecTile)[index]->byDrawID = iDraw;
	(*m_VecTile)[index]->byOption = iOption;
}
