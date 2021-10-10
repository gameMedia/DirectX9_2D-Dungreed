#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

INFLEMENT_SIGLETONE(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEX_INFO* CTextureMgr::GetTexInfo(
	const wstring & wstrObjectKey, 
	const wstring & wstrStateKey, 
	const int & iIndex) const
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	if (m_mapTexture.end() == iter_find)
		return nullptr;

	return iter_find->second->GetTexInfo(wstrStateKey, iIndex);
}

HRESULT CTextureMgr::LoadFromImgPath(const wstring & wstrImgPath)
{
#ifdef _UNICODE
	wifstream	fin;
#else 
	ifstream	fin;
#endif

	fin.open(wstrImgPath);

	if (fin.fail())
		return E_FAIL;

	TCHAR szObjectKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";

	TCHAR szImgCount[MAX_STR] = L"";
	TCHAR szRelativePath[MAX_STR] = L"";

	while (true)
	{
		fin.getline(szObjectKey, MAX_STR, '|');
		fin.getline(szStateKey, MAX_STR, '|');
		fin.getline(szImgCount, MAX_STR, '|');
		fin.getline(szRelativePath, MAX_STR);

		if (fin.eof())
			break;

		if (FAILED(LoadTexture(
			CTexture::TEX_MULTI, szRelativePath, szObjectKey,
			szStateKey, _ttoi(szImgCount))))
			return E_FAIL;
	}

	fin.close();

	return S_OK;
}

HRESULT CTextureMgr::LoadTexture(
	CTexture::TEXTURE_TYPE eTextureType, 
	const wstring & wstrFilePath, 
	const wstring & wstrObjectKey, 
	const wstring & wstrStateKey, 
	const int & iCount)
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	if (m_mapTexture.end() == iter_find)
	{
		CTexture* pTexture = nullptr;

		switch (eTextureType)
		{
		case CTexture::TEX_SINGLE:
			pTexture = CSingleTexture::Create(wstrFilePath);
			break;
		case CTexture::TEX_MULTI:
			pTexture = CMultiTexture::Create(wstrFilePath, wstrStateKey, iCount);
			break;
		}

		NULL_CHECK_RETURN(pTexture, E_FAIL);
		m_mapTexture[wstrObjectKey] = pTexture;
	}
	else if (CTexture::TEX_MULTI == eTextureType)
	{
		HRESULT hr = m_mapTexture[wstrObjectKey]->LoadTexture(
			wstrFilePath, wstrStateKey, iCount);
		FAILED_CHECK_RETURN(hr, E_FAIL);
	}

	return S_OK;
}

void CTextureMgr::Release()
{
	for_each(m_mapTexture.begin(), m_mapTexture.end(), 
		[](auto& MyPair)
	{
		SafeDelete(MyPair.second);
	});

	m_mapTexture.clear();
}
