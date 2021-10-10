#include "stdafx.h"
#include "IcePillar.h"
#include "Boss.h"

#include "NormalEffect.h"
#include "BuffEffect.h"
#include "StopEffect.h"
#include "AnimationImpl.h"
#include "NoAnimationImpl.h"
#include "AbstractFactory.h"
#include "IcePillarEffect.h"
#include "DamageEffect.h"
#include "DamageEffectImpl.h"
#include "LongAttackEffect.h"
#include "HitEffect.h"

#include "NormalSword.h"
#include "FireSword.h"
#include "RegenRing.h"
#include "MagicShield.h"
#include "Bomerang.h"

#include "IceBullet.h"
#include "EnemyHpBar.h"

CIcePillar::CIcePillar()
	:m_fDist(90.f)
	, lateInit(false)
	, EnterPillar(false)
	, PatternStart(false)
	, InitOneTime(false)
	, IdleSet(false)
	, Pattern1Set(false)
	, Pattern2Set(false)
	, Pattern3Set(false)
	, Pattern4Set(false)
{
	FirstAngle = 0.f;
	FirstPos = { 0.f,0.f,0.f };
	m_RotAngle = 90.f;

	CreateTimeBullet = 0.f;
	EndTimeBullet = 0.1f;
	m_FireTermPattern1 = 0.f;
	
	CreateBulletSound = 0.f;
	EndBulletSound = 0.05f;

	CreateTimePattern2 = 0.f;
	EndTimePattern2 = 0.1f;
	m_FireTermPattern2 = 0.f;

	CreateTimePattern3 = 0.f;
	EndTimePattern3 = 0.08f;
	m_FireTermPattern3 = 0.f;

	m_eCurState = IDLE;
	m_eNextState = IDLE;
	myPos = { 0.f, 0.f, 0.f };
	MyNumber = 0;
	HitCheck = false;
	Red = 255;
	Green = 255;
	Blue = 255;
	
	CreateTimeReset = 0.f;
	EndTimeReset = 0.1f;
}


CIcePillar::~CIcePillar()
{
}

int CIcePillar::Update()
{
	CObj::LateInitialize();
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	D3DXVECTOR3 vMouse = {};
	vMouse.x = (float)pt.x;
	vMouse.y = (float)pt.y;
	vMouse.z = 0.f;

	D3DXVECTOR3 pos = myPos;
	m_tUnit.vDir = vMouse - myPos;

	/*D3DXVec3Normalize(&m_tUnit.vDir, &m_tUnit.vDir);*/

	float Dist = D3DXVec3Length(&m_tUnit.vDir);

	
	if (m_tInfo.iHp <= 0.f)
	{
		m_bIsDead = true;
	}

	if (m_bIsDead == true)
	{
		D3DXMATRIX matWorld, matScroll;
		D3DXMatrixTranslation(&matScroll,
			CScrollMgr::GetScrollPos(0),
			CScrollMgr::GetScrollPos(1),
			0.f);
		matWorld = m_tUnit.matWorld*matScroll;

		dynamic_cast<CBoss*>(m_pBoss)->Delete_IcePillar(this);
		CAnimationImpl* pImpl = CAnimationImpl::Create(
			L"IcePillar", L"DestroyPillar", FRAME(0.f, 3.f), 3.f);

		CIcePillarEffect* pEffect = CIcePillarEffect::Create(pImpl, matWorld);
		//CNormalEffect* pEffect = CNormalEffect::Create(pImpl, m_tUnit.vPos + m_pBoss->Get_Info().vPos);
		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);
		
		return OBJ_DEAD;
	}

	

	if (m_eCurState == STATE::ENTERPILLAR)
	{
		if (m_tFrame.fCurFrame >= m_tFrame.fFrameCnt - 2.f)
		{
			m_eNextState = STATE::IDLE;
		}
	}

	UpdateRect();
	MakeWorldMatrix();
	return OBJ_NOEVENT;
}

void CIcePillar::LateUpdate()
{
	FrameChange();
	StateCheck();
	SettingPattern();
	CObj::FrameMove(0.5f);
}

