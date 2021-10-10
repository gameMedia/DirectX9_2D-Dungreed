#pragma once
#include "EffectImpl.h"
class CAlphaAnimationImpl :
	public CEffectImpl
{
private:
	CAlphaAnimationImpl(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey,
		const FRAME& tFrame,
		float _playSpeed = 1.f);
public:
	virtual ~CAlphaAnimationImpl();

public:
	// CEffectImpl��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void Render(const D3DXMATRIX * pWorldMatrix) override;
	FRAME& Get_Frame() { return m_tFrame; }
public:
	static CAlphaAnimationImpl* Create(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey,
		const FRAME& tFrame,
		float _playSpeed = 1.f);

private:
	FRAME m_tFrame;
	wstring m_wstrStateKey;
};

