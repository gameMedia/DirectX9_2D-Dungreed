#pragma once
class CObj;
class CObjMgr
{
public:
	static CObjMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
		{
			m_pInstance = new CObjMgr;
		}
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}


private: 
	// 다른데서 매니저를 생성못하게
	CObjMgr();
	~CObjMgr();

public:
	CObj* Get_Mouse() const { return m_listObj[OBJID::MOUSE].front(); }
	CObj* Get_Player() const { return m_listObj[OBJID::PLAYER].front(); }
	CObj* Get_Terrain() const { return m_listObj[OBJID::TERRAIN].front(); }
	CObj* Get_SubHand() const { return m_listObj[OBJID::SUBHAND].front(); }
	CObj* Get_MainHand() const { return m_listObj[OBJID::MAINHAND].front(); }
	list<CObj*> Get_SkullDog() const { return m_listObj[OBJID::SKULLDOG]; }
	
public: 
	// set함수
	void Set_Tile(vector<TILE_INFO*>* _pOptionTile) { m_vecOptionTile = _pOptionTile; }

public:
	void Initialize();
	int Update();
	void LateUpdate();
	void Render();
	void Release();
public:
	HRESULT Add_Object(OBJID::ID eID, CObj* pObj);
	HRESULT Delete_Object(OBJID::ID eID);
private:
	static CObjMgr* m_pInstance;
	OBJLIST m_listObj[OBJID::END];
	vector<TILE_INFO*>* m_vecOptionTile = nullptr;
};