void CIcePillar::Render()
{

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width*0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height*0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, Red, Green, Blue));
	CreateTimeReset += CTimeMgr::Get_Instance()->GetDelta();
	if (EndTimeReset <= CreateTimeReset)
	{
		CheckReset();
	}
}

void CIcePillar::RectColl(CObj * pObj)
{
	if (HitCheck == false)
	{// 쳐맞으면 무기로 쳐맞음
	 //if (pObj->Get_ItemData().eLength == ITEMINTER::MELEE)
	 //{
		D3DXMATRIX matWorld, matScroll,matTrans;
		D3DXMatrixTranslation(&matScroll,
			CScrollMgr::GetScrollPos(0),
			CScrollMgr::GetScrollPos(1),
			0.f);
		matWorld = m_tUnit.matWorld*matScroll;

		D3DXVECTOR3 BulletPos = {};
		D3DXVec3TransformCoord(&BulletPos, &D3DXVECTOR3(0.f, 0.f, 0.f), &matWorld);
		
		D3DXMatrixTranslation(&matTrans, 
			BulletPos.x , 
			BulletPos.y , 0.f);

		D3DXVECTOR3 HitSize = { 2.f,2.f,0.f };
		CAnimationImpl* pImpl = CAnimationImpl::Create(
			L"Monster", L"MonsterMeleeHit", FRAME(0.f, 4.f), 3.f);
		CLongAttackEffect* pEffect = CLongAttackEffect::Create(pImpl,
			BulletPos, D3DXVECTOR3(3.f, 3.f, 0.f));
		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);
		//		}
		int TotalDamage = (int)pObj->Get_ItemData().iAtt;
		CDamageEffectImpl* pDamageImpl = CDamageEffectImpl::Create(
			L"Font", L"DamageFont", TotalDamage);

		CDamageEffect* pDamageEffect = CDamageEffect::Create(pDamageImpl,
			BulletPos, D3DXVECTOR3(20.f, -10.f, 0.f));
		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pDamageEffect);
		m_tInfo.iHp -= TotalDamage; // 그 공격력만큼 데미지를 빼주고
		Red = 255;
		Green = 0;
		Blue = 0;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTERHIT);
		CSoundMgr::Get_Instance()->PlaySound(L"MyHitMonster.wav", CSoundMgr::MONSTERHIT);
		HitCheck = true;
	}
}

HRESULT CIcePillar::Initialize()
{
	m_tUnit.Texture.wstrObjectKey = L"IcePillar";
	m_tUnit.Texture.wstrStateKey = L"IcePillarIdle";
	m_tUnit.vLook = { 1.f,0.f,0.f };
	m_tUnit.vSize = { 1.f, 1.f,0.f };
	m_tUnit.vPos = { 0.f,0.f,0.f };
	m_tUnit.vLength = { 50.f,50.f,0.f };

	m_fAngle = 135.f; // 45도

	// 얼음기둥의 체력
	m_tInfo.iMaxHp = 500.f;
	m_tInfo.iHp = (int)m_tInfo.iMaxHp;



	return S_OK;
}

HRESULT CIcePillar::LateInitialize()
{
	m_eNextState = ENTERPILLAR;
	D3DXMatrixIdentity(&test);
	return S_OK;
}

void CIcePillar::Release()
{
}



void CIcePillar::MakeWorldMatrix()
{

	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);
	D3DXMatrixRotationZ(&m_tUnit.matRotZ, D3DXToRadian(-m_RotAngle)); // 자전
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x,
		m_tUnit.vPos.y, 0.f);
	D3DXMatrixRotationZ(&m_tUnit.matPlanetZ, D3DXToRadian(-m_fAngle)); // 공전

	if (!(m_pBoss == nullptr))
		m_tUnit.matParent = m_pBoss->Get_Info().matWorld; // 부모 행렬

	m_tUnit.matWorld = 
		m_tUnit.matScale  
		* m_tUnit.matRotZ 
		* m_tUnit.matTrans		
		* m_tUnit.matPlanetZ 
		* m_tUnit.matParent;
	//m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matWorld;

	test = m_tUnit.matPlanetZ*m_tUnit.matParent; // 
	D3DXVec3TransformCoord(&myPos, &m_tUnit.vPos, &test);

}

