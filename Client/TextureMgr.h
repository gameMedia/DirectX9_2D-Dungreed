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
		const wstring& wstrStateKey = L"", /* 멀티 텍스처인 경우 */
		const int& iIndex = 0 /* 멀티 텍스처인 경우 */) const;
public:
	const wstring& GetLoadingMessage() const;
public:
	void SetLoadingMessage(const wstring& wstrLoadingMessage);

public:
	HRESULT LoadFromImgPath(const wstring& wstrImgPath);
	HRESULT LoadTexture(
		CTexture::TEXTURE_TYPE eTextureType,
		const wstring& wstrFilePath,	/* 불러올 이미지 경로 */
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"", /* 멀티 텍스처인 경우 */
		const int& iCount = 0 /* 멀티 텍스처인 경우 */);
	void Release();

private:
	typedef map<wstring, CTexture*>	MAP_TEXTURE;
	MAP_TEXTURE						m_mapTexture;
	wstring							m_wstrLoadingMessage;
};

