#include "stdafx.h"
#include "Bat.h"
#include "AbstractFactory.h"
#include "EnemyHpBar.h"
#include "BatBullet.h"

#include "HitEffect.h"
#include "AnimationImpl.h"

#include "DamageEffect.h"
#include "DamageEffectImpl.h"

#include "LongAttackEffect.h"
#include "MyMoney.h"
#include "BatItem.h"
CBat::CBat()
	:HitCheck(false)
{
	Red = 255;
	Green = 255;
	Blue = 255;

	CreateTimeAttack = 0.f;
	EndTimeAttack = 5.f;
	m_FireTerm = 0.f;
	m_FireAngle = 0.f;
	CreateTimeReset = 0.f;
	EndTimeReset = 0.1f;
}


CBat::~CBat()
{
}

int CBat::Update()
{
	CObj::LateInitialize();

	CObj::UpdateRect();

	if (m_tInfo.iHp <= 0)
	{
		D3DXVECTOR3 HitSize = { 4.f,4.f,0.f };
		CAnimationImpl* pImpl = CAnimationImpl::Create(
			L"Monster", L"MonsterDead", FRAME(0.f, 12.f), 2.f);
		CLongAttackEffect* pEffect = CLongAttackEffect::Create(
			pImpl, m_tUnit.vPos, HitSize);
		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);

		CObjMgr::Get_Instance()->Add_Object(OBJID::MONEY, CAbstractFactory<CMyMoney>::Create(m_tUnit.vPos.x, m_tUnit.vPos.y));
		CObjMgr::Get_Instance()->Add_Object(OBJID::BATITEM, CAbstractFactory<CBatItem>::Create(m_tUnit.vPos.x, m_tUnit.vPos.y));

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTERDEAD);
		CSoundMgr::Get_Instance()->PlaySound(L"MyMonsterDie.wav", CSoundMgr::MONSTERDEAD);
		return OBJ_DEAD;
	}


	CObj::MakeWorldMatrix();
	ActionStart();
	StateCheck();


	return OBJ_NOEVENT;
}

void CBat::LateUpdate()
{
	FrameChange();
	CObj::FrameMove(FrameSpeed);
	//MakeLine();
}

void CBat::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);

	/*CDeviceMgr::Get_Instance()->GetSprite()->End();



	CDeviceMgr::Get_Instance()->GetLine()->SetWidth(5.f);
	CDeviceMgr::Get_Instance()->GetLine()->Draw(m_vOrigin, 5, D3DCOLOR_ARGB(255, 0, 255, 0));

	CDeviceMgr::Get_Instance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);*/

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
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

HRESULT CBat::Initialize()
{
	m_eCurState = IDLE;
	m_eNextState = IDLE;


	m_fSpeed = SkullDog_Speed;
	m_fAngle = 0.f;

	m_tUnit.vPos = { 900.f,500.f,0.f };
	m_tUnit.vLength = { 150.f,150.f,0.f };
	m_tUnit.vCollPos = { m_tUnit.vPos.x,m_tUnit.vPos.y + m_tUnit.vLength.y*0.5f,0.f };
	m_tUnit.vLook = { 1.f,0.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"Bat";
	m_tUnit.Texture.wstrStateKey = L"BatIdle";

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 7.f;
	FrameSpeed = 1.f;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);


	m_tUnit.vSize.x = m_tUnit.vLength.x / pTexInfo->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / pTexInfo->tImgInfo.Height;

	Current_tick = clock();
	Next_tick = (clock_t)0.f;



	CUI* pUi = CAbstractFactory<CEnemyHpBar>::Create();
	CUIMgr::Get_Instance()->Add_Object(UIID::ENEMYLIFEBAR, pUi);
	pUi->Set_Obj(this);

	// 박쥐의 정보
	m_tInfo.iAtt = 1;
	m_tInfo.iGold = 10000;
	m_tInfo.iMaxHp = 500.f;
	m_tInfo.iHp = (int)m_tInfo.iMaxHp;



	return S_OK;
}

HRESULT CBat::LateInitialize()
{
	return S_OK;
}

void CBat::Release()
{
}

void CBat::MakeLine()
{
	m_vOrigin[0] = {
		(-m_tUnit.vLength.x*0.5f) + m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		(-m_tUnit.vLength.y*0.5f) + m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[1] = {
		(m_tUnit.vLength.x*0.5f) + m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		(-m_tUnit.vLength.y*0.5f) + m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[2] = {
		(m_tUnit.vLength.x*0.5f) + m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		(m_tUnit.vLength.y*0.5f) + m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[3] = {
		(-m_tUnit.vLength.x*0.5f) + m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		(m_tUnit.vLength.y*0.5f) + m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[4] = {
		(-m_tUnit.vLength.x*0.5f) + m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		(-m_tUnit.vLength.y*0.5f) + m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
}

void CBat::RectColl(CObj * pObj)
{
	if (HitCheck == false)
	{// 쳐맞으면 무기로 쳐맞음
		//if (pObj->Get_ItemData().eLength == ITEMINTER::MELEE)
		//{
			D3DXVECTOR3 HitSize = { 2.f,2.f,0.f };
			CAnimationImpl* pImpl = CAnimationImpl::Create(
				L"Monster", L"MonsterMeleeHit", FRAME(0.f, 4.f), 3.f);
			CHitEffect* pEffect = CHitEffect::Create(pImpl, &m_tUnit.matTrans);
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);
//		}
		int TotalDamage = (int)pObj->Get_ItemData().iAtt;
		CDamageEffectImpl* pDamageImpl = CDamageEffectImpl::Create(
			L"Font", L"DamageFont", TotalDamage);

		CDamageEffect* pDamageEffect = CDamageEffect::Create(pDamageImpl, m_tUnit.vPos, D3DXVECTOR3(20.f, -10.f, 0.f));
		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pDamageEffect);
		m_tInfo.iHp -= TotalDamage; // 그 공격력만큼 데미지를 빼주고
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTERHIT);
		CSoundMgr::Get_Instance()->PlaySound(L"MyHitMonster.wav", CSoundMgr::MONSTERHIT);
		Red = 255;
		Green = 0;
		Blue = 0;
		HitCheck = true;
	}
}



void CBat::StateCheck()
{
	switch (m_eCurState)
	{
	case STATE::IDLE:
		Idle();
		break;
	case STATE::MOVE:
		Move();
		break;
	case STATE::ATTACK:
		Attack();
		break;
	case STATE::HIT:
		break;
	default:
		break;
	}
}

void CBat::FrameChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case STATE::MOVE:
			m_tUnit.Texture.wstrStateKey = L"BatIdle";
			m_tFrame.fFrameCnt = 7.f;
			FrameSpeed = 1.4f;
			break;
		case STATE::HIT:
			break;
		case STATE::ATTACK:
			m_tUnit.Texture.wstrStateKey = L"BatAttack";
			m_tFrame.fFrameCnt = 10.f;
			FrameSpeed = 1.f;
			break;
		case STATE::END:
			break;
		default:
			break;
		}
		m_eCurState = m_eNextState;
	}
}

