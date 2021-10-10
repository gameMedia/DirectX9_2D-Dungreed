#include "stdafx.h"
#include "Terrain.h"
#include "ToolView.h"


CTerrain::CTerrain()
	: m_pToolView(nullptr)
	, RectTileCheck(false)
	, ShowLine(false)
	, PickTileSizeX(0)
	, PickTileSizeY(0)
	, ShowOptionTile(false)
	, PickTile_Check(false)
{
}


CTerrain::~CTerrain()
{
	Release();
}

void CTerrain::SetToolView(CToolView * pToolView)
{
	m_pToolView = pToolView;
}

void CTerrain::TileAllChange(const int& TILE_X, const int& TILE_Y
	, const float& TILE_CX, const float& TILE_CY,
	const BYTE& byDrawID, const BYTE& byOption)
{ // 모든 타일을 바꾸는 함수 
	if (TILE_X <= 0 || TILE_Y <= 0 || TILE_CX <=0 || TILE_CY<=0)
		return;
	TileRelease();
	const TEX_INFO* pMain = nullptr;
	pMain = CTextureMgr::Get_Instance()->GetTexInfo(
		L"Terrain", L"ImgTile", byDrawID);
	NULL_CHECK(pMain);
	float TILE_SizeX = (TILE_CX/pMain->tImgInfo.Width);
	float TILE_SizeY = (TILE_CY/pMain->tImgInfo.Height);
	m_vecTile.reserve(TILE_X * TILE_Y);
	TILE_INFO* pTile = nullptr;
	if (RectTileCheck == true)
	{ // 사각형 깔자
		for (int i = 0; i < TILE_Y; ++i)
		{
			for (int j = 0; j < TILE_X; ++j)
			{
				pTile = new TILE_INFO;
				ZeroMemory(pTile, sizeof(TILE_INFO));

				pTile->vPos.x = (j*TILE_CX) + (TILE_CX*0.5f);
				pTile->vPos.y = (i*TILE_CY) + (TILE_CY*0.5f);
				pTile->vSize = { TILE_SizeX, TILE_SizeY, 0.f };
				pTile->byDrawID = byDrawID; // 처음에 DrawID를 2로 초기화 해줬고
				pTile->byOption = 0;
				pTile->TILEX1 = TILE_X;
				pTile->TILEY1 = TILE_Y;
				pTile->TILECX = TILE_CX;
				pTile->TILECY = TILE_CY;
				
				m_vecTile.emplace_back(pTile);
			}
		}
	}
	else if (RectTileCheck == false)
	{

		for (int i = 0; i < TILE_Y; ++i)
		{
			for (int j = 0; j < TILE_X; ++j)
			{
				pTile = new TILE_INFO;
				ZeroMemory(pTile, sizeof(TILE_INFO));

				pTile->vPos.x = j * TILE_CX + (i % 2) * (TILE_CX * 0.5f);
				pTile->vPos.y = i *  (TILE_CY * 0.5f);
				pTile->vSize = { 1.f, 1.f, 0.f };
				pTile->byDrawID = byDrawID; // 처음에 DrawID를 2로 초기화 해줬고
				pTile->byOption = byOption;

				pTile->TILEX1 = TILE_X;
				pTile->TILEY1 = TILE_Y;
				pTile->TILECX = TILE_CX;
				pTile->TILECY = TILE_CY;

				m_vecTile.push_back(pTile);
			}
		}
	}

}

