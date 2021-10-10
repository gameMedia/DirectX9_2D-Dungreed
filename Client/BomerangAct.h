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
	// CObj을(를) 통해 상속됨
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

	bool m_tGo; // 부메랑 출발
	bool m_tReturn; // 부메랑이 돌아오기 시작하면 받을 수 있다.
	D3DXVECTOR3 GoDir;

	float m_fAttackPower;
	float m_fAttackTotal;
	float CreateStopTime;
	float DeleteTime;
	bool m_bAttackCheck;
	clock_t Current_tick; // 프로세스가 얼마나 진행했는지 계산해줄 clock_t 자료형 변수
	clock_t Next_tick; // 프로세스가 얼마나 진행했는지 계산해줄 clock_t 자료형 변수
};

