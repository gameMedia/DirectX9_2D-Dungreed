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
	// CEffect을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize() override;
	virtual void Release() override;
	void UpdateRect();
public:
	// 작업
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
	int			iAtt;	// 근접 공격시 그 무기의 공격력을 받아준다.
	float		Angle;	// 근접 공격 했을때의 각도
	float				m_fRevAngleZ;	// 공전 각
	float				m_fRevSpd;		// 공전 속도
};

