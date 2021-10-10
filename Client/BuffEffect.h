#pragma once
#include "Effect.h"
class CBuffEffect :
	public CEffect
{
private:
	CBuffEffect(CEffectImpl* pImp, const D3DXMATRIX* pParentMatrix);

public:
	virtual ~CBuffEffect();

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
	static CBuffEffect* Create(
		CEffectImpl* pImp, const D3DXMATRIX* pParentMatrix);

private:
	const D3DXMATRIX*	m_pParentMatrix;
	float				m_fRevAngleZ;	// ���� ��
	float				m_fRevSpd;		// ���� �ӵ�
};

