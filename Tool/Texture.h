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
		const wstring& wstrStateKey = L"", /* ��Ƽ �ؽ�ó�� ��� */
		const int& iIndex = 0 /* ��Ƽ �ؽ�ó�� ��� */) const PURE;

public:
	virtual HRESULT LoadTexture(
		const wstring& wstrFilePath,	/* �ҷ��� �̹��� ��� */
		const wstring& wstrStateKey = L"", /* ��Ƽ �ؽ�ó�� ��� */
		const int& iCount = 0 /* ��Ƽ �ؽ�ó�� ��� */ ) PURE;
	virtual void Release() PURE;

protected:
	CDeviceMgr*	m_pDeviceMgr;
};

