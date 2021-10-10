#pragma once

#include "Texture.h"

class CTextureMgr
{
	DEFINE_SINGLETONE(CTextureMgr)

private:
	explicit CTextureMgr();
	~CTextureMgr();

public:
	const TEX_INFO* GetTexInfo(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"", /* ��Ƽ �ؽ�ó�� ��� */
		const int& iIndex = 0 /* ��Ƽ �ؽ�ó�� ��� */) const;
	
public:
	HRESULT LoadFromImgPath(const wstring& wstrImgPath);

	HRESULT LoadTexture(
		CTexture::TEXTURE_TYPE eTextureType,
		const wstring& wstrFilePath,	/* �ҷ��� �̹��� ��� */
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"", /* ��Ƽ �ؽ�ó�� ��� */
		const int& iCount = 0 /* ��Ƽ �ؽ�ó�� ��� */);
	
	void Release();

private:
	typedef map<wstring, CTexture*>	MAP_TEXTURE;
	MAP_TEXTURE						m_mapTexture;
};

