#pragma once
class CDeviceMgr
{
	DEFINE_SINGLETONE(CDeviceMgr)

public:
	enum WIN_MODE { MODE_FULL, MODE_WIN };

private:
	CDeviceMgr();
	~CDeviceMgr();

public:
	LPDIRECT3DDEVICE9 GetDevice();
	LPD3DXSPRITE GetSprite();
	LPD3DXFONT GetFont();
	LPD3DXLINE GetLine();
public:
	HRESULT InitDevice(
		HWND hWnd,
		const DWORD& dwWinCX,
		const DWORD& dwWinCY,
		WIN_MODE eMode);
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);

private:
	void Release();

private:
	// IDirect3D9: ��ġ ������ ��ġ�� ������ ��ü�� ������ ���� Com��ü.
	LPDIRECT3D9				m_pSDK;

	// IDirect3DDevice9: �׷��� ��ġ�� �����ϴ� Com��ü.
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	// ID3DXSprite: ���̷�Ʈ���� 2D �̹��� �������� ���ִ� Com��ü.
	LPD3DXSPRITE			m_pSprite;

	// ID3DXFont: ���̷�Ʈ���� 2D ��Ʈ �������� ���ִ� Com��ü.
	LPD3DXFONT				m_pFont;

	// ���� ������ Com��ü
	LPD3DXLINE				m_pLine;

	// COM (Component Object Model)
	// ���α׷��ֵ� ��ǰ �����ϵ��� �ٷﺸ��. ����� �����ϴ� ���α׷��� ��.
};