void CBat::ActionStart()
{
	UNIT_INFO TargetInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	D3DXVECTOR3 TargetPos = TargetInfo.vPos;
	PlayerDir = TargetPos - m_tUnit.vPos;
	PlayerLength = D3DXVec3Length(&PlayerDir); // 거리 구하고


	D3DXVec3Normalize(&PlayerDir, &PlayerDir);
	D3DXVec3Normalize(&m_tUnit.vLook, &m_tUnit.vLook);
	float cos = D3DXVec3Dot(&PlayerDir, &m_tUnit.vLook);
	CreateTimeTarget += CTimeMgr::Get_Instance()->GetDelta();
	if (EndTimeTarget <= CreateTimeTarget)
	{
		if (cos <= 0.f&&m_tUnit.vSize.x > 0)
		{// 만약 둔각이라면? 
		 // 플레이어는 왼쪽에 있다가 된다.
			m_tUnit.vSize.x *= -1.f;
		}
		else if (cos > 0.f&&m_tUnit.vSize.x < 0)
		{
			m_tUnit.vSize.x *= -1.f;
		}
		CreateTimeTarget = 0.f;
	}
}

void CBat::Idle()
{
	UNIT_INFO TargetInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	D3DXVECTOR3 TargetPos = TargetInfo.vPos;
	PlayerDir = TargetPos - m_tUnit.vPos;
	PlayerLength = D3DXVec3Length(&PlayerDir); // 거리 구하고



	if (PlayerLength < 300.f)
	{ // 초기 Idle이지만 플레이어가 이 위치에 오면 움직여라
		m_eNextState = MOVE;
	}
	CreateTimeAttack += CTimeMgr::Get_Instance()->GetDelta();
	if (EndTimeAttack <= CreateTimeAttack)
	{
		m_eNextState = ATTACK;
		CreateTimeAttack = 0.f;
	}
}

void CBat::Move()
{
	UNIT_INFO TargetInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	D3DXVECTOR3 TargetPos = TargetInfo.vPos;
	PlayerDir = TargetPos - m_tUnit.vPos;
	D3DXVec3Normalize(&PlayerDir, &PlayerDir);

	m_tUnit.vPos += PlayerDir*50.f*m_pTimeMgr->GetDelta();
	CreateTimeAttack += CTimeMgr::Get_Instance()->GetDelta();
	if (EndTimeAttack <= CreateTimeAttack)
	{
		m_eNextState = ATTACK;
		CreateTimeAttack = 0.f;
	}
}

void CBat::Attack()
{
	m_FireTerm += m_pTimeMgr->GetDelta();
	UNIT_INFO TargetInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	D3DXVECTOR3 TargetPos = TargetInfo.vPos;
	D3DXVECTOR3 PlayerDir1 = TargetPos - m_tUnit.vPos;
	D3DXVec3Normalize(&PlayerDir1, &PlayerDir1);
	if (1.f <= m_FireTerm)
	{

		D3DXMATRIX matRotZ, matTrans, matWorld;
		D3DXVECTOR3 vDir = { 1.f, 0.f, 0.f };
		CBatBullet* pBullet = nullptr;

		for (m_FireAngle = 0.f; m_FireAngle < 360.f;)
		{
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_FireAngle));
			/*D3DXMatrixTranslation(&matTrans, m_tUnit.vPos.x, m_tUnit.vPos.y, 0.f);

			matWorld =  matTrans;

			
			D3DXVec3TransformNormal(&PlayerDir1, &PlayerDir1, &matWorld);
*/
			pBullet = CBatBullet::Create(m_tUnit.vPos, PlayerDir1, m_FireAngle);
			CObjMgr::Get_Instance()->Add_Object(OBJID::BATBULLET, pBullet);
			m_FireAngle += 20.f;
		}

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BATATTACK);
		CSoundMgr::Get_Instance()->PlaySound(L"MyBatAtt.wav", CSoundMgr::BATATTACK);

		m_FireTerm = 0.f;
		m_eNextState = IDLE;
	}
}

void CBat::CheckReset()
{
	HitCheck = false; // 다시 쳐맞기
	Red = 255;
	Green = 255;
	Blue = 255;
	CreateTimeReset = 0.f;
}

CBat * CBat::Create()
{
	CBat* m_pInstance = new CBat;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
