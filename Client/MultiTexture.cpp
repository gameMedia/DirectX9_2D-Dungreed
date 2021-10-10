#include "stdafx.h"
#include "MultiTexture.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEX_INFO * CMultiTexture::GetTexInfo(
	const wstring& wstrStateKey, 
	const int& iIndex) const
{
	auto iter_find = m_mapMultiTex.find(wstrStateKey);

	if (m_mapMultiTex.end() == iter_find)
		return nullptr;

	return iter_find->second[iIndex];
}

// ex) wstrFilePath = ..\Texture\Stage\Effect\Crash\Crash%d.png
HRESULT CMultiTexture::LoadTexture(
	const wstring& wstrFilePath, 
	const wstring& wstrStateKey, 
	const int& iCount)
{
	auto iter_find = m_mapMultiTex.find(wstrStateKey);

	if (m_mapMultiTex.end() != iter_find)
	{
		MSG_BOX(wstrStateKey.c_str());
		MSG_BOX(L"이미 중복 되었음");
		return E_FAIL;
	}

	TCHAR szFullPath[MAX_STR] = L"";
	TEX_INFO* pTexInfo = nullptr;

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, wstrFilePath.c_str(), i);

		pTexInfo = new TEX_INFO;
		ZeroMemory(pTexInfo, sizeof(TEX_INFO));

		// 이미지 파일로부터 이미지 정보를 얻어오는 함수.
		HRESULT hr = D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo));
		FAILED_CHECK_MSG_RETURN(hr, L"D3DXGetImageInfoFromFile Failed", E_FAIL);

		// 이미지 파일 로드하면서 IDirect3DTexture9 객체 생성.
		hr = D3DXCreateTextureFromFileEx(
			m_pDeviceMgr->GetDevice(), /* 장치 전달 */
			szFullPath, /* 이미지 경로 */
			pTexInfo->tImgInfo.Width, /* 이미지 가로 너비 */
			pTexInfo->tImgInfo.Height, /* 이미지 높이 */
			pTexInfo->tImgInfo.MipLevels, /* 밉맵의 정도 */
			0, /* 텍스처 사용 방식. 특별한 경우가 아닌 이상 0 */
			pTexInfo->tImgInfo.Format, /* 이미지의 픽셀 포맷*/
			D3DPOOL_MANAGED, /* 메모리 사용 방식 */
			D3DX_DEFAULT,	/* 이미지 필터 */
			D3DX_DEFAULT,	/* 밉맵 필터 */
			0,	/* 제거할 색상 */
			nullptr,
			nullptr,
			&(pTexInfo->pTexture));
		FAILED_CHECK_MSG_RETURN(hr, wstrFilePath.c_str(), E_FAIL);

		/*m_mapMultiTex.insert({ wstrStateKey, VEC_TEXINFO() });
		iter_find = m_mapMultiTex.find(wstrStateKey);
		iter_find->second.push_back(pTexInfo);*/

		m_mapMultiTex[wstrStateKey].push_back(pTexInfo);
	}	

	return S_OK;
}

void CMultiTexture::Release()
{
	for_each(m_mapMultiTex.begin(), m_mapMultiTex.end(),
		[](auto& MyPair)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(),
			[](auto& pTexInfo)
		{
			pTexInfo->pTexture->Release();
			SafeDelete(pTexInfo);
		});

		MyPair.second.clear();
		MyPair.second.shrink_to_fit();
	});

	m_mapMultiTex.clear();
}

CMultiTexture* CMultiTexture::Create(
	const wstring & wstrFilePath, 
	const wstring & wstrStateKey, 
	const int & iCount)
{
	CMultiTexture* pInstance = new CMultiTexture;

	if (FAILED(pInstance->LoadTexture(wstrFilePath, wstrStateKey, iCount)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
