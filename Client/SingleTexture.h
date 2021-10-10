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
		const wstring& wstrStateKey = L"", /* ��Ƽ �ؽ�ó�� ��� */
		const int& iIndex = 0 /* ��Ƽ �ؽ�ó�� ��� */) const override;

private:
	// CTexture��(��) ���� ��ӵ�
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

