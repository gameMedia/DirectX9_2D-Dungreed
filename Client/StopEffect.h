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
	// CEffect��(��) ���� ��ӵ�
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
	float				m_fRevAngleZ;	// ���� ��
	float				m_fRevSpd;		// ���� �ӵ�
};
