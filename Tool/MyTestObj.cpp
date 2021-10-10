#include "stdafx.h"
#include "MyTestObj.h"
#include "ToolView.h"

CMyTestObj::CMyTestObj()
	:TestObjSizeX(100),
	TestObjSizeY(100),
	TestObjCheck(false)
	,m_iDrawID(0)
{
	ZeroMemory(&m_TestUNIT, sizeof(UNIT_SAVE));
	m_Obj = L"Money";
	m_State = L"Bullion";
}


CMyTestObj::~CMyTestObj()
{
}

void CMyTestObj::SetToolView(CToolView * pToolView)
{
	m_pToolView = pToolView;
}

void CMyTestObj::TestObjRender()
{
	if (TestObjCheck == false)
		return;
	D3DXMATRIX matWorld,matScale, matTrans;
	const TEX_INFO* pTexInfo = nullptr;
	
	TCHAR szIndex[MIN_STR] = L"";
	D3DXMatrixScaling(
		&matScale,
		m_TestUNIT.vSize.x,
		m_TestUNIT.vSize.y,
		0.f);
	D3DXMatrixTranslation(
		&matTrans,
		m_TestUNIT.vPos.x - m_pToolView->GetScrollPos(0),
		m_TestUNIT.vPos.y - m_pToolView->GetScrollPos(1),
		0.f);
	matWorld = matScale*matTrans;


	pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
		m_Obj, m_State, m_iDrawID);


	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(125, 255, 255, 255));

}

