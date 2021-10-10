#pragma once
#include "Obj.h"
class CIceBullet :
	public CObj
{
private:
	CIceBullet();

public:
	virtual ~CIceBullet();

public:
	const UNIT_INFO& GetInfo() const;

public:
	void SetAngle(const float& Angle);
	void SetPos(const D3DXVECTOR3& vPos);
	void SetDir(const D3DXVECTOR3& vDir);
	void SetParent(const D3DXMATRIX& _Parent);
	void SetSpeed(const float& Speed);
	virtual void RectColl(CObj* pObj = nullptr);
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize() override;
	virtual void Release() override;

private:
	// 월드행렬 만들어준다.
	void MoveBullet();
	void MakeWorldMatrix();
	void MakeLine();
public:
	static CIceBullet* Create(
		const D3DXMATRIX ParentMatrix,
		const D3DXVECTOR3 vDir,
		const D3DXVECTOR3 vPos,
		const float Speed = 400.f
		);

private:
	D3DXMATRIX matScroll;
	D3DXVECTOR2 m_vOrigin[5];

	float BulletSpeed;
};
