#pragma once

class CToolView;
class CTerrain
{
private:
	explicit CTerrain();

public:
	~CTerrain();

public:
	void SetToolView(CToolView* pToolView);

public:
	void TileAllChange(
		const int& TILE_X, const int& TILE_Y
		, const float& TILE_CX, const float& TILE_CY,
		const BYTE& byDrawID, const BYTE& byOption = 0);

	void Render();
	void MiniRender();
	void TileChange(
		const D3DXVECTOR3& vPos, const BYTE& byDrawID, const BYTE& byOption = 0);
	void ImgTileDelete(const D3DXVECTOR3& vPos,int & _Option);
	HRESULT SaveTile(const TCHAR* pFilePath);
	HRESULT LoadTile(const TCHAR* pFilePath);

private:
	HRESULT Initialize();
	
	///// Release
	void Release();
	void TileRelease();
	void FieldTileRelease();
	void OptionTileRelease();
	/////

	int SelectIndex(const D3DXVECTOR3& vPos);
	bool IsPicking(const D3DXVECTOR3& vPos, const size_t& index);
	
public:
	static CTerrain* Create(CToolView* pToolView);
public:
	vector<TILE_INFO*>	m_vecTile; // ���� ��ü Ÿ��
	vector<TILE_INFO*>	m_vecFieldTile; // ��ŷ�� �� ���� �� Ÿ��
	vector<TILE_INFO*> m_vecOptionTile; // ��ŷ�� �� ���� �� �ɼ� Ÿ��
	bool RectTileCheck;
	bool ShowLine;
	bool ShowOptionTile;
	bool PickTile_Check;
	

	float PickTileSizeX;
	float PickTileSizeY;

private:
	CToolView*			m_pToolView;
};


