#pragma once
#include "Effect.h"
class CDashEffect :
	public CEffect
{
private:
	CDashEffect(CEffectImpl* pImp);

public:
	virtual ~CDashEffect();

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
	static CDashEffect* Create(
		CEffectImpl* pImp,
		const D3DXVECTOR3& vPos,
		const D3DXVECTOR3& vSize);

private:


};

