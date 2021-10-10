#pragma once
#include "Effect.h"
class CMeleeAttackEffect :
	public CEffect
{
private:
	CMeleeAttackEffect(
		CEffectImpl* pImp,
		D3DXMATRIX pParentMatrix,
		const ITEM_DATA& _Data,
		const float& _Angle);

public:
	virtual ~CMeleeAttackEffect();

public:
	// CEffect��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize() override;
	virtual void Release() override;
	void UpdateRect();
public:
	// �۾�
	void RectColl(CObj* pObj = nullptr);
public:
	static CMeleeAttackEffect* Create(
		CEffectImpl* pImp,
		D3DXMATRIX pParentMatrix,
		const ITEM_DATA& _Data,
		const float& _Angle);

private:
	D3DXMATRIX	m_pParentMatrix;
	D3DXVECTOR2 m_vOrigin[5];
	int			iAtt;	// ���� ���ݽ� �� ������ ���ݷ��� �޾��ش�.
	float		Angle;	// ���� ���� �������� ����
	float				m_fRevAngleZ;	// ���� ��
	float				m_fRevSpd;		// ���� �ӵ�
};

