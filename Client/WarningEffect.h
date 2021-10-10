#pragma once
#include "Effect.h"
class CWarningEffect :
	public CEffect
{
private:
	CWarningEffect(CEffectImpl* pImp);

public:
	virtual ~CWarningEffect();

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
	static CWarningEffect* Create(
		CEffectImpl* pImp,
		const D3DXVECTOR3& vPos,
		const D3DXVECTOR3& vSize);

private:


};

