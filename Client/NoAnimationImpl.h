#pragma once
#include "EffectImpl.h"
class CNoAnimationImpl :
	public CEffectImpl
{
private:
	CNoAnimationImpl(const wstring&wstrObjectKey,
		float fLifeTime,
		float _playSpeed = 1.f);
public:
	virtual ~CNoAnimationImpl();

public:
	// CEffectImpl을(를) 통해 상속됨
	virtual int Update() override;
	virtual void Render(const D3DXMATRIX* pWorldMatrix) override;

public:
	static CNoAnimationImpl* Create(
		const wstring& wstrObjectKey, 
		float fLifeTime,
		float _playSpeed=1.f);

private:
	float m_fLifeTime;
	float m_fTimeCount;

};

