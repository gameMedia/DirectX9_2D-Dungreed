#include "stdafx.h"
#include "Monster.h"
#include "ActMgr.h"

#include "KeyMgr.h"
CMonster::CMonster()
	:m_pSubject(CMonsterSubject::Get_Instance()),
	m_iHp(0)
{
}


CMonster::~CMonster()
{
}

HRESULT CMonster::Initialize()
{

	m_tUnit.vLook = { 1.f,0.f,0.f };
	m_tUnit.vSize = { 2.f,2.f,0.f };


	m_iHp = 100;

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 6.f;

	return S_OK;
}

HRESULT CMonster::LateInitialize()
{
	m_pSubject->AddData(CMonsterSubject::MONSTER_HP, &m_iHp);
	m_pSubject->AddData(m_pSubject->MONSTER_POS, &m_tUnit.vPos);
	return S_OK;
}

int CMonster::Update()
{
	CObj::LateInitialize();
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	D3DXVECTOR3 vMouse = {};
	vMouse.x = (float)pt.x;
	vMouse.y = (float)pt.y;


	m_tUnit.vDir = vMouse - m_tUnit.vPos;
	D3DXVec3TransformNormal(&m_tUnit.vDir, &m_tUnit.vDir, &m_tUnit.matWorld);
	D3DXVec3Normalize(&m_tUnit.vDir, &m_tUnit.vDir);
	
	if (CKeyMgr::Get_Instance()->KeyPressing(KEY_UP))
	{
	
		
		m_tUnit.vPos += m_tUnit.vDir*5.f;
		m_pSubject->Notify(CMonsterSubject::MONSTER_HP, &m_iHp);
		
	}
	// 위치 좌표는 계속 갱신
	MakeWorldMatrix();
	//D3DXVec3TransformCoord(&m_tUnit.v, &m_tUnit.vDir, &m_tUnit.matWorld);
	m_pSubject->Notify(CMonsterSubject::MONSTER_POS, &m_tUnit.vPos);

	return OBJ_NOEVENT;
}

void CMonster::LateUpdate()
{
	CObj::FrameMove();
}

void CMonster::Render()
{
	CObj::UpdateRect();
	
	
	const TEX_INFO* pTexInfo = nullptr;
	TCHAR szIndex[MIN_STR] = L"";

	pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
		L"Monster", L"Guardian", (int)m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
}

void CMonster::Release()
{
}

void CMonster::ActStart(int _State)
{
}

CMonster * CMonster::Create()
{
	CMonster* m_pInstance = new CMonster;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
