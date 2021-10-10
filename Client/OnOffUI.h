#pragma once
class COnOffUI
{
protected:
	explicit COnOffUI();

public:
	virtual ~COnOffUI();
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

	// �߰��ɰ�

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

};