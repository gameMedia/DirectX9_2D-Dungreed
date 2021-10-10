#include "stdafx.h"
#include "Terrain.h"



CTerrain::CTerrain()
	: RectTileCheck(false)
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



void CTerrain::Render()
{
	D3DXMATRIX matScale, matTrans;
	const TEX_INFO* pTexInfo = nullptr;
	const TEX_INFO* pMain = nullptr;
	TCHAR szIndex[MIN_STR] = L"";
	UNIT_INFO playerpos = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	for (size_t j = 0; j < m_vecFieldTile.size(); ++j)
	{
		D3DXVECTOR3 Dir = { 
			playerpos.vPos.x - m_vecFieldTile[j]->vPos.x,
			playerpos.vPos.y - m_vecFieldTile[j]->vPos.y,0.f };
		float Length = D3DXVec3Length(&Dir);
		if (Length > 750.f)
		{
			
			continue;
		}
		D3DXMatrixScaling(
			&matScale, m_vecFieldTile[j]->vSize.x, m_vecFieldTile[j]->vSize.y, 0.f);
		D3DXMatrixTranslation(
			&matTrans,
			m_vecFieldTile[j]->vPos.x - CScrollMgr::GetScrollPos(0),
			m_vecFieldTile[j]->vPos.y - CScrollMgr::GetScrollPos(1),
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

		//for (size_t k = 0; k < m_vecOptionTile.size(); ++k)
		//{
		//	D3DXMatrixScaling(
		//		&matScale,
		//		m_vecOptionTile[k]->vSize.x,
		//		m_vecOptionTile[k]->vSize.y, 0.f);
		//	D3DXMatrixTranslation(
		//		&matTrans,
		//		m_vecOptionTile[k]->vPos.x - CScrollMgr::GetScrollPos(0),
		//		m_vecOptionTile[k]->vPos.y - CScrollMgr::GetScrollPos(1),
		//		0.f);
		//	m_vecOptionTile[k]->matWorld = matScale * matTrans;
		//	pMain = CTextureMgr::Get_Instance()->GetTexInfo(
		//		L"Terrain", L"OptionTile", m_vecOptionTile[k]->byOption);
		//	NULL_CHECK(pMain);

		//	float CenterX = pMain->tImgInfo.Width*0.5f;
		//	float CenterY = pMain->tImgInfo.Height*0.5f;

		//	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&m_vecOptionTile[k]->matWorld);
		//	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		//		pMain->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
		//		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		//	////폰트 출력
		//	//D3DXMATRIX TextMatWorld, TextMatScale;
		//	//D3DXMatrixScaling(&TextMatScale, 1.f, 1.f, 0.f);
		//	//TextMatWorld = TextMatScale*matTrans;
		//	//swprintf_s(szIndex, L"%d", m_vecOptionTile[k]->byOption);
		//	//CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld);
		//	//CDeviceMgr::Get_Instance()->GetFont()->DrawText(
		//	//	CDeviceMgr::Get_Instance()->GetSprite(), szIndex, lstrlen(szIndex),
		//	//	nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		//}

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

		if ((j - 1) == size_Tile)
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

	//HRESULT hr = LoadTile(L"../Data/Town.dat");
	//FAILED_CHECK_RETURN(hr, E_FAIL);


	//m_vecTile.reserve(TILEX * TILEY);


	//TILE_INFO* pTile = nullptr;

	//for (int i = 0; i < TILEY; ++i)
	//{
	//	for (int j = 0; j < TILEX; ++j)
	//	{
	//		pTile = new TILE_INFO;
	//		ZeroMemory(pTile, sizeof(TILE_INFO));

	//		pTile->vPos.x = (j*TILECX1) + (TILECX1*0.5f);
	//		pTile->vPos.y = (i*TILECY1) + (TILECY1*0.5f);
	//		pTile->vSize = { 1.f, 1.f, 0.f };
	//		pTile->byDrawID = 0; // 처음에 DrawID를 0로 초기화 해줬고
	//		pTile->TILEX1 = (int)TILEX;
	//		pTile->TILEY1 = (int)TILEY;
	//		pTile->TILECX = (float)TILECX1;
	//		pTile->TILECY = (float)TILECY1;

	//		m_vecTile.push_back(pTile);
	//	}
	//}

	return S_OK;
}

int CTerrain::Update()
{
	return 0;
}

void CTerrain::LateUpdate()
{
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




CTerrain* CTerrain::Create()
{

	CTerrain* pInstance = new CTerrain;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
