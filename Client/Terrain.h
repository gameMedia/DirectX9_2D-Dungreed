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
	vector<TILE_INFO*>	m_vecTile; // ���� ��ü Ÿ��
	vector<TILE_INFO*>	m_vecFieldTile; // ��ŷ�� �� ���� �� Ÿ��
	vector<TILE_INFO*> m_vecOptionTile; // ��ŷ�� �� ���� �� �ɼ� Ÿ��
	bool RectTileCheck;
	bool ShowLine;
	bool ShowOptionTile;
	bool PickTile_Check;
	

	float PickTileSizeX;
	float PickTileSizeY;



	// CObj��(��) ���� ��ӵ�
};


