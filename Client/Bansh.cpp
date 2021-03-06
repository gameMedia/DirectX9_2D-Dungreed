#include "stdafx.h"
#include "Bansh.h"
#include "AbstractFactory.h"
#include "EnemyHpBar.h"
#include "BanshBullet.h"

#include "HitEffect.h"
#include "AnimationImpl.h"
#include "LongAttackEffect.h"

#include "DamageEffect.h"
#include "DamageEffectImpl.h"
#include "MyMoney.h"
#include "NormalSword.h"

CBansh::CBansh()
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


CBansh::~CBansh()
{
}

int CBansh::Update()
{
	CObj::LateInitialize();
	CObj::UpdateRect();

	if (m_tInfo.iHp <= 0)
	{
		D3DXVECTOR3 HitSize = { 2.f,2.f,0.f };
		CAnimationImpl* pImpl = CAnimationImpl::Create(
			L"Monster", L"MonsterDead", FRAME(0.f, 12.f), 2.f);
		CLongAttackEffect* pEffect = CLongAttackEffect::Create(
			pImpl, m_tUnit.vPos,HitSize);
		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);
		CObjMgr::Get_Instance()->Add_Object(OBJID::NORMALSWORD, CAbstractFactory<CNormalSword>::Create(m_tUnit.vPos.x, m_tUnit.vPos.y));
		CObjMgr::Get_Instance()->Add_Object(OBJID::MONEY, CAbstractFactory<CMyMoney>::Create(m_tUnit.vPos.x, m_tUnit.vPos.y));
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTERDEAD);
		CSoundMgr::Get_Instance()->PlaySound(L"MyMonsterDie.wav", CSoundMgr::MONSTERDEAD);
		return OBJ_DEAD;
	}


	CObj::MakeWorldMatrix();
	ActionStart();
	StateCheck();


	return OBJ_NOEVENT;
}

void CBansh::LateUpdate()
{
	FrameChange();
	CObj::FrameMove(FrameSpeed);
	//MakeLine();
}

void CBansh::Render()
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

HRESULT CBansh::Initialize()
{
	m_eCurState = IDLE;
	m_eNextState = IDLE;


	m_fSpeed = SkullDog_Speed;
	m_fAngle = 0.f;

	m_tUnit.vPos = { 1300.f,500.f,0.f };
	m_tUnit.vLength = { 50.f,50.f,0.f };
	m_tUnit.vCollPos = { m_tUnit.vPos.x,m_tUnit.vPos.y + m_tUnit.vLength.y*0.5f,0.f };
	m_tUnit.vLook = { 1.f,0.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"Bansh";
	m_tUnit.Texture.wstrStateKey = L"BanshIdle";

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 6.f;
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

	// ?????? ????
	m_tInfo.iAtt = 1;
	m_tInfo.iGold = 10000;
	m_tInfo.iMaxHp = 500.f;
	m_tInfo.iHp = (int)m_tInfo.iMaxHp;



	return S_OK;
}

HRESULT CBansh::LateInitialize()
{
	return S_OK;
}

void CBansh::Release()
{
}

void CBansh::MakeLine()
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

void CBansh::RectColl(CObj * pObj)
{
	if (HitCheck == false)
	{// ???????? ?????? ??????
		//if (pObj->Get_ItemData().eLength == ITEMINTER::MELEE)
		//{
			D3DXVECTOR3 HitSize = { 2.f,2.f,0.f };
			CAnimationImpl* pImpl = CAnimationImpl::Create(
				L"Monster", L"MonsterMeleeHit", FRAME(0.f, 4.f), 3.f);
			CHitEffect* pEffect = CHitEffect::Create(pImpl, &m_tUnit.matTrans);
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);
	//	}
		int TotalDamage = (int)pObj->Get_ItemData().iAtt;
		CDamageEffectImpl* pDamageImpl = CDamageEffectImpl::Create(
			L"Font", L"DamageFont", TotalDamage);

		CDamageEffect* pDamageEffect = CDamageEffect::Create(pDamageImpl, m_tUnit.vPos, D3DXVECTOR3(20.f, -10.f, 0.f));
		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pDamageEffect);
		m_tInfo.iHp -= TotalDamage; // ?? ?????????? ???????? ??????
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTERHIT);
		CSoundMgr::Get_Instance()->PlaySound(L"MyHitMonster.wav", CSoundMgr::MONSTERHIT);
		Red = 255;
		Green = 0;
		Blue = 0;
		HitCheck = true;
	}
}


