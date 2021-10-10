#pragma once
class CTileMgr
{
public:
	DEFINE_SINGLETONE(CTileMgr)

private:
	CTileMgr();
	~CTileMgr();
public:
	void Set_Tile(vector<TILE_INFO*>* _pTile) { m_VecTile = _pTile; }
	void SaveTile();
	void LoadTile();
	void Release();
	void Set_Change(int index, int iDraw, int iOption);
private:
	vector<TILE_INFO*>* m_VecTile;

};