void CIcePillar::StateCheck()
{
	switch (m_eCurState)
	{
	case STATE::IDLE:
		Idle();
		break;
	case STATE::PATTERN1:
		Pattern1();
		break;
	case STATE::PATTERN2:
		Pattern2();
		break;
	case STATE::HARDPATTERN1:
		HardPattern1();
		break;
	case STATE::HARDPATTERN2:
		HardPattern2();
		break;
	case STATE::ENTERPILLAR:
		m_tUnit.Texture.wstrStateKey = L"IcePillarCreate";
		m_tFrame.fFrameCnt = 20.f;
		break;
	default:
		break;
	}
}

void CIcePillar::FrameChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case STATE::IDLE:
			m_tUnit.Texture.wstrStateKey = L"IcePillarIdle";
			m_tFrame.fFrameCnt = 1.f;
			break;
		case STATE::ATT:

			break;
		case STATE::PATTERN1:
			break;
		case STATE::PATTERN2:
			break;
		case STATE::ENTERPILLAR:
			m_tUnit.Texture.wstrStateKey = L"IcePillarCreate";
			m_tFrame.fFrameCnt = 20.f;
			break;
		case STATE::END:
			break;
		}
		m_eCurState = m_eNextState;
	}
}

void CIcePillar::SettingPattern()
{
	// 셋팅해야할 STATE가 트루일때 그 행동을 하기위한 것들을 실행
	if (IdleSet == true)
	{
		IdleSetting();
	}
	if (Pattern1Set == true)
	{
		Pattern1Setting();
	}
	if (Pattern2Set == true)
	{
		Pattern2Setting();
	}
	if (Pattern3Set == true)
	{
		Pattern3Setting();
	}
	if (Pattern4Set == true)
	{
		Pattern4Setting();
	}
}

void CIcePillar::Idle()
{
	if (InitOneTime == false)
	{// 처음위치를 한번만 초기화 해주자
		FirstAngle = m_fAngle;
		FirstPos = { m_tUnit.vPos.x,m_tUnit.vPos.y,0.f };
		InitOneTime = true;
	}
	if (PatternStart == false)
	{// 패턴을 해라 명령이 떨어졌을때 먼저 할것
		
		IdleSet = true; 
		Pattern1Set = false;
		Pattern2Set = false;
		Pattern3Set = false;
		Pattern4Set = false;
		//m_fAngle = FirstAngle; // 45 135 225 315
		//m_tUnit.vPos = FirstPos;

		PatternStart = true;
	}
	// 공전각
	m_fAngle += 100.f*CTimeMgr::Get_Instance()->GetDelta();
	 
	
}
void CIcePillar::IdleSetting()
{// Idle상태일때 셋팅할 것
	D3DXVECTOR3 vDir = {
		(FirstPos.x - m_tUnit.vPos.x),
		(FirstPos.y - m_tUnit.vPos.y),
		0.f };
	float GoalLength = D3DXVec3Length(&vDir);// 그 지점까지의 거리 재고
	D3DXVec3Normalize(&vDir, &vDir);// 정규화 해서 방향 구하고
	m_tUnit.vPos += vDir * 400.f *(float)m_pTimeMgr->GetDelta();
	if (GoalLength <= 10.f)
	{
		m_tUnit.vPos = FirstPos;
		m_RotAngle = 90.f;
		IdleSet = false;
	}

}

//////////////////////////////// Pattern1

