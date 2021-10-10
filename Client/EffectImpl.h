#pragma once
class CEffectImpl
{
protected:
	CEffectImpl(const wstring& wstrObjectKey);
public:
	virtual ~CEffectImpl();

public:
	virtual int Update() PURE;
	virtual void Render(const D3DXMATRIX* pWorldMatrix) PURE;

protected:
	wstring m_wstrObjectKey;
	float playSpeed;
};

