#pragma once
class CTexture
{
public:
	enum TEXTURE_TYPE { TEX_SINGLE, TEX_MULTI };

protected:
	explicit CTexture();

public:
	virtual ~CTexture();

public:
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"", /* 멀티 텍스처인 경우 */
		const int& iIndex = 0 /* 멀티 텍스처인 경우 */) const PURE;

public:
	virtual HRESULT LoadTexture(
		const wstring& wstrFilePath,	/* 불러올 이미지 경로 */
		const wstring& wstrStateKey = L"", /* 멀티 텍스처인 경우 */
		const int& iCount = 0 /* 멀티 텍스처인 경우 */ ) PURE;
	virtual void Release() PURE;

protected:
	CDeviceMgr*	m_pDeviceMgr;
};