void CIcePillar::Pattern1()
{
	if (PatternStart == false)
	{// 패턴을 해라 명령이 떨어졌을때 먼저 할것
		
		Pattern1Set = true; // Pattern1의 셋팅을 시작해라
		
		IdleSet = false;
		Pattern2Set = false;
		Pattern3Set = false;
		Pattern4Set = false;

		PatternStart = true;
	}
	if (Pattern1Set == false)
	{ // 세팅이 다완료되었을때 패턴을 실행하라

		m_RotAngle += 500.f*CTimeMgr::Get_Instance()->GetDelta();
		CreateTimeBullet += m_pTimeMgr->GetDelta();
		m_FireTermPattern1 += m_pTimeMgr->GetDelta();
		if (EndTimeBullet <= CreateTimeBullet)
		{

			D3DXMATRIX matWorld, matScroll;
			D3DXMatrixTranslation(&matScroll,
				CScrollMgr::GetScrollPos(0),
				CScrollMgr::GetScrollPos(1),
				0.f);
			matWorld = m_tUnit.matWorld*matScroll;
			
			D3DXVECTOR3 Pattern1vDir = { 1.f,0.f,0.f };
			CIceBullet* pBullet = nullptr;
			D3DXVECTOR3 BulletPos = {};
			D3DXVec3TransformCoord(&BulletPos, &D3DXVECTOR3(0.f, 0.f, 0.f), &matWorld);
			pBullet = CIceBullet::Create(matWorld,
				PlayerDir1,BulletPos,700.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::ICEBULLET, pBullet);

			CreateBulletSound += m_pTimeMgr->GetDelta();
			if (EndBulletSound <= CreateBulletSound)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ICEBULLETCREATE);
				CSoundMgr::Get_Instance()->PlaySound(L"MySpawnIceBullet.wav", CSoundMgr::ICEBULLETCREATE);
				CreateBulletSound = 0.f;
			}
			CreateTimeBullet = 0.f;
		}

		if (8.f <= m_FireTermPattern1)
		{
			//m_eNextState = IDLE;
			m_pBoss->BulletState = IDLE;
			PatternStart = false;
			m_FireTermPattern1 = 0.f;
		}
	}
}

void CIcePillar::Pattern1Setting()
{
	
	D3DXVECTOR3 Goal = { // 가야 하는 지점
		150.f,0.f,0.f};
	D3DXVECTOR3 vDir = {
		(Goal.x - m_tUnit.vPos.x),
		(Goal.y - m_tUnit.vPos.y),
		0.f };
	m_fAngle = FirstAngle;
	

	float GoalLength = D3DXVec3Length(&vDir);// 그 지점까지의 거리 재고
	D3DXVec3Normalize(&vDir, &vDir);// 정규화 해서 방향 구하고
	m_tUnit.vPos += vDir * 400.f *(float)m_pTimeMgr->GetDelta();
	if (GoalLength <= 10.f)
	{
		m_tUnit.vPos = Goal;
		TargetInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
		
		D3DXMATRIX matWorld, matScroll;
		D3DXMatrixTranslation(&matScroll,
			CScrollMgr::GetScrollPos(0),
			CScrollMgr::GetScrollPos(1),
			0.f);
		matWorld = m_tUnit.matWorld*matScroll;

		CIceBullet* pBullet = nullptr;
		D3DXVECTOR3 BulletPos = {};
		D3DXVec3TransformCoord(&BulletPos, &D3DXVECTOR3(0.f, 0.f, 0.f), &matWorld);
		TargetPos = TargetInfo.vPos;
		PlayerDir1 = TargetPos - BulletPos;
		D3DXVec3Normalize(&PlayerDir1, &PlayerDir1);
		Pattern1Set = false;
	}
}

//////////////////////////////// Pattern2

void CIcePillar::Pattern2()
{ // 빙글빙글 탄막
	if (PatternStart == false)
	{// 패턴을 해라 명령이 떨어졌을때 먼저 할것

		Pattern2Set = true; // Pattern1의 셋팅을 시작해라

		IdleSet = false;
		Pattern1Set = false;
		Pattern3Set = false;
		Pattern4Set = false;

		PatternStart = true;
	}
	
	// 공전각
	
	if (Pattern2Set == false)
	{ // 세팅이 완료된 후에 동작을 해라

		m_fAngle += 500.f*CTimeMgr::Get_Instance()->GetDelta();

		D3DXMATRIX matWorld, matScroll;
		D3DXMatrixTranslation(&matScroll,
			CScrollMgr::GetScrollPos(0),
			CScrollMgr::GetScrollPos(1),
			0.f);
		matWorld = m_tUnit.matWorld*matScroll;
		TargetInfo = m_pBoss->Get_Info();
		CIceBullet* pBullet = nullptr;
		D3DXVECTOR3 BulletPos = {};
		D3DXVec3TransformCoord(&BulletPos, &D3DXVECTOR3(0.f, 0.f, 0.f), &matWorld);
		TargetPos = TargetInfo.vPos;
		PlayerDir1 = -(TargetPos - BulletPos);
		D3DXVec3Normalize(&PlayerDir1, &PlayerDir1);

		
		m_FireTermPattern2 += m_pTimeMgr->GetDelta();
		CreateTimePattern2 += m_pTimeMgr->GetDelta();
		if (EndTimePattern2 <= CreateTimePattern2)
		{

			pBullet = CIceBullet::Create(matWorld,
				PlayerDir1, BulletPos,500.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::ICEBULLET, pBullet);

			CreateBulletSound += m_pTimeMgr->GetDelta();
			if (EndBulletSound <= CreateBulletSound)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ICEBULLETCREATE);
				CSoundMgr::Get_Instance()->PlaySound(L"MySpawnIceBullet.wav", CSoundMgr::ICEBULLETCREATE);
				CreateBulletSound = 0.f;
			}
			CreateTimePattern2 = 0.f;
		}

		if (8.f <= m_FireTermPattern2)
		{
			m_pBoss->BulletState = IDLE;
			PatternStart = false;
			m_FireTermPattern2 = 0.f;
		}

	}
}

