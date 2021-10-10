#include "stdafx.h"
#include "Boss.h"

#include "AbstractFactory.h"
#include "IcePillar.h"


#include "HitEffect.h"
#include "AnimationImpl.h"

#include "DamageEffect.h"
#include "DamageEffectImpl.h"

#include "IceSpear.h"

#include "BossHpBar.h"

CBoss::CBoss()
	:EnterBoss(false)

{
	CreateStunTime = 0.f;
	EndStunTime = 7.f;
	FrameSpeed = 1.f;
	CreateTimeIceSpear = 0.f;
	EndTimeIceSpear = 1.5f;
	OneTimeCancle = false;

	CreateTimeNormalAttack = 3.f;
	EndTimeNormalAttack = 5.f;
}


CBoss::~CBoss()
{
	Release();
}

void CBoss::Delete_IcePillar(CIcePillar* _p)
{
	NULL_CHECK(_p)
		for (auto iter = m_IcePillar.begin(); iter != m_IcePillar.end(); iter++)
		{

			if ((*iter) == _p)
			{
				m_IcePillar.erase(iter);
				if (m_IcePillar.empty())
				{// 만약 다죽었다면?
					m_eNextState = STATE::STUN;
				}
				return;
			}
		}
}

int CBoss::Update()
{
	CObj::LateInitialize();
	CObj::UpdateRect();
	if (EnterBoss == false)
	{//
		m_eNextState = ENTERBOSS; // 처음 등장할때만
		EnterBoss = true;
	}

	switch (m_eCurState)
	{
	case STATE::IDLE:
		NormalAttStart(); // 1페이즈
		break;
	case STATE::ATT:
		HardAttStart(); // 2페이즈
		break;
	case STATE::TELEPORT:
		break;
	case STATE::STUN:
		Stun();
		break;
	case STATE::DEAD:
		break;

	}

	if (m_tInfo.iHp <= 0.f
		&&m_eNextState!=DEAD
		&&m_eNextState!=REALDEAD)
	{
		m_eNextState = DEAD;
	}

	if (m_eCurState == STATE::DEAD)
	{
		if (m_tFrame.fCurFrame >= m_tFrame.fFrameCnt - 2.f)
		{
			m_eNextState = REALDEAD;
		}
	}

	if (m_eCurState == STATE::ENTERBOSS)
	{
		if (m_tFrame.fCurFrame >= m_tFrame.fFrameCnt - 2.f)
		{
			m_eNextState = STATE::IDLE;
		}
	}

	CObj::MakeWorldMatrix();
	return OBJ_NOEVENT;
}

void CBoss::LateUpdate()
{
	StateCheck();


	CObj::FrameMove(FrameSpeed);
}

void CBoss::Render()
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

	CreateTimeReset += m_pTimeMgr->GetDelta();
	if (EndTimeReset <= CreateTimeReset)
	{
		CheckReset();
	}
}

HRESULT CBoss::Initialize()
{
	m_eCurState = STATE::IDLE;
	m_eNextState = STATE::IDLE;
	m_tUnit.Texture.wstrObjectKey = L"Boss";
	m_tUnit.Texture.wstrStateKey = L"BossIdle";

	m_fSpeed = Boss_Speed;
	m_fAngle = 0.f;

	m_tUnit.vPos = { 946.f,600.f,0.f };
	m_tUnit.vLook = { 1.f,0.f,0.f };
	m_tUnit.vSize = { 3.f,3.f,0.f };
	m_tUnit.vLength = { 110.f,66.f,0.f };
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 6.f;

	m_vOrigin[0] = { 40.f, 0.f, 0.f };
	m_vOrigin[1] = { 30.f, -30.f, 0.f };
	m_vOrigin[2] = { 30.f, 30.f, 0.f };
	m_vOrigin[3] = { -30.f, 30.f, 0.f };

	m_tInfo.iMaxHp = 1000;
	m_tInfo.iHp = (int)m_tInfo.iMaxHp;

	HitCheck = false;
	Red = 255;
	Green = 255;
	Blue = 255;

	CreateTimeReset = 0.f;
	EndTimeReset = 0.2f;

	CUI* pUi = CAbstractFactory<CBossHpBar>::Create();
	CUIMgr::Get_Instance()->Add_Object(UIID::BOSSHPBAR, pUi);
	pUi->Set_Obj(this);

	return S_OK;
}

