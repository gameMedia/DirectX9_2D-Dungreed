#pragma once
#include "EffectImpl.h"
class CAnimationImpl :
	public CEffectImpl
{
private:
	CAnimationImpl(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey,
		const FRAME& tFrame,
		float _playSpeed = 1.f);
public:
	virtual ~CAnimationImpl();

public:
	// CEffectImpl을(를) 통해 상속됨
	virtual int Update() override;
	virtual void Render(const D3DXMATRIX * pWorldMatrix) override;
	FRAME& Get_Frame() { return m_tFrame; }
public:
	static CAnimationImpl* Create(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey,
		const FRAME& tFrame,
		float _playSpeed = 1.f);

private:
	FRAME m_tFrame;
	wstring m_wstrStateKey;
};

