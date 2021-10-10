#pragma once
#include "Effect.h"
class CDamageEffect :
	public CEffect
{
private:
	CDamageEffect(CEffectImpl* pImp);

public:
	virtual ~CDamageEffect();

public:
	// CEffect을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize() override;
	virtual void Release() override;

private:
	// 작업
	void Set_Dir(float _Angle);

public:
	static CDamageEffect* Create(
		CEffectImpl* pImp,
		const D3DXVECTOR3 & vPos, 
		const D3DXVECTOR3 & vShowPos
	);

private:
	float CreateTimeUpPos;
	float UpSpeed;

};