HRESULT CBoss::LateInitialize()
{
	BulletState = 1;
	return S_OK;
}

void CBoss::Release()
{
	//for (auto iter = m_IcePillar.begin(); iter != m_IcePillar.end(); iter++)
	//{

	//	if ((*iter))
	//	{
	//		m_IcePillar.erase(iter);
	//		
	//	}
	//}
}



void CBoss::HardAttStart()
{
	if (m_IcePillar.empty())
	{// 얼음 기둥들이 없다면 생성해야 한다.
		for (int i = 0; i < 4; ++i)
		{// 기둥이 없을때 4개 생성
		 // 생성시 니플헤임의 주변을 공전하는 걸로
			D3DXVECTOR3 m_vPoint[4] = {};
			CObj* pPillar = CAbstractFactory<CIcePillar>::Create(m_vOrigin[0].x, m_vOrigin[0].y);
			CObjMgr::Get_Instance()->Add_Object(OBJID::ICEPILLAR, pPillar);
			m_IcePillar.emplace_back(dynamic_cast<CIcePillar*>(pPillar));
			dynamic_cast<CIcePillar*>(pPillar)->Set_Boss(this);
			dynamic_cast<CIcePillar*>(pPillar)->Set_Angel(90 * (i)+45);
			dynamic_cast<CIcePillar*>(pPillar)->Set_Number(i);
			BulletState = dynamic_cast<CIcePillar*>(pPillar)->IDLE;
			CreateTimeNormalAttack = 3.f;
		}
	}
	else if (!(m_IcePillar.empty()))
	{// 얼음 기둥이 하나라도 있다면? 공격을 해야지
		int Dead = 1;
		int state = m_IcePillar.front()->Get_State();

		CreateTimeNormalAttack += m_pTimeMgr->GetDelta();
		if (EndTimeNormalAttack <= CreateTimeNormalAttack)
		{
			if (rand() % 3 == 0)
			{
				BulletState = m_IcePillar.front()->PATTERN1;
				for (auto iter2 = m_IcePillar.begin(); iter2 != m_IcePillar.end(); iter2++)
				{
					dynamic_cast<CIcePillar*>((*iter2))->Set_PatternStart(false);
				}
			}
			else if (rand() % 3 == 1)
			{
				BulletState = m_IcePillar.front()->PATTERN2;
				for (auto iter2 = m_IcePillar.begin(); iter2 != m_IcePillar.end(); iter2++)
				{
					dynamic_cast<CIcePillar*>((*iter2))->Set_PatternStart(false);
				}
			}
			else if (rand() % 3 == 2)
			{
				BulletState = m_IcePillar.front()->HARDPATTERN1;
				for (auto iter2 = m_IcePillar.begin(); iter2 != m_IcePillar.end(); iter2++)
				{
					dynamic_cast<CIcePillar*>((*iter2))->Set_PatternStart(false);
				}
			}
			CreateTimeNormalAttack = 0.f;
		}

		if (CKeyMgr::Get_Instance()->KeyDown(KEY_Q))
		{
			BulletState = m_IcePillar.front()->IDLE;
			for (auto iter1 = m_IcePillar.begin(); iter1 != m_IcePillar.end(); iter1++)
			{
				dynamic_cast<CIcePillar*>((*iter1))->Set_PatternStart(false);
			}
		}
		else if (CKeyMgr::Get_Instance()->KeyDown(KEY_W))
		{
			BulletState = m_IcePillar.front()->PATTERN1;
			for (auto iter2 = m_IcePillar.begin(); iter2 != m_IcePillar.end(); iter2++)
			{
				dynamic_cast<CIcePillar*>((*iter2))->Set_PatternStart(false);
			}
		}
		else if (CKeyMgr::Get_Instance()->KeyDown(KEY_E))
		{
			BulletState = m_IcePillar.front()->PATTERN2;
			for (auto iter2 = m_IcePillar.begin(); iter2 != m_IcePillar.end(); iter2++)
			{
				dynamic_cast<CIcePillar*>((*iter2))->Set_PatternStart(false);
			}
		}
		else if (CKeyMgr::Get_Instance()->KeyDown(KEY_R))
		{
			BulletState = m_IcePillar.front()->HARDPATTERN1;
			for (auto iter2 = m_IcePillar.begin(); iter2 != m_IcePillar.end(); iter2++)
			{
				dynamic_cast<CIcePillar*>((*iter2))->Set_PatternStart(false);
			}
		}

		for (auto iter = m_IcePillar.begin(); iter != m_IcePillar.end(); iter++)
		{
			if (state != dynamic_cast<CIcePillar*>((*iter))->ENTERPILLAR)
			{// 생성중이 아니라면 패턴을 쓸 수 있다.
				dynamic_cast<CIcePillar*>((*iter))->Set_State(BulletState);

			}
			//dynamic_cast<CIcePillar*>((*iter))->Set_TransPos()
		}
	}
	CreateTimeIceSpear += m_pTimeMgr->GetDelta();
	if (EndTimeIceSpear <= CreateTimeIceSpear)
	{
		CIceSpear* IceSpear = nullptr;
		D3DXVECTOR3 SpearPos = {};
		D3DXVECTOR3 vDir = {};
		UNIT_INFO TargetInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
		D3DXVECTOR3 TargetPos = TargetInfo.vPos;
		if (rand() % 2 == 0)
		{// 왼쪽에서 창날리기
			SpearPos = { 0.f,TargetPos.y,0.f };
			vDir = { 1.f,0.f,0.f };
		}
		else if (rand() % 2 == 1)
		{// 오른쪽에서 창날리기
			SpearPos = { 2100.f,TargetPos.y,0.f };
			vDir = { -1.f,0.f,0.f };
		}
		IceSpear = CIceSpear::Create(vDir, SpearPos);
		CObjMgr::Get_Instance()->Add_Object(OBJID::ICESPEAR, IceSpear);
		//IceSpear = CIceSpear::Create()
		// 플레이어 좌표(한번만 넘겨줘서 초기 생성 위치를 잡아준다.) , 나아갈 방향
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSSICESPEAR);
		CSoundMgr::Get_Instance()->PlaySound(L"MyIceSpear.wav", CSoundMgr::BOSSICESPEAR);
		
		CreateTimeIceSpear = 0.f;
	}
}

