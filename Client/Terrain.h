#pragma once

#include "Obj.h"

class CTerrain : public CObj
{
private:
	explicit CTerrain();

public:
	virtual ~CTerrain();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render();

	HRESULT LoadTile(const TCHAR* pFilePath);
public:
	vector<TILE_INFO*>* Get_OptionTile() { return &m_vecOptionTile; }

private:
	virtual HRESULT Initialize();
	
	///// Release
	void Release();
	void TileRelease();
	void FieldTileRelease();
	void OptionTileRelease();
	/////

public:
	static CTerrain* Create();

public:
	vector<TILE_INFO*>	m_vecTile; // 기존 전체 타일
	vector<TILE_INFO*>	m_vecFieldTile; // 피킹시 그 위에 깔릴 타일
	vector<TILE_INFO*> m_vecOptionTile; // 피킹시 그 위에 깔릴 옵션 타일
	bool RectTileCheck;
	bool ShowLine;
	bool ShowOptionTile;
	bool PickTile_Check;
	

	float PickTileSizeX;
	float PickTileSizeY;



	// CObj을(를) 통해 상속됨
};


