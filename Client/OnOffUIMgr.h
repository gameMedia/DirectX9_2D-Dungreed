#pragma once

class COnOffUI;
class COnOffUIMgr
{
	DEFINE_SINGLETONE(COnOffUIMgr)

private:
	COnOffUIMgr();
	~COnOffUIMgr();

public:
	//Get
	COnOffUI* Get_Inventory() { return m_listOnOffUI.find(ONOFFUIID::ONINVENTORY)->second; }
public:
	//Set

public:
	void Initialize();
	int Update(ONOFFUIID::ID eID);
	void LateUpdate(ONOFFUIID::ID eID);
	void Render(ONOFFUIID::ID eID);
	void Release();

public:
	HRESULT Add_Object(ONOFFUIID::ID eID, COnOffUI* pUi);

	HRESULT Delete_Object(ONOFFUIID::ID eID);
private:
	typedef map<ONOFFUIID::ID, COnOffUI*>	UILIST;
	typedef UILIST::iterator UIITER;
	UILIST m_listOnOffUI; // ²¯´Ù ÄÖ´Ù ÇÒ UI
};

