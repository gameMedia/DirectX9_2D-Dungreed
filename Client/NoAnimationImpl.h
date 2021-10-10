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
	// CEffectImpl��(��) ���� ��ӵ�
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

