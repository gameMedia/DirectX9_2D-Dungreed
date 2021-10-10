#pragma once
class CField
{
protected:
	explicit CField();

public:
	virtual ~CField();

public:
	// Get
	virtual D3DXVECTOR3& Get_Pos() = 0;
public:
	// Set
	
public:
	virtual int Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render() = 0;

protected:
	virtual HRESULT Initialize() = 0;
	virtual HRESULT LateInitialize();
	virtual void Release() = 0;
public:
	// ÀÛ¾÷
	virtual void MakeWorldMatrix();
	virtual void Set_Map() = 0;
protected:
	MAP_INFO RenderMap;
	UNIT_INFO m_tMap;
	FRAME m_tFrame;
	CTimeMgr*	m_pTimeMgr;
	CDeviceMgr* m_pDeviceMgr;
	CTextureMgr* m_pTextureMgr;
	float m_fSpeed;
	bool m_bIsInit;
	D3DXVECTOR3 playerPos;
};