void CIcePillar::Pattern2Setting()
{
	D3DXVECTOR3 Goal = { // 가야 하는 지점
		20.f,0.f,0.f };
	D3DXVECTOR3 vDir = {
		(Goal.x - m_tUnit.vPos.x),
		(Goal.y - m_tUnit.vPos.y),
		0.f };
	m_fAngle = FirstAngle;


	float GoalLength = D3DXVec3Length(&vDir);// 그 지점까지의 거리 재고
	D3DXVec3Normalize(&vDir, &vDir);// 정규화 해서 방향 구하고
	m_tUnit.vPos += vDir * 400.f *(float)m_pTimeMgr->GetDelta();
	if (GoalLength <= 10.f)
	{
		m_tUnit.vPos = Goal;
		m_RotAngle = 90.f;

		Pattern2Set = false;
	}

}

//////////////////////////////// Pattern3

void CIcePillar::HardPattern1()
{
	if (PatternStart == false)
	{// 패턴을 해라 명령이 떨어졌을때 먼저 할것

		Pattern3Set = true; // Pattern1의 셋팅을 시작해라

		IdleSet = false;
		Pattern1Set = false;
		Pattern2Set = false;
		Pattern4Set = false;

		PatternStart = true;
	}

	if (Pattern3Set == false)
	{// 세팅이 완료 되었으면
		m_RotAngle += 500.f*CTimeMgr::Get_Instance()->GetDelta();
		D3DXMATRIX matWorld, matScroll;
		D3DXMatrixTranslation(&matScroll,
			CScrollMgr::GetScrollPos(0),
			CScrollMgr::GetScrollPos(1),
			0.f);
		matWorld = m_tUnit.matWorld*matScroll;
		TargetInfo = m_pBoss->Get_Info();
		CIceBullet* pBullet = nullptr;
		D3DXVECTOR3 BulletPos = {};
		D3DXVec3TransformCoord(&BulletPos, &D3DXVECTOR3(0.f, 0.f, 0.f), &matWorld);
		D3DXMATRIX matRotZ;
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_RotAngle));
		D3DXVec3TransformCoord(&PlayerDir1, &PlayerDir1, &matRotZ);

		m_FireTermPattern3 += m_pTimeMgr->GetDelta();
		CreateTimePattern3 += m_pTimeMgr->GetDelta();
		if (EndTimePattern3 <= CreateTimePattern3)
		{

			pBullet = CIceBullet::Create(matWorld,
				PlayerDir1, BulletPos,300.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::ICEBULLET, pBullet);

			CreateBulletSound += m_pTimeMgr->GetDelta();
			if (EndBulletSound <= CreateBulletSound)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ICEBULLETCREATE);
				CSoundMgr::Get_Instance()->PlaySound(L"MySpawnIceBullet.wav", CSoundMgr::ICEBULLETCREATE);
				CreateBulletSound = 0.f;
			}
			CreateTimePattern3 = 0.f;
		}

		if (8.f <= m_FireTermPattern3)
		{
			m_pBoss->BulletState = IDLE;
			PatternStart = false;
			m_FireTermPattern3 = 0.f;
		}
	}

}

