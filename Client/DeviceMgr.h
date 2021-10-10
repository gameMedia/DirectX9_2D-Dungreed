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
	// IDirect3D9: 장치 검증과 장치를 제어할 객체의 생성을 돕는 Com객체.
	LPDIRECT3D9				m_pSDK;

	// IDirect3DDevice9: 그래픽 장치를 제어하는 Com객체.
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	// ID3DXSprite: 다이렉트에서 2D 이미지 렌더링을 해주는 Com객체.
	LPD3DXSPRITE			m_pSprite;

	// ID3DXFont: 다이렉트에서 2D 폰트 렌더링을 해주는 Com객체.
	LPD3DXFONT				m_pFont;

	// 라인 랜더링 Com객체
	LPD3DXLINE				m_pLine;

	// COM (Component Object Model)
	// 프로그래밍도 부품 조립하듯이 다뤄보자. 마쏘에서 제공하는 프로그래밍 모델.
};

