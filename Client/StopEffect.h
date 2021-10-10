#pragma once
#include "Effect.h"
class CStopEffect :
	public CEffect
{
private:
	CStopEffect(CEffectImpl* pImp, D3DXMATRIX* pParentMatrix);

public:
	virtual ~CStopEffect();

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
	static CStopEffect* Create(
		CEffectImpl* pImp, D3DXMATRIX* pParentMatrix);

private:
	D3DXMATRIX*	m_pParentMatrix;
	float				m_fRevAngleZ;	// 공전 각
	float				m_fRevSpd;		// 공전 속도
};
