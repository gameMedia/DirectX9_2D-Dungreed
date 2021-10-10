#pragma once
#include "Obj.h"
class CBatBullet :
	public CObj
{
private:
	CBatBullet();

public:
	virtual ~CBatBullet();

public:
	const UNIT_INFO& GetInfo() const;

public:
	void SetAngle(const float& Angle);
	void SetPos(const D3DXVECTOR3& vPos);
	void SetDir(const D3DXVECTOR3& vDir);
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
	static CBatBullet* Create(
		const D3DXVECTOR3& vPos,
		const D3DXVECTOR3 vDir
		, const float Angle);

private:

	D3DXVECTOR2 m_vOrigin[5];
};