void CTerrain::Render()
{
	D3DXMATRIX matScale, matTrans;
	const TEX_INFO* pTexInfo = nullptr;
	const TEX_INFO* pMain = nullptr;
	TCHAR szIndex[MIN_STR] = L"";

	if (ShowLine == true)
	{

		for (size_t i = 0; i < m_vecTile.size(); ++i)
		{
			//m_vecTile[i]->vPos = { 400.f, 300.f, 0.f };

			D3DXMatrixScaling(
				&matScale, m_vecTile[i]->vSize.x, m_vecTile[i]->vSize.y, 0.f);
			D3DXMatrixTranslation(
				&matTrans,
				(m_vecTile[i]->vPos.x - m_pToolView->GetScrollPos(0)),
				(m_vecTile[i]->vPos.y - m_pToolView->GetScrollPos(1)),
				0.f);

			m_vecTile[i]->matWorld = matScale * matTrans;

			pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
				L"Terrain", L"ImgTile", m_vecTile[i]->byDrawID);
			NULL_CHECK(pTexInfo);

			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

			CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&m_vecTile[i]->matWorld);
			CDeviceMgr::Get_Instance()->GetSprite()->Draw(
				pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
				nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		
		}

	}
	for (size_t j = 0; j < m_vecFieldTile.size(); ++j)
	{
		D3DXMatrixScaling(
			&matScale, m_vecFieldTile[j]->vSize.x, m_vecFieldTile[j]->vSize.y, 0.f);
		D3DXMatrixTranslation(
			&matTrans,
			m_vecFieldTile[j]->vPos.x - m_pToolView->GetScrollPos(0),
			m_vecFieldTile[j]->vPos.y - m_pToolView->GetScrollPos(1),
			0.f);
		m_vecFieldTile[j]->matWorld = matScale * matTrans;
		pMain = CTextureMgr::Get_Instance()->GetTexInfo(
			L"Terrain", L"ImgTile", m_vecFieldTile[j]->byDrawID);
		NULL_CHECK(pMain);

		float CenterX = pMain->tImgInfo.Width*0.5f;
		float CenterY = pMain->tImgInfo.Height*0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&m_vecFieldTile[j]->matWorld);
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			pMain->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		 //폰트 출력
		D3DXMATRIX TextMatWorld, TextMatScale;
		D3DXMatrixScaling(&TextMatScale, 1.f, 1.f, 0.f);
		TextMatWorld = TextMatScale*matTrans;
		swprintf_s(szIndex, L"%d",j);
		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld);
		CDeviceMgr::Get_Instance()->GetFont()->DrawText(
			CDeviceMgr::Get_Instance()->GetSprite(), szIndex, lstrlen(szIndex),
			nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (ShowOptionTile == true)
	{
		for (size_t k = 0; k < m_vecOptionTile.size(); ++k)
		{
			D3DXMatrixScaling(
				&matScale,
				m_vecOptionTile[k]->vSize.x,
				m_vecOptionTile[k]->vSize.y, 0.f);
			D3DXMatrixTranslation(
				&matTrans,
				m_vecOptionTile[k]->vPos.x - m_pToolView->GetScrollPos(0),
				m_vecOptionTile[k]->vPos.y - m_pToolView->GetScrollPos(1),
				0.f);
			m_vecOptionTile[k]->matWorld = matScale * matTrans;
			pMain = CTextureMgr::Get_Instance()->GetTexInfo(
				L"Terrain", L"OptionTile", m_vecOptionTile[k]->byOption);
			NULL_CHECK(pMain);

			float CenterX = pMain->tImgInfo.Width*0.5f;
			float CenterY = pMain->tImgInfo.Height*0.5f;

			CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&m_vecOptionTile[k]->matWorld);
			CDeviceMgr::Get_Instance()->GetSprite()->Draw(
				pMain->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
				nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			//폰트 출력
			D3DXMATRIX TextMatWorld, TextMatScale;
			D3DXMatrixScaling(&TextMatScale, 1.f, 1.f, 0.f);
			TextMatWorld = TextMatScale*matTrans;
			swprintf_s(szIndex, L"%d", m_vecOptionTile[k]->byOption);
			CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld);
			CDeviceMgr::Get_Instance()->GetFont()->DrawText(
				CDeviceMgr::Get_Instance()->GetSprite(), szIndex, lstrlen(szIndex),
				nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

}

void CTerrain::MiniRender()
{
	D3DXMATRIX matScale, matTrans;
	const TEX_INFO* pTexInfo = nullptr;
	const TEX_INFO* pMain = nullptr;
	TCHAR szIndex[MIN_STR] = L"";
	if (ShowLine == true)
	{
		for (size_t i = 0; i < m_vecTile.size(); ++i)
		{
			D3DXMatrixScaling(
				&matScale,
				m_vecTile[i]->vSize.x * 0.5f,
				m_vecTile[i]->vSize.y * 0.5f,
				0.f);
			D3DXMatrixTranslation(
				&matTrans,
				m_vecTile[i]->vPos.x * 0.5f - m_pToolView->GetScrollPos(0)*0.5f,
				m_vecTile[i]->vPos.y * 0.5f,
				0.f);

			m_vecTile[i]->matWorld = matScale * matTrans;

			pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
				L"Terrain", L"ImgTile", m_vecTile[i]->byDrawID);
			NULL_CHECK(pTexInfo);

			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

			CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&m_vecTile[i]->matWorld);
			CDeviceMgr::Get_Instance()->GetSprite()->Draw(
				pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
				nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	for (size_t j = 0; j < m_vecFieldTile.size(); ++j)
	{
		D3DXMatrixScaling(
			&matScale,
			m_vecFieldTile[j]->vSize.x * 0.5f,
			m_vecFieldTile[j]->vSize.y * 0.5f,
			0.f);
		D3DXMatrixTranslation(
			&matTrans,
			m_vecFieldTile[j]->vPos.x * 0.5f - m_pToolView->GetScrollPos(0)*0.5f,
			m_vecFieldTile[j]->vPos.y * 0.5f,
			0.f);
		m_vecFieldTile[j]->matWorld = matScale * matTrans;
		pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
			L"Terrain", L"ImgTile", m_vecFieldTile[j]->byDrawID);
		NULL_CHECK(pTexInfo);

		float CenterX = pTexInfo->tImgInfo.Width*0.5f;
		float CenterY = pTexInfo->tImgInfo.Height*0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&m_vecFieldTile[j]->matWorld);
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			pTexInfo->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		////폰트 출력
		//D3DXMATRIX TextMatWorld, TextMatScale;
		//D3DXMatrixScaling(&TextMatScale, 1.f, 1.f, 0.f);
		//TextMatWorld = TextMatScale*matTrans;
		//swprintf_s(szIndex, L"%d", j);
		//CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld);
		//CDeviceMgr::Get_Instance()->GetFont()->DrawText(
		//	CDeviceMgr::Get_Instance()->GetSprite(), szIndex, lstrlen(szIndex),
		//	nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (ShowOptionTile == true)
	{
		for (size_t k = 0; k < m_vecOptionTile.size(); ++k)
		{
			D3DXMatrixScaling(
				&matScale,
				m_vecOptionTile[k]->vSize.x * 0.5f,
				m_vecOptionTile[k]->vSize.y * 0.5f, 0.f);
			D3DXMatrixTranslation(
				&matTrans,
				m_vecOptionTile[k]->vPos.x * 0.5f - m_pToolView->GetScrollPos(0) * 0.5f,
				m_vecOptionTile[k]->vPos.y * 0.5f,
				0.f);
			m_vecOptionTile[k]->matWorld = matScale * matTrans;
			pMain = CTextureMgr::Get_Instance()->GetTexInfo(
				L"Terrain", L"OptionTile", m_vecOptionTile[k]->byOption);
			NULL_CHECK(pMain);

			float CenterX = pMain->tImgInfo.Width*0.5f;
			float CenterY = pMain->tImgInfo.Height*0.5f;

			CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&m_vecOptionTile[k]->matWorld);
			CDeviceMgr::Get_Instance()->GetSprite()->Draw(
				pMain->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
				nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			////폰트 출력
			//D3DXMATRIX TextMatWorld, TextMatScale;
			//D3DXMatrixScaling(&TextMatScale, 1.f, 1.f, 0.f);
			//TextMatWorld = TextMatScale*matTrans;
			//swprintf_s(szIndex, L"%d", m_vecOptionTile[k]->byOption);
			//CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld);
			//CDeviceMgr::Get_Instance()->GetFont()->DrawText(
			//	CDeviceMgr::Get_Instance()->GetSprite(), szIndex, lstrlen(szIndex),
			//	nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CTerrain::TileChange(
	const D3DXVECTOR3 & vPos, const BYTE & byDrawID, const BYTE & byOption)
{
	int iIndex = SelectIndex(vPos);

	if (-1 == iIndex)
		return;

	if (PickTile_Check == true)
	{

		const TEX_INFO* pMain = nullptr;
		if (byOption == 0)
		{
			pMain = CTextureMgr::Get_Instance()->GetTexInfo(
				L"Terrain", L"ImgTile", byDrawID);

			float TILE_SizeX = (PickTileSizeX / pMain->tImgInfo.Width);
			float TILE_SizeY = (PickTileSizeY / pMain->tImgInfo.Height);
			NULL_CHECK(pMain);
			{
				TILE_INFO* pTile;
				pTile = nullptr;
				pTile = new TILE_INFO;
				pTile->vPos.x = m_vecTile[iIndex]->vPos.x;
				pTile->vPos.y = m_vecTile[iIndex]->vPos.y;
				pTile->vSize = { TILE_SizeX, TILE_SizeY, 0.f };
				pTile->vLength = { PickTileSizeX,PickTileSizeY,0.f };
				pTile->byDrawID = byDrawID; // 처음에 DrawID를 2로 초기화 해줬고
				pTile->byOption = 0;
				pTile->TILEX1 = (int)m_vecTile[iIndex]->vPos.x;
				pTile->TILEY1 = (int)m_vecTile[iIndex]->vPos.y;
				pTile->TILECX = m_vecTile[iIndex]->TILECX;
				pTile->TILECY = m_vecTile[iIndex]->TILECY;

				for (auto i : m_vecFieldTile)
				{
					if (i->vPos == pTile->vPos)
					{// 위치가 동일한가?
						i->byDrawID = byDrawID;
						i->byOption = 0;
						return;
					}
				}
				m_vecFieldTile.push_back(pTile);
			}
		}

		else if (byOption > 0)
		{
			// 옵션 타일
			pMain = CTextureMgr::Get_Instance()->GetTexInfo(
				L"Terrain", L"OptionTile", byOption);

			float TILE_SizeX = (PickTileSizeX / pMain->tImgInfo.Width);
			float TILE_SizeY = (PickTileSizeY / pMain->tImgInfo.Height);
			NULL_CHECK(pMain);
			{
				TILE_INFO* pTile;
				pTile = nullptr;
				pTile = new TILE_INFO;
				pTile->vPos.x = m_vecTile[iIndex]->vPos.x;
				pTile->vPos.y = m_vecTile[iIndex]->vPos.y;
				pTile->vSize = { TILE_SizeX, TILE_SizeY, 0.f };
				pTile->vLength = { PickTileSizeX,PickTileSizeY,0.f };
				pTile->byDrawID = byOption; // 처음에 DrawID를 2로 초기화 해줬고
				pTile->byOption = byOption;
				pTile->TILEX1 = (int)m_vecTile[iIndex]->vPos.x;
				pTile->TILEY1 = (int)m_vecTile[iIndex]->vPos.y;
				pTile->TILECX = m_vecTile[iIndex]->TILECX;
				pTile->TILECY = m_vecTile[iIndex]->TILECY;

				for (auto i : m_vecOptionTile)
				{
					if (i->vPos == pTile->vPos)
					{// 위치가 동일한가?
						i->byDrawID = byOption;
						i->byOption = byOption;
						return;
					}
				}
				m_vecOptionTile.push_back(pTile);
			}
		}
	}
	//m_vecTile[iIndex]->byDrawID = byDrawID;
	//m_vecTile[iIndex]->byOption = byOption;
}

void CTerrain::ImgTileDelete(const D3DXVECTOR3 & vPos, int & _Option)
{
	int iIndex = SelectIndex(vPos);
	if (-1 == iIndex)
		return;

	if (PickTile_Check == true)
	{

		if (_Option == 0)
		{
			{
				TILE_INFO* pTile;
				pTile = nullptr;
				pTile = new TILE_INFO;
				pTile->vPos.x = m_vecTile[iIndex]->vPos.x;
				pTile->vPos.y = m_vecTile[iIndex]->vPos.y;
				pTile->vSize = { m_vecTile[iIndex]->vSize.x, m_vecTile[iIndex]->vSize.y, 0.f };

				pTile->TILEX1 = (int)m_vecTile[iIndex]->vPos.x;
				pTile->TILEY1 = (int)m_vecTile[iIndex]->vPos.y;
				pTile->TILECX = m_vecTile[iIndex]->TILECX;
				pTile->TILECY = m_vecTile[iIndex]->TILECY;

				for (auto iter = m_vecFieldTile.begin(); iter != m_vecFieldTile.end();)
				{
					if ((*iter)->vPos == pTile->vPos)
					{
						m_vecFieldTile.erase(iter);

						SafeDelete(pTile);
						pTile = nullptr;
						return;
					}
					else
						++iter;

					if (iter == m_vecFieldTile.end())
					{ // 순회해서 끝에 도달헀다면 해당하는 타일이없는것이다.
						SafeDelete(pTile);
						pTile = nullptr;
						return;
					}
				}
			}
		}
		else if (_Option > 0)
		{
			{
				TILE_INFO* pTile;
				pTile = nullptr;
				pTile = new TILE_INFO;
				pTile->vPos.x = m_vecTile[iIndex]->vPos.x;
				pTile->vPos.y = m_vecTile[iIndex]->vPos.y;
				pTile->vSize = { m_vecTile[iIndex]->vSize.x, m_vecTile[iIndex]->vSize.y, 0.f };

				pTile->TILEX1 = (int)m_vecTile[iIndex]->vPos.x;
				pTile->TILEY1 = (int)m_vecTile[iIndex]->vPos.y;
				pTile->TILECX = m_vecTile[iIndex]->TILECX;
				pTile->TILECY = m_vecTile[iIndex]->TILECY;

				for (auto iter = m_vecOptionTile.begin(); iter != m_vecOptionTile.end();)
				{
					if ((*iter)->vPos == pTile->vPos)
					{
						m_vecOptionTile.erase(iter);

						SafeDelete(pTile);
						pTile = nullptr;
						return;
					}
					else
						++iter;

					if (iter == m_vecOptionTile.end())
					{ // 순회해서 끝에 도달헀다면 해당하는 타일이없는것이다.
						SafeDelete(pTile);
						pTile = nullptr;
						return;
					}
				}
			}
		}
	}
}

HRESULT CTerrain::SaveTile(const TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwBytes = 0;
//	size_t iStringLen = 0;
	size_t ImgTile_Size = 0;
	for (size_t j = 0; j < m_vecFieldTile.size(); ++j)
	{
		ImgTile_Size++;
	}

	WriteFile(hFile, &ImgTile_Size, sizeof(size_t), &dwBytes, nullptr);
	for (auto& pTile : m_vecFieldTile)
	{
		WriteFile(hFile, pTile, sizeof(TILE_INFO), &dwBytes, nullptr);
	}
	size_t OptionTile_Size = 0;

	for (size_t k = 0; k < m_vecOptionTile.size(); ++k)
	{
		OptionTile_Size++;
	}

	WriteFile(hFile, &OptionTile_Size, sizeof(size_t), &dwBytes, nullptr);
	for (auto& pTile : m_vecOptionTile)
	{
		WriteFile(hFile, pTile, sizeof(TILE_INFO), &dwBytes, nullptr);
	}


	CloseHandle(hFile);

	

	return S_OK;
}

HRESULT CTerrain::LoadTile(const TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	if (!m_vecFieldTile.empty())
		FieldTileRelease();
	if (!m_vecOptionTile.empty())
		OptionTileRelease();


	size_t size_Tile = 0;
	DWORD dwBytes = 0;
	TILE_INFO* pTile = nullptr;


	ReadFile(hFile, &size_Tile, sizeof(size_t), &dwBytes, nullptr);

	for (size_t j = 0; j < size_Tile; ++j)
	{
		pTile = new TILE_INFO;
		ZeroMemory(pTile, sizeof(TILE_INFO));

		ReadFile(hFile, pTile, sizeof(TILE_INFO), &dwBytes, nullptr);

		if ((j-1) == size_Tile)
		{
			SafeDelete(pTile);
			break;
		}

		m_vecFieldTile.push_back(pTile);
	}

	// 옵션타일 로드
	size_t OptionTile_Size = 0;

	ReadFile(hFile, &OptionTile_Size, sizeof(size_t), &dwBytes, nullptr);

	for (size_t k = 0; k < OptionTile_Size; ++k)
	{
		pTile = new TILE_INFO;
		ZeroMemory(pTile, sizeof(TILE_INFO));

		ReadFile(hFile, pTile, sizeof(TILE_INFO), &dwBytes, nullptr);

		if ((k - 1) == OptionTile_Size)
		{
			SafeDelete(pTile);
			break;
		}
		m_vecOptionTile.push_back(pTile);
	}

	CloseHandle(hFile);
	return S_OK;

}

HRESULT CTerrain::Initialize()
{

	m_vecTile.reserve(TILEX * TILEY);


	TILE_INFO* pTile = nullptr;

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			pTile = new TILE_INFO;
			ZeroMemory(pTile, sizeof(TILE_INFO));

			pTile->vPos.x = (j*TILECX1) + (TILECX1*0.5f);
			pTile->vPos.y = (i*TILECY1) + (TILECY1*0.5f);
			pTile->vSize = { 1.f, 1.f, 0.f };
			pTile->byDrawID = 0; // 처음에 DrawID를 0로 초기화 해줬고
			pTile->TILEX1 = (int)TILEX;
			pTile->TILEY1 = (int)TILEY;
			pTile->TILECX = (float)TILECX1;
			pTile->TILECY = (float)TILECY1;

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE_INFO*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
	for_each(m_vecFieldTile.begin(), m_vecFieldTile.end(), SafeDelete<TILE_INFO*>);
	m_vecFieldTile.clear();
	m_vecFieldTile.shrink_to_fit();
}

void CTerrain::TileRelease()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE_INFO*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTerrain::FieldTileRelease()
{
	for_each(m_vecFieldTile.begin(), m_vecFieldTile.end(), SafeDelete<TILE_INFO*>);
	m_vecFieldTile.clear();
	m_vecFieldTile.shrink_to_fit();
}


void CTerrain::OptionTileRelease()
{
	for_each(m_vecOptionTile.begin(), m_vecOptionTile.end(), SafeDelete<TILE_INFO*>);
	m_vecOptionTile.clear();
	m_vecOptionTile.shrink_to_fit();
}

int CTerrain::SelectIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (IsPicking(vPos, i))
			return i;
	}

	return -1;
}

bool CTerrain::IsPicking(const D3DXVECTOR3& vPos, const size_t& index)
{
	// 내적 (데카르트 기준)

	// 마름모의 꼭지점 12시, 3시, 6시, 9시 구성
	D3DXVECTOR3 vPoint[4] =
	{
		{ m_vecTile[index]->vPos.x, m_vecTile[index]->vPos.y + m_vecTile.front()->TILECY * 0.5f, 0.f },
		{ m_vecTile[index]->vPos.x + m_vecTile.front()->TILECX * 0.5f, m_vecTile[index]->vPos.y, 0.f },
		{ m_vecTile[index]->vPos.x, m_vecTile[index]->vPos.y - m_vecTile.front()->TILECY * 0.5f, 0.f },
		{ m_vecTile[index]->vPos.x - m_vecTile.front()->TILECX * 0.5f, m_vecTile[index]->vPos.y, 0.f }
	};

	// 시계 방향으로 마름모의 4개 방향벡터 구성
	D3DXVECTOR3 vDir[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	// 위에서 만든 4개의 방향벡터와 각각 직각을 이루는 법선 벡터 설정.
	//  2D에서 법선 벡터 구하는 방법.
	/*( X, Y ) -> ( -Y, X ), ( Y, -X )
	( 1, 0 ) -> ( 0, 1 ), ( 0, -1 )
	( 3, 3 ) -> ( -3, 3 ), ( 3, -3 )*/

	// 외적을 이용해서 법선벡터 구해보자!
	D3DXVECTOR3 vNormal[4] = {};

	for (int i = 0; i < 4; ++i)
		D3DXVec3Cross(&vNormal[i], &D3DXVECTOR3(0.f, 0.f, 1.f), &vDir[i]);

	// 각 꼭지점과의 마우스 방향벡터
	D3DXVECTOR3 vMouseDir[4] =
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	// vMouseDir와 vNormal를 각각 내적해서 모두 둔각이면 true
	for (int i = 0; i < 4; ++i)
	{
		if (0.f < D3DXVec3Dot(&vMouseDir[i], &vNormal[i]))
			return false;
	}// 이거를 지나가면 true니까

	return true;
}

CTerrain* CTerrain::Create(CToolView* pToolView)
{
	NULL_CHECK_RETURN(pToolView, nullptr);

	CTerrain* pInstance = new CTerrain;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->SetToolView(pToolView);

	return pInstance;
}