void CMyTestObj::Render()
{
	D3DXMATRIX matWorld,matScale, matTrans;
	const TEX_INFO* pTexInfo = nullptr;
	TCHAR szIndex[MIN_STR] = L"";
	if (m_listUNIT.empty())
		return;

	for (auto i : m_listUNIT)
	{

		pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
			i->ObjKey, i->StateKey, i->iDrawiD);

		i->vSize.x = (i->X / pTexInfo->tImgInfo.Width);
		i->vSize.y = (i->Y / pTexInfo->tImgInfo.Height);
		D3DXMatrixScaling(
			&matScale, i->vSize.x, i->vSize.y, 0.f);
		D3DXMatrixTranslation(
			&matTrans,
			(i->vPos.x - m_pToolView->GetScrollPos(0)),
			i->vPos.y - m_pToolView->GetScrollPos(1), 0.f);
		matWorld = matScale * matTrans;
		
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		D3DXMATRIX TextMatWorld, TextMatScale;
		D3DXMatrixScaling(&TextMatScale, 1.f, 1.f, 0.f);
		TextMatWorld = TextMatScale*matTrans;
		swprintf_s(szIndex, L"%s", i->strName.c_str());
		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld);
		CDeviceMgr::Get_Instance()->GetFont()->DrawText(
			CDeviceMgr::Get_Instance()->GetSprite(), szIndex, lstrlen(szIndex),
			nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CMyTestObj::ObjDelete(const D3DXVECTOR3 & vPos, int & _Option)
{
}

HRESULT CMyTestObj::SaveObjlist(const TCHAR * pFilePath)
{

#ifdef _UNICODE
	wofstream fout;
#else
	ofstream fout;
#endif

	fout.open(pFilePath);
	
	if (fout.fail())
		return E_FAIL;

	wstring wstrCombined = L"";
	TCHAR szDrawID[MIN_STR] = L"";
	TCHAR szOption[MIN_STR] = L"";
	TCHAR szPosX[MIN_STR] = L"";
	TCHAR szPosY[MIN_STR] = L"";
	TCHAR szSizeX[MIN_STR] = L"";
	TCHAR szSizeY[MIN_STR] = L"";
	TCHAR szX[MIN_STR] = L"";
	TCHAR szY[MIN_STR] = L"";
	for (auto& pUnitInfo : m_listUNIT)
	{
		_itow_s(pUnitInfo->iDrawiD, szDrawID, 10);
		_itow_s(pUnitInfo->iOption, szOption, 10);
		_itow_s(pUnitInfo->vPos.x, szPosX, 10);
		_itow_s(pUnitInfo->vPos.y, szPosY, 10);
		_itow_s(pUnitInfo->vSize.x*100, szSizeX, 10);
		_itow_s(pUnitInfo->vSize.y*100, szSizeY, 10);
		_itow_s(pUnitInfo->X, szX, 10);
		_itow_s(pUnitInfo->Y, szY, 10);
		wstrCombined = pUnitInfo->ObjKey + L"|" + pUnitInfo->StateKey + L"|"
			+ pUnitInfo->strName + L"|" + szDrawID + L"|" + szOption +
			L"|" + szPosX + L"|" + szPosY + L"|" + szSizeX + L"|" + szSizeY + L"|" + szX + L"|" + szY;

		fout << wstrCombined << endl;
	}

	fout.close();
	
	return S_OK;
}

HRESULT CMyTestObj::LoadObjlist(const TCHAR * pFilePath)
{
#ifdef _UNICODE
	wifstream fin;
#else
	ifstream fin;
#endif

	fin.open(pFilePath);

	if (fin.fail())
		return E_FAIL;

	if (!m_listUNIT.empty())
	{
		for_each(m_listUNIT.begin(), m_listUNIT.end(), SafeDelete<UNIT_SAVE*>);
		m_listUNIT.clear();
	}

	wstring wstrCombined = L"";
	TCHAR szObjectKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szName[MAX_STR] = L"";
	TCHAR szDrawID[MIN_STR] = L"";
	TCHAR szOption[MIN_STR] = L"";
	TCHAR szPosX[MIN_STR] = L"";
	TCHAR szPosY[MIN_STR] = L"";
	TCHAR szSizeX[MIN_STR] = L"";
	TCHAR szSizeY[MIN_STR] = L"";
	TCHAR szX[MIN_STR] = L"";
	TCHAR szY[MIN_STR] = L"";

	UNIT_SAVE* pUnitInfo = nullptr;


	while (true)
	{
		fin.getline(szObjectKey, MAX_STR, '|');
		fin.getline(szStateKey, MAX_STR, '|');
		fin.getline(szName, MAX_STR, '|');
		fin.getline(szDrawID, MIN_STR, '|');
		fin.getline(szOption, MIN_STR, '|');
		fin.getline(szPosX, MIN_STR, '|');
		fin.getline(szPosY, MIN_STR, '|');
		fin.getline(szSizeX, MIN_STR, '|');
		fin.getline(szSizeY, MIN_STR, '|');
		fin.getline(szX, MIN_STR, '|');
		fin.getline(szY, MIN_STR);

		if (fin.eof())
			break;

		pUnitInfo = new UNIT_SAVE;
		pUnitInfo->ObjKey = szObjectKey;
		pUnitInfo->StateKey = szStateKey;
		pUnitInfo->strName = szName;
		pUnitInfo->iDrawiD = _ttoi(szDrawID);
		pUnitInfo->iOption = _ttoi(szOption);
		pUnitInfo->vPos.x = _ttoi(szPosX);
		pUnitInfo->vPos.y = _ttoi(szPosY);
		pUnitInfo->vSize.x = _ttoi(szSizeX)/100;
		pUnitInfo->vSize.y = _ttoi(szSizeY)/100;
		pUnitInfo->X = _ttoi(szX);
		pUnitInfo->Y = _ttoi(szY);
		
		m_listUNIT.push_back(pUnitInfo);


	}

	fin.close();
	

	return S_OK;
}

void CMyTestObj::PickObj(const D3DXVECTOR3 & vPos, int & _Option)
{
	if (TestObjCheck == false)
		return;
	const TEX_INFO* pMain = nullptr;
	pMain = CTextureMgr::Get_Instance()->GetTexInfo(
		m_Obj, m_State, m_iDrawID);

	UNIT_SAVE* pUnit;
	pUnit = nullptr;
	pUnit = new UNIT_SAVE;
	pUnit->vPos.x = vPos.x;
	pUnit->vPos.y = vPos.y;
	pUnit->vPos.z = 0.f;
	pUnit->iDrawiD = m_iDrawID;
	pUnit->ObjKey = m_Obj;
	pUnit->StateKey = m_State;
	pUnit->vSize = { m_TestUNIT.vSize.x,m_TestUNIT.vSize.y,0.f };
	pUnit->iOption = _Option;
	pUnit->strName = m_Obj;
	pUnit->X = m_TestUNIT.X;
	pUnit->Y = m_TestUNIT.Y;

	m_listUNIT.emplace_back(pUnit);
}

void CMyTestObj::TestObjSet(const D3DXVECTOR3 & vPos)
{
	const TEX_INFO* pTexInfo = nullptr;
	pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
	m_Obj,m_State,m_iDrawID);

	
	m_TestUNIT.vPos.x = vPos.x;
	m_TestUNIT.vPos.y = vPos.y;
	m_TestUNIT.vSize.x = (TestObjSizeX / pTexInfo->tImgInfo.Width);
	m_TestUNIT.vSize.y = (TestObjSizeY / pTexInfo->tImgInfo.Height);
	m_TestUNIT.X = TestObjSizeX;
	m_TestUNIT.Y = TestObjSizeY;
}

HRESULT CMyTestObj::Initialize()
{
	m_TestUNIT.vPos = { 0.f,0.f,0.f };
	m_TestUNIT.vSize = { 1.f,1.f,0.f };
	return S_OK;
}

void CMyTestObj::Release()
{

}

CMyTestObj * CMyTestObj::Create(CToolView * pToolView)
{
	CMyTestObj* m_pInstance = new CMyTestObj;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	m_pInstance->SetToolView(pToolView);
	return m_pInstance;

}
