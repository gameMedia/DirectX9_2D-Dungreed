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

	// �̹��� ���Ϸκ��� �̹��� ������ ������ �Լ�.
	HRESULT hr = D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &(m_pTexInfo->tImgInfo));
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXGetImageInfoFromFile Failed", E_FAIL);

	// �̹��� ���� �ε��ϸ鼭 IDirect3DTexture9 ��ü ����.
	hr = D3DXCreateTextureFromFileEx(
		m_pDeviceMgr->GetDevice(), /* ��ġ ���� */
		wstrFilePath.c_str(), /* �̹��� ��� */
		m_pTexInfo->tImgInfo.Width, /* �̹��� ���� �ʺ� */
		m_pTexInfo->tImgInfo.Height, /* �̹��� ���� */
		m_pTexInfo->tImgInfo.MipLevels, /* �Ӹ��� ���� */
		0, /* �ؽ�ó ��� ���. Ư���� ��찡 �ƴ� �̻� 0 */
		m_pTexInfo->tImgInfo.Format, /* �̹����� �ȼ� ����*/
		D3DPOOL_MANAGED, /* �޸� ��� ��� */
		D3DX_DEFAULT,	/* �̹��� ���� */
		D3DX_DEFAULT,	/* �Ӹ� ���� */
		0,	/* ������ ���� */
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
