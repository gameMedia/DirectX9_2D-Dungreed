#pragma once
class CUI;
class CUIMgr
{
	DEFINE_SINGLETONE(CUIMgr)

private:
	CUIMgr();
	~CUIMgr();

public:
	// Get
	CUI* Get_SetButton() { return m_listUI[UIID::SetButton].front(); }
	CUI* Get_ExitButton() { return m_listUI[UIID::ExitButton].front(); }
public:
	//Set

public:
	void Initialize();
	int Update();
	void LateUpdate();
	void Render();
	void Release();

public:
	HRESULT Add_Object(UIID::ID eID, CUI* pUi);

	HRESULT Delete_Object(UIID::ID eID);
private:
	typedef list<CUI*>	UILIST; 
	typedef UILIST::iterator UIITER;
	UILIST m_listUI[UIID::END]; // 항상 출력되는 UI들의 집합
	
};

