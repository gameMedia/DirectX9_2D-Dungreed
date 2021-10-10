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
		const wstring& wstrStateKey = L"", /* ��Ƽ �ؽ�ó�� ��� */
		const int& iIndex = 0 /* ��Ƽ �ؽ�ó�� ��� */) const;

public:
	virtual HRESULT LoadTexture(
		const wstring& wstrFilePath,	/* �ҷ��� �̹��� ��� */
		const wstring& wstrStateKey = L"", /* ��Ƽ �ؽ�ó�� ��� */
		const int& iCount = 0 /* ��Ƽ �ؽ�ó�� ��� */);
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

