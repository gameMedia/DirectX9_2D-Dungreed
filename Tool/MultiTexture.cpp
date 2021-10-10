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
		MSG_BOX(L"�̹� �ߺ� �Ǿ���");
		return E_FAIL;
	}

	TCHAR szFullPath[MAX_STR] = L"";
	TEX_INFO* pTexInfo = nullptr;

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, wstrFilePath.c_str(), i);

		pTexInfo = new TEX_INFO;
		ZeroMemory(pTexInfo, sizeof(TEX_INFO));

		// �̹��� ���Ϸκ��� �̹��� ������ ������ �Լ�.
		HRESULT hr = D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo));
		FAILED_CHECK_MSG_RETURN(hr, L"D3DXGetImageInfoFromFile Failed", E_FAIL);

		// �̹��� ���� �ε��ϸ鼭 IDirect3DTexture9 ��ü ����.
		hr = D3DXCreateTextureFromFileEx(
			m_pDeviceMgr->GetDevice(), /* ��ġ ���� */
			szFullPath, /* �̹��� ��� */
			pTexInfo->tImgInfo.Width, /* �̹��� ���� �ʺ� */
			pTexInfo->tImgInfo.Height, /* �̹��� ���� */
			pTexInfo->tImgInfo.MipLevels, /* �Ӹ��� ���� */
			0, /* �ؽ�ó ��� ���. Ư���� ��찡 �ƴ� �̻� 0 */
			pTexInfo->tImgInfo.Format, /* �̹����� �ȼ� ����*/
			D3DPOOL_MANAGED, /* �޸� ��� ��� */
			D3DX_DEFAULT,	/* �̹��� ���� */
			D3DX_DEFAULT,	/* �Ӹ� ���� */
			0,	/* ������ ���� */
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
