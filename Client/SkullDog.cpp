#include "stdafx.h"
#include "SkullDog.h"
#include "AbstractFactory.h"
#include "EnemyHpBar.h"
#include "AnimationImpl.h"
#include "HitEffect.h"

#include "DamageEffect.h"
#include "DamageEffectImpl.h"

#include "LongAttackEffect.h"
#include "MyMoney.h"
#include "NormalSword.h"

CSkullDog::CSkullDog()
	:HitCheck(false)
{
	m_bJumpCheck = false;
	iJumpPower = 8.f;
	CreateTimeJump = 0.f;
	EndTimeJump = 3.f;

	CreateTimeTarget = 0.f;
	EndTimeTarget = 2.f;
	JumpTotal = 0.f;

	Red = 255;
	Green = 255;
	Blue = 255;

	CreateTimeReset = 0.f;
	EndTimeReset = 0.1f;
}


CSkullDog::~CSkullDog()
{
}

int CSkullDog::Update()
{
	if (m_tInfo.iHp <= 0)
	{
		D3DXVECTOR3 HitSize = { 2.f,2.f,0.f };
		CAnimationImpl* pImpl = CAnimationImpl::Create(
			L"Monster", L"MonsterDead", FRAME(0.f, 12.f), 2.f);
		CLongAttackEffect* pEffect = CLongAttackEffect::Create(
			pImpl, m_tUnit.vPos, HitSize);
		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);
		
		CObjMgr::Get_Instance()->Add_Object(OBJID::MONEY, CAbstractFactory<CMyMoney>::Create(m_tUnit.vPos.x, m_tUnit.vPos.y));
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTERDEAD);
		CSoundMgr::Get_Instance()->PlaySound(L"MyMonsterDie.wav", CSoundMgr::MONSTERDEAD);
		return OBJ_DEAD;
	}
	CObj::LateInitialize();
	CObj::UpdateRect();

	GravityTile();
	GravityLine();

	CObj::MakeWorldMatrix();
	ActionStart();
	StateCheck();

	m_tUnit.vCollPos = { m_tUnit.vPos.x,m_tUnit.vPos.y + m_tUnit.vLength.y*0.5f,0.f };
	return OBJ_NOEVENT;
}

void CSkullDog::LateUpdate()
{
	FrameChange();
	CObj::FrameMove(FrameSpeed);
	//MakeLine();

	m_bColl = false; // 충돌처리 초기화
}

void CSkullDog::Render()
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

HRESULT CSkullDog::Initialize()
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
	m_tUnit.Texture.wstrObjectKey = L"SkullDog";
	m_tUnit.Texture.wstrStateKey = L"SkullDogIdle";

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 4.f;
	FrameSpeed = 1.f;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);


	m_tUnit.vSize.x = m_tUnit.vLength.x / pTexInfo->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / pTexInfo->tImgInfo.Height;

	Current_tick = clock();
	Next_tick = (clock_t)0.f;

	JumpStop = false;

	CUI* pUi = CAbstractFactory<CEnemyHpBar>::Create();
	CUIMgr::Get_Instance()->Add_Object(UIID::ENEMYLIFEBAR, pUi);
	pUi->Set_Obj(this);

	// 해골개의 정보
	m_tInfo.iAtt = 10;
	m_tInfo.iGold = 0;
	m_tInfo.iMaxHp = 200.f;
	m_tInfo.iHp = (int)m_tInfo.iMaxHp;
	


	return S_OK;
}

HRESULT CSkullDog::LateInitialize()
{
	return S_OK;
}

void CSkullDog::Release()
{
}

void CSkullDog::MakeLine()
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

void CSkullDog::RectColl(CObj * pObj)
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
		//}
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

void CSkullDog::GravityLine()
{
	// 중력 처리를 해보자
	float fY = 0.f;
	float fY1 = 0.f;
	bool bIsColl = CLineMgr::Get_Instance()->LineCollisionMove((&m_tUnit.vCollPos), &fY);
	bool bIsUp = CLineMgr::Get_Instance()->LineCollisionDash((&m_tUnit.vCollPos), &fY1);

	if (m_eNextState != JUMP)
	{
		//if (bIsUp&& m_tUnit.vCollPos.y +20.f >fY1)
		//{// 떨어진 강아지를 위로 올려줘야한다.
		//	m_tUnit.vPos.y = fY1 - (m_tUnit.vLength.y*0.5f);
		//}



		Next_tick = ((clock() - Current_tick) + 90) / 90;

		if (bIsColl == true)
		{// 만약 라인과 충돌 중이라면 플레이어 중력을 초기화 해줘야한다.
			Current_tick = clock();

		}
		else if (bIsColl == false)
		{
			float ScrollSpeed = (0.3f*Next_tick*Next_tick*0.5f);

			m_tUnit.vPos.y += ScrollSpeed;

		}

	}


}

