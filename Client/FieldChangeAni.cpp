#include "stdafx.h"
#include "FieldChangeAni.h"


CFieldChangeAni::CFieldChangeAni(
	const wstring& wstrObjectKey,
	const wstring& wstrStateKey,
	const FRAME& tFrame,
	float _playSpeed)
	:CEffectImpl(wstrObjectKey),
	m_wstrStateKey(wstrStateKey),
	m_tFrame(tFrame)
{
	playSpeed = _playSpeed;
}


CFieldChangeAni::~CFieldChangeAni()
{
}

int CFieldChangeAni::Update()
{
	m_tFrame.fCurFrame += m_tFrame.fFrameCnt*CTimeMgr::Get_Instance()->GetDelta()*playSpeed;

	if (m_tFrame.fCurFrame >= m_tFrame.fFrameCnt)
	{
		CSceneMgr::Get_Instance()->SceneChange(CSceneMgr::Get_Instance()->DUNGEON);
		return OBJ_DEAD;
	}
	return OBJ_NOEVENT;
}

void CFieldChangeAni::Render(const D3DXMATRIX * pWorldMatrix)
{
	const TEX_INFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
		m_wstrObjectKey, m_wstrStateKey, (int)m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(pWorldMatrix);
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture,
		nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

CFieldChangeAni * CFieldChangeAni::Create(const wstring & wstrObjectKey, const wstring & wstrStateKey, const FRAME & tFrame, float _playSpeed)
{
	return new CFieldChangeAni(wstrObjectKey, wstrStateKey, tFrame, _playSpeed);
}
