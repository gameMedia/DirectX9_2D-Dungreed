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
	// �⺻
	virtual int	Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render() = 0;
	
protected:
	virtual HRESULT Initialize() = 0;
	virtual HRESULT LateInitialize();
	virtual void Release() = 0;
public:
	//�۾�
	void UpdateRect();
	// float �ִϸ��̼� ����ӵ�
	void FrameMove(float _AniPlaySpeed = 1.f);
	// ������� ������ش�
	virtual void MakeWorldMatrix();
	// �浹�̺�Ʈ �Լ�
	virtual void RectColl(CObj* pObj = nullptr);
	// �߰��ɰ�

protected:
	
	UNIT_INFO m_tUnit;
	INFO m_tInfo; // Unit�� ����? ���ݷ� ü�� ��
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

	clock_t Current_tick; // ���μ����� �󸶳� �����ߴ��� ������� clock_t �ڷ��� ����
	clock_t Next_tick; // ���μ����� �󸶳� �����ߴ��� ������� clock_t �ڷ��� ����
	bool m_bColl;

	bool LateInit; // �Ŀ� Init�Ұ� ���⼭ ����
};

