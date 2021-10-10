#pragma once
#include "Obj.h"
class CBomerangAct :
	public CObj
{
private:
	explicit CBomerangAct();
public:
	virtual ~CBomerangAct();

public:
	const UNIT_INFO& GetInfo() const;
public:
	void SetAngle(const float& Angle);
	void SetPos(const D3DXVECTOR3& vPos);
	void SetData(const ITEM_DATA& _Main);
	void SetDir(const D3DXVECTOR3& vDir);
	virtual void RectColl(CObj* pObj = nullptr);
public:
	// CObj��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize() override;
	virtual void Release() override;
private:
	void MoveBomerang();
	void MakeWorldMatrix();
	void MakeLine();
	void UpdateRect();
public:
	static CBomerangAct* Create(
		const D3DXVECTOR3 Dir, ITEM_DATA _Main);
private:
	D3DXVECTOR2 m_vOrigin[5];

	bool m_tGo; // �θ޶� ���
	bool m_tReturn; // �θ޶��� ���ƿ��� �����ϸ� ���� �� �ִ�.
	D3DXVECTOR3 GoDir;

	float m_fAttackPower;
	float m_fAttackTotal;
	float CreateStopTime;
	float DeleteTime;
	bool m_bAttackCheck;
	clock_t Current_tick; // ���μ����� �󸶳� �����ߴ��� ������� clock_t �ڷ��� ����
	clock_t Next_tick; // ���μ����� �󸶳� �����ߴ��� ������� clock_t �ڷ��� ����
};

