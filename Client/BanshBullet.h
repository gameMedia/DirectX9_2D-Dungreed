#pragma once
#include "Obj.h"
class CBanshBullet :
	public CObj
{
private:
	CBanshBullet();

public:
	virtual ~CBanshBullet();

public:
	const UNIT_INFO& GetInfo() const;

public:
	void SetPos(const D3DXVECTOR3& vPos);
	void SetDir(const D3DXVECTOR3& vDir);
	virtual void RectColl(CObj* pObj = nullptr);
public:
	// CGameObject��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize() override;
	virtual void Release() override;

private:
	// ������� ������ش�.
	void MoveBullet();
	void MakeWorldMatrix();
	void MakeLine();
public:
	static CBanshBullet* Create(
		const D3DXVECTOR3& vPos,
		const D3DXVECTOR3& vDir);

private:

	D3DXVECTOR2 m_vOrigin[5];
};
