#pragma once
class CObj
{
protected:
	explicit CObj();

public:
	virtual ~CObj();
public:
	// Get
	
public:
	// Set
	void Set_Time() { Current_tick = clock(); }
	void Set_CollisionRect(bool _bcoll) { m_bColl = true; }
	void Set_Pos(float x, float y)
	{
		m_tUnit.vCollPos.x = x;
		m_tUnit.vCollPos.y =( y + m_tUnit.vLength.y*0.5f);
		m_tUnit.vPos.x = x;
		m_tUnit.vPos.y = y;

	}
	void Set_Dead(bool _bDead)
	{
		m_bIsDead = _bDead;
	}
	const D3DXVECTOR3& Get_Pos() const { return m_tUnit.vPos; }
	const UNIT_INFO& Get_Info() const { return m_tUnit; }
	const RECT& Get_Rect() const { return m_tRect; }
	const INFO& Get_UnitData() const { return m_tInfo; }
	const ITEM_DATA& Get_ItemData() const { return m_tItemData; }
	
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
	// 충돌이벤트 함수
	virtual void RectColl(CObj* pObj = nullptr);
	// 추가될것

protected:
	
	UNIT_INFO m_tUnit;
	INFO m_tInfo; // Unit의 정보? 공격력 체력 등
	ITEM_DATA m_tItemData;
	int iAttack;
	RECT m_tRect;
	FRAME m_tFrame;
	CTimeMgr*		m_pTimeMgr;
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
	float m_fSpeed;
	
	
	
	
	// API
	bool m_bIsDead;

	float m_fAngle;
	CObj* m_pTarget;
	bool m_bIsInit;

	clock_t Current_tick; // 프로세스가 얼마나 진행했는지 계산해줄 clock_t 자료형 변수
	clock_t Next_tick; // 프로세스가 얼마나 진행했는지 계산해줄 clock_t 자료형 변수
	bool m_bColl;

	bool LateInit; // 후에 Init할걸 여기서 하자
};

