#pragma once

#include "Effect.h"

class CNormalEffect:
	public CEffect
{
private:
	CNormalEffect(CEffectImpl* pImp);

public:
	virtual ~CNormalEffect();

public:
	// CEffect을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize();
	virtual void Release();

public:
	static CNormalEffect* Create(
		CEffectImpl* pImp, const D3DXVECTOR3& vPos);


};

