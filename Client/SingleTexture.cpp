#include "stdafx.h"
#include "SingleTexture.h"


CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

const TEX_INFO * CSingleTexture::GetTexInfo(
	const wstring & wstrStateKey, 
	const int & iIndex) const
{
	return m_pTexInfo;
}

HRESULT CSingleTexture::LoadTexture(
	const wstring & wstrFilePath, 
	const wstring & wstrStateKey/* = L""*/, 
	const int & iCount/* = 0*/)
{
	m_pTexInfo = new TEX_INFO;
	ZeroMemory(m_pTexInfo, sizeof(TEX_INFO)); 

	// 이미지 파일로부터 이미지 정보를 얻어오는 함수.
	HRESULT hr = D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &(m_pTexInfo->tImgInfo));
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXGetImageInfoFromFile Failed", E_FAIL);

	// 이미지 파일 로드하면서 IDirect3DTexture9 객체 생성.
	hr = D3DXCreateTextureFromFileEx(
		m_pDeviceMgr->GetDevice(), /* 장치 전달 */
		wstrFilePath.c_str(), /* 이미지 경로 */
		m_pTexInfo->tImgInfo.Width, /* 이미지 가로 너비 */
		m_pTexInfo->tImgInfo.Height, /* 이미지 높이 */
		m_pTexInfo->tImgInfo.MipLevels, /* 밉맵의 정도 */
		0, /* 텍스처 사용 방식. 특별한 경우가 아닌 이상 0 */
		m_pTexInfo->tImgInfo.Format, /* 이미지의 픽셀 포맷*/
		D3DPOOL_MANAGED, /* 메모리 사용 방식 */
		D3DX_DEFAULT,	/* 이미지 필터 */
		D3DX_DEFAULT,	/* 밉맵 필터 */
		0,	/* 제거할 색상 */
		nullptr,
		nullptr,
		&(m_pTexInfo->pTexture));
	FAILED_CHECK_MSG_RETURN(hr, wstrFilePath.c_str(), E_FAIL);

	return S_OK;
}

void CSingleTexture::Release()
{
	m_pTexInfo->pTexture->Release();
	SafeDelete(m_pTexInfo);
}

CSingleTexture* CSingleTexture::Create(
	const wstring & wstrFilePath, 
	const wstring & wstrStateKey, 
	const int & iCount)
{
	CSingleTexture* pInstance = new CSingleTexture;

	if (FAILED(pInstance->LoadTexture(wstrFilePath, wstrStateKey, iCount)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
