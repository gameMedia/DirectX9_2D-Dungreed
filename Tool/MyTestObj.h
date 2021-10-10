#pragma once

class CToolView;
class CMyTestObj
{
private:
	explicit CMyTestObj();
public:
	~CMyTestObj();

public:
	void SetToolView(CToolView* pToolView);
public:
	void TestObjRender();
	void Render();
	void ObjDelete(const D3DXVECTOR3& vPos, int &_Option);
	HRESULT SaveObjlist(const TCHAR* pFilePath);
	HRESULT LoadObjlist(const TCHAR* pFilePath);
	void PickObj(const D3DXVECTOR3 & vPos, int& _Option);
public:
	void TestObjSet(
		const D3DXVECTOR3& vPos);

private:
	HRESULT Initialize();
	void Release();
public:
	float TestObjSizeX;
	float TestObjSizeY;
	bool TestObjCheck;
	list<UNIT_SAVE*>	m_listUNIT;
	UNIT_SAVE			m_TestUNIT;
	wstring m_Obj;
	wstring m_State;
	int m_iDrawID;
public:
	static CMyTestObj* Create(CToolView* pToolView);
private:
	CToolView*		m_pToolView;
};