void CBoss::NormalAttStart()
{
	if (m_IcePillar.empty())
	{// 얼음 기둥들이 없다면 생성해야 한다.
		for (int i = 0; i < 4; ++i)
		{// 기둥이 없을때 4개 생성
		 // 생성시 니플헤임의 주변을 공전하는 걸로
			D3DXVECTOR3 m_vPoint[4] = {};
			CObj* pPillar = CAbstractFactory<CIcePillar>::Create(m_vOrigin[0].x, m_vOrigin[0].y);
			CObjMgr::Get_Instance()->Add_Object(OBJID::ICEPILLAR, pPillar);
			m_IcePillar.emplace_back(dynamic_cast<CIcePillar*>(pPillar));
			dynamic_cast<CIcePillar*>(pPillar)->Set_Boss(this);
			dynamic_cast<CIcePillar*>(pPillar)->Set_Angel(90 * (i)+45);
			dynamic_cast<CIcePillar*>(pPillar)->Set_Number(i);
			BulletState = dynamic_cast<CIcePillar*>(pPillar)->IDLE;
			CreateTimeNormalAttack = 3.f;
		}
	}
	else if (!(m_IcePillar.empty()))
	{// 얼음 기둥이 하나라도 있다면? 공격을 해야지
		int Dead = 1;
		int state = m_IcePillar.front()->Get_State();
		
		CreateTimeNormalAttack += m_pTimeMgr->GetDelta();
			if (EndTimeNormalAttack <= CreateTimeNormalAttack)
			{
				if (rand() % 2 == 0)
				{
					BulletState = m_IcePillar.front()->PATTERN1;
					for (auto iter2 = m_IcePillar.begin(); iter2 != m_IcePillar.end(); iter2++)
					{
						dynamic_cast<CIcePillar*>((*iter2))->Set_PatternStart(false);
					}
				}
				else if (rand() % 2 == 1)
				{
					BulletState = m_IcePillar.front()->HARDPATTERN1;
					for (auto iter2 = m_IcePillar.begin(); iter2 != m_IcePillar.end(); iter2++)
					{
						dynamic_cast<CIcePillar*>((*iter2))->Set_PatternStart(false);
					}
				}
				CreateTimeNormalAttack = 0.f;
			}

		if (CKeyMgr::Get_Instance()->KeyDown(KEY_Q))
		{
			BulletState = m_IcePillar.front()->IDLE;
			for (auto iter1 = m_IcePillar.begin(); iter1 != m_IcePillar.end(); iter1++)
			{
				dynamic_cast<CIcePillar*>((*iter1))->Set_PatternStart(false);
			}
		}
		else if (CKeyMgr::Get_Instance()->KeyDown(KEY_W))
		{
			BulletState = m_IcePillar.front()->PATTERN1;
			for (auto iter2 = m_IcePillar.begin(); iter2 != m_IcePillar.end(); iter2++)
			{
				dynamic_cast<CIcePillar*>((*iter2))->Set_PatternStart(false);
			}
		}
		else if (CKeyMgr::Get_Instance()->KeyDown(KEY_E))
		{
			BulletState = m_IcePillar.front()->PATTERN2;
			for (auto iter2 = m_IcePillar.begin(); iter2 != m_IcePillar.end(); iter2++)
			{
				dynamic_cast<CIcePillar*>((*iter2))->Set_PatternStart(false);
			}
		}
		else if (CKeyMgr::Get_Instance()->KeyDown(KEY_R))
		{
			BulletState = m_IcePillar.front()->HARDPATTERN1;
			for (auto iter2 = m_IcePillar.begin(); iter2 != m_IcePillar.end(); iter2++)
			{
				dynamic_cast<CIcePillar*>((*iter2))->Set_PatternStart(false);
			}
		}

		for (auto iter = m_IcePillar.begin(); iter != m_IcePillar.end(); iter++)
		{
			if (state != dynamic_cast<CIcePillar*>((*iter))->ENTERPILLAR)
			{// 생성중이 아니라면 패턴을 쓸 수 있다.
				dynamic_cast<CIcePillar*>((*iter))->Set_State(BulletState);

			}
			//dynamic_cast<CIcePillar*>((*iter))->Set_TransPos()
		}
	}


}

