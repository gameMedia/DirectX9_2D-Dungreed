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
	// CEffect��(��) ���� ��ӵ�
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
	float				m_fRevAngleZ;	// ���� ��
	float				m_fRevSpd;		// ���� �ӵ�
};