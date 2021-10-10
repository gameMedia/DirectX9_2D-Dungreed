#pragma once

class CLine;
class CToolView;
class CLineMgr
{
	DEFINE_SINGLETONE(CLineMgr)
private:
	CLineMgr();
	~CLineMgr();
public:
	void Initialize();
	void SetLPoint(
		const D3DXVECTOR3 & vPos,
		int& _Alpha,
		int& _Red, 
		int& _Green, 
		int& _Blue,
		int& _iOption); //

	void SetRPoint(
		const D3DXVECTOR3 & vPos,
		int& _Alpha,
		int& _Red,
		int& _Green,
		int& _Blue,
		int& _iOption); //

	void LateUpdate();
	void Render(CToolView* _pToolView);
	void Release();

public:

	HRESULT SaveData(const TCHAR * pFilePath);
	HRESULT LoadData(const TCHAR * pFilePath);
public:
	bool DrawLineCheck;
	list<CLine*> m_listLine;
};

