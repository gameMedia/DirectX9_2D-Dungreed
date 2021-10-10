#pragma once
#include "Obj.h"
class CIceSpear :
	public CObj
{
private:
	CIceSpear();

public:
	virtual ~CIceSpear();

public:
	const UNIT_INFO& GetInfo() const;

public:
	void SetAngle(const float& Angle);
	void SetPos(const D3DXVECTOR3& vPos);
	void SetDir(const D3DXVECTOR3& vDir);
	void SetParent(const D3DXMATRIX& _Parent);
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
	static CIceSpear* Create(
		const D3DXVECTOR3 vDir,
		const D3DXVECTOR3 vPos
	);

private:
	D3DXMATRIX matScroll;
	D3DXVECTOR2 m_vOrigin[5];
	D3DXVECTOR2 WarningLine[2];
	float WarningLineWidth; // 위험을 알려주는 선의 굵기

	bool InitOneTIme;
	bool MakeLinettt;
	float CreateTimeSpear;
	float EndTimeSpear;
};