void CIcePillar::Pattern3Setting()
{
	D3DXVECTOR3 Goal = { // 가야 하는 지점
		100.f,0.f,0.f };
	D3DXVECTOR3 vDir = {
		(Goal.x - m_tUnit.vPos.x),
		(Goal.y - m_tUnit.vPos.y),
		0.f };
	m_fAngle = FirstAngle;


	float GoalLength = D3DXVec3Length(&vDir);// 그 지점까지의 거리 재고
	D3DXVec3Normalize(&vDir, &vDir);// 정규화 해서 방향 구하고
	m_tUnit.vPos += vDir * 300.f *(float)m_pTimeMgr->GetDelta();
	if (GoalLength <= 10.f)
	{
		m_tUnit.vPos = Goal;

		D3DXMATRIX matWorld, matScroll;
		D3DXMatrixTranslation(&matScroll,
			CScrollMgr::GetScrollPos(0),
			CScrollMgr::GetScrollPos(1),
			0.f);
		matWorld = m_tUnit.matWorld*matScroll;
		TargetInfo = m_pBoss->Get_Info();
		CIceBullet* pBullet = nullptr;
		D3DXVECTOR3 BulletPos = {};
		D3DXVec3TransformCoord(&BulletPos, &D3DXVECTOR3(0.f, 0.f, 0.f), &matWorld);
		TargetPos = TargetInfo.vPos;
		PlayerDir1 = -(TargetPos - BulletPos);
		D3DXVec3Normalize(&PlayerDir1, &PlayerDir1);
		Pattern3Set = false;
	}
}

//////////////////////////////// Pattern4

void CIcePillar::HardPattern2()
{
	if (PatternStart == false)
	{// 패턴을 해라 명령이 떨어졌을때 먼저 할것

		Pattern4Set = true; // Pattern1의 셋팅을 시작해라

		IdleSet = false;
		Pattern1Set = false;
		Pattern2Set = false;
		Pattern3Set = false;

		PatternStart = true;
	}

}

void CIcePillar::Pattern4Setting()
{
}

void CIcePillar::AttStart()
{
	m_tUnit.vPos.x = 100.f;
	m_RotAngle += 500.f*CTimeMgr::Get_Instance()->GetDelta();
	CreateTimeBullet += m_pTimeMgr->GetDelta();
	if (EndTimeBullet <= CreateTimeBullet)
	{
		CAnimationImpl* pImpl = CAnimationImpl::Create(
			L"IceBullet", L"CreateIceBullet", FRAME(0.f, 3.f), 1.f);

		CStopEffect* pEffect = CStopEffect::Create(pImpl, &m_tUnit.matWorld);
		//CNormalEffect* pEffect = CNormalEffect::Create(pImpl, m_tUnit.vPos + m_pBoss->Get_Info().vPos);
		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);
		CreateTimeBullet = 0.f;
	}
	//	}
}

CIcePillar * CIcePillar::Create()
{
	CIcePillar* m_pInstance = new CIcePillar;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}

void CIcePillar::CheckReset()
{
	HitCheck = false; // 다시 쳐맞기
	Red = 255;
	Green = 255;
	Blue = 255;
	CreateTimeReset = 0.f;
}

void CIcePillar::UpdateRect()
{
	D3DXMATRIX matWorld, matScroll;
	D3DXMatrixTranslation(&matScroll,
		CScrollMgr::GetScrollPos(0),
		CScrollMgr::GetScrollPos(1),
		0.f);
	matWorld = m_tUnit.matWorld*matScroll;
	
	D3DXVECTOR3 BulletPos = {};
	D3DXVec3TransformCoord(&BulletPos, &D3DXVECTOR3(0.f, 0.f, 0.f), &matWorld);

	m_tRect.left = LONG(BulletPos.x - (m_tUnit.vLength.x * 0.5f));
	m_tRect.top = LONG(BulletPos.y - (m_tUnit.vLength.y * 0.5f));
	m_tRect.right = LONG(BulletPos.x + (m_tUnit.vLength.x * 0.5f));
	m_tRect.bottom = LONG(BulletPos.y + (m_tUnit.vLength.y * 0.5f));
}
