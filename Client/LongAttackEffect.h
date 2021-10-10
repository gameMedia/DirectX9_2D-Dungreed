#pragma once
#include "Effect.h"
class CLongAttackEffect :
	public CEffect
{
private:
	CLongAttackEffect(CEffectImpl* pImp);

public:
	virtual ~CLongAttackEffect();

public:
	// CEffect을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize() override;
	virtual void Release() override;

public:
	static CLongAttackEffect* Create(
		CEffectImpl* pImp,
		const D3DXVECTOR3& vPos,
		const D3DXVECTOR3& vSize);

private:


};