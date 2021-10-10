#pragma once
#include "Texture.h"
class CSingleTexture :
	public CTexture
{
private:
	explicit CSingleTexture();

public:
	virtual ~CSingleTexture();

public:
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"", /* 멀티 텍스처인 경우 */
		const int& iIndex = 0 /* 멀티 텍스처인 경우 */) const override;

private:
	// CTexture을(를) 통해 상속됨
	virtual HRESULT LoadTexture(
		const wstring & wstrFilePath, 
		const wstring & wstrStateKey = L"", 
		const int & iCount = 0) override;
	virtual void Release() override;

public:
	static CSingleTexture* Create(
		const wstring & wstrFilePath,
		const wstring & wstrStateKey = L"",
		const int & iCount = 0);

private:
	TEX_INFO*	m_pTexInfo;
};

