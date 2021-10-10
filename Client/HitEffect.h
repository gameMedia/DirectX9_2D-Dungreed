#pragma once
#include "Effect.h"
class CHitEffect :
	public CEffect
{
private:
	CHitEffect(CEffectImpl* pImp, D3DXMATRIX* MatTrans);

public:
	virtual ~CHitEffect();

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
	static CHitEffect* Create(
		CEffectImpl* pImp, D3DXMATRIX* MatTrans);

private:
	D3DXMATRIX*	m_pParentTrans;
	float				m_fRevAngleZ;	// 공전 각
	float				m_fRevSpd;		// 공전 속도
};