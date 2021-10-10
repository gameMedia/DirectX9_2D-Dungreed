#pragma once

class CField;
class CFieldMgr
{
	DEFINE_SINGLETONE(CFieldMgr)
private:
	CFieldMgr();
	~CFieldMgr();

public:
	// Get함수
	CField* Get_Dungeon1() { return m_mapField[FIELDTYPE::INDUNGEON].find(FIELDID::DUNGEON1)->second; }
	CField* Get_Dungeon2() { return m_mapField[FIELDTYPE::INDUNGEON].find(FIELDID::DUNGEON2)->second; }
	CField* Get_WeaponShopStage() { return m_mapField[FIELDTYPE::INDUNGEON].find(FIELDID::WEAPON_SHOP)->second; }
	CField* Get_Dungeon3() { return m_mapField[FIELDTYPE::INDUNGEON].find(FIELDID::DUNGEON3)->second; }
	CField* Get_Restaurant() { return m_mapField[FIELDTYPE::INDUNGEON].find(FIELDID::RESTAURANT)->second; }
	CField* Get_Dungeon4() { return m_mapField[FIELDTYPE::INDUNGEON].find(FIELDID::DUNGEON4)->second; }
	CField* Get_Reinforce() { return m_mapField[FIELDTYPE::INDUNGEON].find(FIELDID::REINFORCE)->second; }
	CField* Get_BossStage() { return m_mapField[FIELDTYPE::INDUNGEON].find(FIELDID::BOSSMAP)->second; }
public:
	// set함수

public:
	void Initialize();
	int Update(FIELDTYPE::TYPE eType, FIELDID::ID eID);
	void LateUpdate(FIELDTYPE::TYPE eType, FIELDID::ID eID);
	void Render(FIELDTYPE::TYPE eType,FIELDID::ID eID);
	void Release();

public:
	HRESULT Add_Field(FIELDTYPE::TYPE eType,FIELDID::ID eID, CField* pField);
	
private:
	typedef map<FIELDID::ID, CField*> FIELDLIST;
	typedef FIELDLIST::iterator FIELDITER;
	FIELDLIST m_mapField[FIELDTYPE::END];
};

