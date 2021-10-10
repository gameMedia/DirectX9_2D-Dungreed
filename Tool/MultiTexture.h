#pragma once
#include "Texture.h"
class CMultiTexture :
	public CTexture
{
private:
	explicit CMultiTexture();

public:
	virtual ~CMultiTexture();

public:
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"", /* 멀티 텍스처인 경우 */
		const int& iIndex = 0 /* 멀티 텍스처인 경우 */) const;

public:
	virtual HRESULT LoadTexture(
		const wstring& wstrFilePath,	/* 불러올 이미지 경로 */
		const wstring& wstrStateKey = L"", /* 멀티 텍스처인 경우 */
		const int& iCount = 0 /* 멀티 텍스처인 경우 */);
	virtual void Release();

public:
	static CMultiTexture* Create(
		const wstring& wstrFilePath,
		const wstring& wstrStateKey,
		const int& iCount);

private:
	typedef vector<TEX_INFO*>			VEC_TEXINFO;
	typedef map<wstring, VEC_TEXINFO>	MAP_MULTITEX;
	MAP_MULTITEX						m_mapMultiTex;
};

