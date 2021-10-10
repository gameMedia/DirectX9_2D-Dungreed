#pragma once

class CToolView;
class CMyMap
{
private:
	explicit CMyMap();

public:
	~CMyMap();

public:
	void SetToolView(CToolView* pToolView);

public:

	void Render();
	void MiniRender();
	void MapChange(const BYTE& byDrawID,
		const int & MapSizeX,
		const int & MapSizeY);
	HRESULT SaveMap(const TCHAR* pFilePath);
	HRESULT LoadMap(const TCHAR* pFilePath);

private:
	HRESULT Initialize();
	void Release();

public:
	static CMyMap* Create(CToolView* pToolView);

public:
	bool ShowMap;
	MAP_INFO* m_MyMap;
	int m_MapSizeX;
	int m_MapSizeY;
private:
	CToolView* m_pToolView;
	
};