void CBoss::Stun()
{
	CreateStunTime += m_pTimeMgr->GetDelta();
	if (m_tInfo.iHp <= 500)
	{// 반피 이하가 되면 2페이즈
		if (OneTimeCancle == false)
		{
			m_eNextState = ATT;
			CreateStunTime = 0.f;
			OneTimeCancle = true;
			CreateTimeNormalAttack = 4.f;
		}
	}
	if (EndStunTime <= CreateStunTime)
	{
		if (m_tInfo.iHp > 500)
			m_eNextState = IDLE;
		else if (m_tInfo.iHp <= 500)
			m_eNextState = ATT;

		CreateStunTime = 0.f;
		CreateTimeNormalAttack = 4.f;
	}
}



void CBoss::StateCheck()
{
	if (m_eCurState != m_eNextState)
	{

		switch (m_eNextState)
		{
		case STATE::IDLE:
			m_tUnit.Texture.wstrStateKey = L"BossIdle";
			m_tFrame.fFrameCnt = 6.f;
			break;
		case STATE::MOVE:
			break;
		case STATE::ATT:
			m_tUnit.Texture.wstrStateKey = L"BossAttack";
			m_tFrame.fFrameCnt =10.f;
			break;
		case STATE::DEAD:
			m_tUnit.Texture.wstrStateKey = L"BossDie";
			m_tFrame.fFrameCnt = 30.f;
			FrameSpeed = 0.3f;
			break;
		case STATE::TELEPORT:
			break;
		case STATE::STUN:
			m_tUnit.Texture.wstrStateKey = L"BossStun";
			m_tFrame.fFrameCnt = 6.f;
			break;
		case STATE::ENTERBOSS:
			m_tUnit.Texture.wstrStateKey = L"BossCreate";
			m_tFrame.fFrameCnt = 16.f;
			break;
		case STATE::REALDEAD:
			m_tUnit.Texture.wstrStateKey = L"RealDead";
			m_tFrame.fFrameCnt = 1.f;
			break;
		case STATE::END:

			break;
		}
		m_eCurState = m_eNextState;
	}
}

void CBoss::RectColl(CObj * pObj)
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

CBoss * CBoss::Create()
{
	CBoss* m_pInstance = new CBoss;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}

void CBoss::CheckReset()
{
	HitCheck = false; // 다시 쳐맞기
	Red = 255;
	Green = 255;
	Blue = 255;
	CreateTimeReset = 0.f;
}
