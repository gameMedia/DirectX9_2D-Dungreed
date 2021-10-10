#pragma once
#include "Obj.h"

class CEffectImpl;
class CEffect :
	public CObj
{
protected:
	CEffect(CEffectImpl* pImp);
public:
	virtual ~CEffect();
public:
	void SetPos(const D3DXVECTOR3& vPos);
	void SetSize(const D3DXVECTOR3& vSize);
	void SetScale(const D3DXMATRIX& _matScale);
	void SetTrans(const D3DXMATRIX& _matTrans);
	void SetRotZ(const D3DXMATRIX& _matRotZ);
	void SetmatPlanetZ(const D3DXMATRIX& _matPlanetZ);
	void SetLength(const D3DXVECTOR3& _vLength);
public:
	// CObj을(를) 통해 상속됨
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;

protected:
	virtual HRESULT Initialize() PURE;
	virtual void Release() ;

protected:
	CEffectImpl*	m_pImplementor;
	UNIT_INFO m_tEffect;

};

