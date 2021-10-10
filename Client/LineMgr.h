#pragma once

class CLine;

class CLineMgr
{
	DEFINE_SINGLETONE(CLineMgr)
private:
	CLineMgr();
	~CLineMgr();
public:
	void Initialize();

public:
	void Set_PlayerPastPos(D3DXVECTOR3 _PastPos) { PlayerPastPos = _PastPos; }

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
	void Render();
	void Release();

public:
	bool LineCollision(D3DXVECTOR3 * vPos, float* pOutY);
	bool LineCollisionMove(D3DXVECTOR3 * vPos, float* pOutY);
	bool LineCollisionDash(D3DXVECTOR3 * vPos, float* pOutY);
	HRESULT SaveData(const TCHAR * pFilePath);
	HRESULT LoadData(const TCHAR * pFilePath);
private:
	bool DrawLineCheck;
	list<CLine*> m_listLine;
private:
	D3DXVECTOR3 PlayerPastPos;
};