void CBansh::StateCheck()
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

void CBansh::FrameChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case STATE::MOVE:
			m_tUnit.Texture.wstrStateKey = L"BanshIdle";
			m_tFrame.fFrameCnt = 6.f;
			FrameSpeed = 1.4f;
			break;
		case STATE::HIT:
			break;
		case STATE::ATTACK:
			m_tUnit.Texture.wstrStateKey = L"BanshAttack";
			m_tFrame.fFrameCnt = 6.f;
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

void CBansh::ActionStart()
{
	UNIT_INFO TargetInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	D3DXVECTOR3 TargetPos = TargetInfo.vPos;
	PlayerDir = TargetPos - m_tUnit.vPos;
	PlayerLength = D3DXVec3Length(&PlayerDir); // ???? ??????


	D3DXVec3Normalize(&PlayerDir, &PlayerDir);
	D3DXVec3Normalize(&m_tUnit.vLook, &m_tUnit.vLook);
	float cos = D3DXVec3Dot(&PlayerDir, &m_tUnit.vLook);
	CreateTimeTarget += CTimeMgr::Get_Instance()->GetDelta();
	if (EndTimeTarget <= CreateTimeTarget)
	{
		if (cos <= 0.f&&m_tUnit.vSize.x > 0)
		{// ???? ??????????? 
		 // ?????????? ?????? ?????? ????.
			m_tUnit.vSize.x *= -1.f;
		}
		else if (cos > 0.f&&m_tUnit.vSize.x < 0)
		{
			m_tUnit.vSize.x *= -1.f;
		}
		CreateTimeTarget = 0.f;
	}
}

void CBansh::Idle()
{
	UNIT_INFO TargetInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	D3DXVECTOR3 TargetPos = TargetInfo.vPos;
	PlayerDir = TargetPos - m_tUnit.vPos;
	PlayerLength = D3DXVec3Length(&PlayerDir); // ???? ??????



	if (PlayerLength < 300.f)
	{ // ???? Idle?????? ?????????? ?? ?????? ???? ????????
		m_eNextState = MOVE;
	}
}

void CBansh::Move()
{
	CreateTimeAttack += CTimeMgr::Get_Instance()->GetDelta();
	if (EndTimeAttack <= CreateTimeAttack)
	{
		m_eNextState = ATTACK;
		CreateTimeAttack = 0.f;
	}
}

void CBansh::Attack()
{
	m_FireTerm += m_pTimeMgr->GetDelta();
	if (1.f <= m_FireTerm)
	{

		D3DXMATRIX matRotZ, matTrans, matWorld;
		D3DXVECTOR3 vDir = { 1.f, 0.f, 0.f };
		CBanshBullet* pBullet = nullptr;

		for (m_FireAngle = 0.f; m_FireAngle < 360.f;)
		{
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_FireAngle));
			D3DXMatrixTranslation(&matTrans, m_tUnit.vPos.x, m_tUnit.vPos.y, 0.f);
			
			matWorld = matRotZ * matTrans;

			vDir = { 1.f, 0.f, 0.f };
			D3DXVec3TransformNormal(&vDir, &vDir, &matWorld);

			pBullet = CBanshBullet::Create(m_tUnit.vPos, vDir);
			CObjMgr::Get_Instance()->Add_Object(OBJID::BANSHBULLET, pBullet);
			m_FireAngle += 30.f;
		}




		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BANSHATTACK);
		CSoundMgr::Get_Instance()->PlaySound(L"MyBanshScream.wav", CSoundMgr::BANSHATTACK);
		m_FireTerm = 0.f;
		m_eNextState = MOVE;
	}
}

void CBansh::CheckReset()
{
	HitCheck = false; // ???? ??????
	Red = 255;
	Green = 255;
	Blue = 255;
	CreateTimeReset = 0.f;
}

CBansh * CBansh::Create()
{
	CBansh* m_pInstance = new CBansh;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