void CSkullDog::GravityTile()
{
	if (m_bColl == true)
	{// 만약 타일과 충돌 중이라면
		Current_tick = clock();
		JumpStop = true; // 점프 안되면 보자
	}
}

void CSkullDog::StateCheck()
{
	switch (m_eCurState)
	{
	case STATE::IDLE:
		Idle();
		break;
	case STATE::MOVE:
		Move();
		break;
	case STATE::FIRE:
		break;
	case STATE::HIT:
		break;
	case STATE::JUMP:
		Jump();
		break;
	default:
		break;
	}
}

void CSkullDog::FrameChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case STATE::MOVE:
			m_tUnit.Texture.wstrStateKey = L"SkullDogRun";
			m_tFrame.fFrameCnt = 7.f;
			FrameSpeed = 1.4f;
			break;
		case STATE::HIT:
			break;
		case STATE::FIRE:
			break;
		case STATE::JUMP:
			m_tUnit.Texture.wstrStateKey = L"SkullDogJump";
			m_tFrame.fFrameCnt = 1.f;
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

void CSkullDog::ActionStart()
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

void CSkullDog::Idle()
{
	UNIT_INFO TargetInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	D3DXVECTOR3 TargetPos = TargetInfo.vPos;
	PlayerDir = TargetPos - m_tUnit.vPos;
	PlayerLength = D3DXVec3Length(&PlayerDir); // 거리 구하고



	if (PlayerLength < 300.f)
	{ // 초기 Idle이지만 플레이어가 이 위치에 오면 움직여라
		m_eNextState = MOVE;
	}
}

void CSkullDog::Move()
{

	float fY = 0.f;
	bool bIsColl = CLineMgr::Get_Instance()->LineCollisionMove((&m_tUnit.vCollPos), &fY);

	if (m_tUnit.vSize.x < 0.f)
	{
		m_tUnit.vPos.x -= m_fSpeed;

	}
	else if (m_tUnit.vSize.x > 0.f)
	{
		m_tUnit.vPos.x += m_fSpeed;

	}
	if (bIsColl)
	{

		m_tUnit.vPos.y = fY - (m_tUnit.vLength.y * 0.5f);

	}
	
	CreateTimeJump += CTimeMgr::Get_Instance()->GetDelta();
	if (EndTimeJump <= CreateTimeJump)
	{
		m_eNextState = JUMP;
		CreateTimeJump = 0.f;
	}

}

void CSkullDog::Jump()
{
	float fY = 0.f;
	bool bIsColl = CLineMgr::Get_Instance()->LineCollision((&m_tUnit.vCollPos), &fY);
	float fY1 = 0.f;
	bool bIsUp = CLineMgr::Get_Instance()->LineCollisionMove((&m_tUnit.vCollPos), &fY1);

	if (m_tUnit.vSize.x < 0.f)
	{
		m_tUnit.vPos.x -= m_fSpeed;

	}
	else if (m_tUnit.vSize.x > 0.f)
	{
		m_tUnit.vPos.x += m_fSpeed;

	}
	if (m_bJumpCheck == false)
	{
		Jump_Current_Tick = clock();
		JumpStop = false;
		m_bJumpCheck = true;
	}
	else if (m_bJumpCheck == true)
	{
		Jump_Next_Tick = ((clock() - Jump_Current_Tick) + 100) / 100;
		JumpTotal = (iJumpPower*Jump_Next_Tick) - (3.8f*Jump_Next_Tick*Jump_Next_Tick*0.5f);
		m_tUnit.vPos.y -= JumpTotal;
		m_tUnit.vCollPos.y -= JumpTotal;
		if (bIsColl&& m_tUnit.vCollPos.y > fY)
		{
			m_bJumpCheck = false;

			m_tUnit.vCollPos.y = fY;
			m_tUnit.vPos.y = m_tUnit.vCollPos.y - (m_tUnit.vLength.y * 0.5f);
			m_eNextState = MOVE; // 점프가 끝나면 무브로 바꿔주자

		}
		//if (bIsUp&& m_tUnit.vCollPos.y+30.f >fY1)
		//{
		//	m_bJumpCheck = false;

		//	m_tUnit.vCollPos.y = fY1;
		//	m_tUnit.vPos.y = fY1 - (m_tUnit.vLength.y * 0.5f);
		//	m_eNextState = MOVE; // 점프가 끝나면 무브로 바꿔주자
		//}
	}
}

void CSkullDog::CheckReset()
{
	HitCheck = false; // 다시 쳐맞기
	Red = 255;
	Green = 255;
	Blue = 255;
	CreateTimeReset = 0.f;
}

CSkullDog * CSkullDog::Create()
{
	CSkullDog* m_pInstance = new CSkullDog;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
