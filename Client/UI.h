#pragma once

class CObj;
class CUI
{
protected:
	explicit CUI();

public:
	virtual ~CUI();
public:
	// Get

public:
	// Set
	void Set_Pos(float x, float y)
	{
		m_tUnit.vPos.x = x;
		m_tUnit.vPos.y = y;
	}
	const D3DXVECTOR3& Get_Pos() const { return m_tUnit.vPos; }
	const UNIT_INFO& Get_Info() const { return m_tUnit; }
	void Set_Dead(bool _dead)
	{
		m_bIsDead = _dead;
	}

public:
	// 기본
	virtual int	Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render() = 0;

protected:
	virtual HRESULT Initialize() = 0;
	virtual HRESULT LateInitialize();
	virtual void Release() = 0;
public:
	//작업
	void UpdateRect();
	// float 애니메이션 재생속도
	void FrameMove(float _AniPlaySpeed = 1.f);
	// 월드행렬 만들어준다
	virtual void MakeWorldMatrix();

	void Set_Obj(CObj* _pObj) { m_pObj = _pObj; }

	// 추가될것

protected:

	UNIT_INFO m_tUnit;
	RECT m_tRect;
	FRAME m_tFrame;
	CTimeMgr*		m_pTimeMgr;
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
	float m_fSpeed;

	bool m_bIsDead;



	bool m_bIsInit;

	CObj* m_pObj; // 오브젝트를 받아와서 HPBar만들어줄거임

};
