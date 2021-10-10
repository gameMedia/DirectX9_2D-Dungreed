#include "stdafx.h"
#include "MainHand.h"
#include "Inventory.h"
#include "Player.h"
#include "Mouse.h"
#include "MeleeAttackEffect.h"

#include "NormalEffect.h"
#include "AnimationImpl.h"
#include "SubHand.h"
#include "BomerangAct.h"
#include "AbstractFactory.h"
#include "MyWeaponShop.h"
#include "RevolverBullet.h"
#include "MyRestaurant.h"
#include "BatItemBullet.h"


CMainHand::CMainHand()
	:m_bIsAttack(false)
	, InitOneTimeHand(false)
	, m_bAttackCheck(false)
	, HandThrow(false)
	, HandReturn(false)
	, OnOffInven(false)
	, AttackBomerang(false)
{
}


CMainHand::~CMainHand()
{
}

void CMainHand::Set_FirstMainItem(ITEM_DATA _FirstInven)
{
	First = _FirstInven;
	m_eNextState = STATE::EQUIPEDFIRST;
}

void CMainHand::Set_SecondMainItem(ITEM_DATA _SecondInven)
{
	Second = _SecondInven;

}

int CMainHand::Update()
{
	HandSetting();

	UNIT_INFO& playerPos = m_pPlayer->Get_Pos();

	D3DXVECTOR3 vLook = { 0.f,-1.f,0.f };

	Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());

	MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
	MousevDir.x += CScrollMgr::GetScrollPos(0);
	MousevDir.y += CScrollMgr::GetScrollPos(1);
	D3DXVec3Normalize(&MousevDir, &MousevDir);
	D3DXVec3Normalize(&vLook, &vLook);
	float cos = D3DXVec3Dot(&MousevDir, &vLook);

	/*if (AttackCount == 1)
	{*/
	m_RotAngle = D3DXToDegree(acosf(cos));
	//}
	if (AttackCount == -1)
	{

		/*if(m_RotAngle > m_RotAngle2 - 90.f)*/
		m_RotAngle2 = D3DXToDegree(acosf(cos)) + 150.f;

	}



	OnOffInven = dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_InvenSetting();
	//UNIT_INFO& playerPos = m_pPlayer->Get_Pos();
	if (CKeyMgr::Get_Instance()->KeyDown(KEY_ONE))
	{
		m_eNextState = STATE::EQUIPEDFIRST;
		dynamic_cast<CSubHand*>(CObjMgr::Get_Instance()->Get_SubHand())->Set_State(STATE::EQUIPEDFIRST);
	}
	if (CKeyMgr::Get_Instance()->KeyDown(KEY_TWO))
	{
		m_eNextState = STATE::EQUIPEDSECOND;
		dynamic_cast<CSubHand*>(CObjMgr::Get_Instance()->Get_SubHand())->Set_State(STATE::EQUIPEDSECOND);
	}
	if (OnOffInven == false &&
		CMyWeaponShop::ShopSetting == false &&
		CMyRestaurant::InnSetting == false)
	{// 인벤이 닫혔을때
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
		{
			if (m_bIsAttack == false)
				m_bIsAttack = true;
		}
	}
	if (m_bIsAttack == true)
	{
		Attack();
	}

	return 0;
}

void CMainHand::LateUpdate()
{
	StateCheck();
	MakeWorldMatrix();
}

void CMainHand::Render()
{
	if (AttackBomerang == false)
	{
		const TEX_INFO* MainItem = m_pTextureMgr->GetTexInfo(
			Main.Texture.wstrObjectKey, Main.Texture.wstrStateKey, Main.Texture.iIndex);
		NULL_CHECK(MainItem);

		float fCenterX = 0;

		float fCenterY = 0;
		if (Main.szName == L"Revolver")
		{
			fCenterX = 0;
		}
		else
			fCenterX = MainItem->tImgInfo.Width*0.5f;

		if (Main.szName == L"Bomerang") // 부메랑 들고있으면 좌표 수정
			fCenterY = MainItem->tImgInfo.Height*0.5f;
		else
			fCenterY = MainItem->tImgInfo.Height;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(Main.matWorld));
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			MainItem->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (Main.szName != L"")
	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width*0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height*0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}


void CMainHand::Release()
{
}

void CMainHand::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);
	//D3DXMatrixRotationZ(&m_tUnit.matRotZ, D3DXToRadian(-m_RotAngle)); // 자전
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x,
		m_tUnit.vPos.y, 0.f);

	if (!(m_pPlayer == nullptr))
		m_tUnit.matParent = m_pPlayer->Get_Info().matWorld; // 부모 행렬

	m_tUnit.matWorld = m_tUnit.matScale  * m_tUnit.matTrans * m_tUnit.matParent;
	//m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matWorld;



	D3DXMatrixScaling(&Main.matScale, Main.vSize.x, Main.vSize.y, 0.f);

	if (Main.szName == L"Revolver")
	{
		if (AttackCount == -1)
			D3DXMatrixRotationZ(&m_tUnit.matRotZ, D3DXToRadian(m_RotAngle2 - 90.f)); // 자전
		else
			D3DXMatrixRotationZ(&m_tUnit.matRotZ, D3DXToRadian(m_RotAngle - 90.f)); // 자전
	}
	else
	{
		if (AttackCount == -1)
			D3DXMatrixRotationZ(&m_tUnit.matRotZ, D3DXToRadian(m_RotAngle2 - 60.f)); // 자전
		else
			D3DXMatrixRotationZ(&m_tUnit.matRotZ, D3DXToRadian(m_RotAngle - 60.f)); // 자전
	}
	D3DXMatrixTranslation(&Main.matTrans,
		Main.vPos.x,
		Main.vPos.y, 0.f);



	Main.matWorld = Main.matScale *m_tUnit.matRotZ * Main.matTrans * m_tUnit.matParent;

}

void CMainHand::StateCheck()
{

	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case STATE::UNEQUIPED:
			Main.iAtt = 1;
			Main.szName = L"";
			Main.Texture.wstrObjectKey = L"Player";
			Main.Texture.wstrStateKey = L"Hand";
			Main.Texture.iIndex = 0;

			Main.vSize = { 1.f,1.f,0.f };
			Main.vLength = { 3.f,3.f,0.f };
			Main.vPos.x = 6.f;
			Main.vPos.y = 7.f;

			Main.vSize.x = HandSize.x;
			Main.vSize.y = HandSize.y;



			break;

		case STATE::EQUIPEDFIRST:
			Main = First;

			break;
		case STATE::EQUIPEDSECOND:
			Main = Second;

			break;

		case STATE::END:
			break;
		}

		m_eCurState = m_eNextState;

		if (Main.szName == L"")
		{// 만약 장착할 무기가 없다면 맨주먹으로
			m_eNextState = UNEQUIPED;
		}
		else if (Main.szName == L"Revolver")
		{
			Main.vPos = { 5.f,6.f,0.f };
			Main.vSize = { 1.f,1.f,0.f };
		}
		else
		{
			Main.vPos = m_tUnit.vPos;
			Main.vSize = { 1.f,1.f,0.f };
		}


	}
}

void CMainHand::UnEquiped()
{
	UNIT_INFO& playerPos = m_pPlayer->Get_Pos();
	if (InitOneTimeHand == false)
	{
		D3DXVECTOR3 vLook = { 1.f,0.f,0.f };
		D3DXMATRIX matRotZ;
		Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
		MousevDir.x += CScrollMgr::GetScrollPos(0);
		MousevDir.y += CScrollMgr::GetScrollPos(1);
		D3DXVec3Normalize(&MousevDir, &MousevDir);
		D3DXVec3Normalize(&vLook, &vLook);
		Dir = MousevDir;
		AttackPower = 7.f;

		AttackStop = false;
		InitOneTimeHand = true;
	}
	if (InitOneTimeHand == true)
	{
		if (m_bAttackCheck == false)
		{
			Current_tick = clock();
			m_bAttackCheck = true;
			AttackStop = false;
			HandThrow = true; //주먹을 던져
		}
		if (m_bAttackCheck == true)
		{
			Next_tick = ((clock() - Current_tick) + 100) / 100;
			if (HandThrow == true)
			{
				AttackPowerTotal = (AttackPower*Next_tick) - (GRAVITY*Next_tick*Next_tick*0.5f);
				Total_Attack = (Dir*AttackPowerTotal);
				if (playerPos.vSize.x > 0.f)
					Main.vPos += Total_Attack;
				else if (playerPos.vSize.x <= 0.f)
				{
					Main.vPos.x -= Total_Attack.x;
					Main.vPos.y += Total_Attack.y;
				}
				if ((AttackPower*Next_tick) <= (GRAVITY*Next_tick*Next_tick*0.5f))
				{// 힘이 끝에 도달했을때
					HandThrow = false;
					Current_tick = clock();
					HandReturn = true;
				}
			}
			if (HandReturn == true)
			{
				D3DXVECTOR3 vLook = { 1.f,0.f,0.f };
				D3DXVECTOR3 Direction = m_tUnit.vPos - Main.vPos;
				D3DXVec3Normalize(&Direction, &Direction);
				D3DXVec3Normalize(&vLook, &vLook);
				Dir = Direction;
				AttackPowerTotal = ((AttackPower*Next_tick) - (GRAVITY*Next_tick*Next_tick*0.5f));
				Total_Attack = (Dir*AttackPowerTotal);
				Main.vPos += Total_Attack;
				float Length = D3DXVec3Length(&Total_Attack);

				if (Length <= 4.f)
				{
					HandReturn = false;
					Main.vPos = m_tUnit.vPos;
					m_bAttackCheck = false;
					InitOneTimeHand = false;
					m_bIsAttack = false;
				}
			}



		}
	}
}

void CMainHand::EquipedFirst()
{

	if (Main.eLength == ITEMINTER::MELEE)
	{// 근접무기라면 근접공격을 나가야지

		AttackCount *= -1;
		UNIT_INFO& playerPos = m_pPlayer->Get_Pos();

		D3DXVECTOR3 vLook = { 0.f,-1.f,0.f };
		D3DXVECTOR3 vAttackLook = { 1.f,0.f,0.f };

		Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
		MousevDir.x += CScrollMgr::GetScrollPos(0);
		MousevDir.y += CScrollMgr::GetScrollPos(1);
		D3DXVec3Normalize(&MousevDir, &MousevDir);
		D3DXVec3Normalize(&vLook, &vLook);
		float cos = D3DXVec3Dot(&MousevDir, &vLook);
		m_RotAngle2 = D3DXToDegree(acosf(cos));


		D3DXVec3Normalize(&vAttackLook, &vAttackLook);
		float Attackcos = m_RotAngle2;


		D3DXMATRIX matRotZ, matTrans, matWorld, matScale;


		D3DXMatrixScaling(&matScale,
			(Main.vSize.x),
			(Main.vSize.y),
			0.f);


		if (AttackCount == -1)
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_RotAngle2 - 60.f)); // 자전
		else
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_RotAngle - 60.f)); // 자전

		if (playerPos.vSize.x >= 0.f)
		{
			D3DXMatrixTranslation(&matTrans,
				Main.vPos.x + MousevDir.x*40.f,
				Main.vPos.y + 10.f + MousevDir.y*40.f, 0.f);
		}
		else if (playerPos.vSize.x < 0.f)
		{
			D3DXMatrixTranslation(&matTrans,
				Main.vPos.x - MousevDir.x*40.f,
				Main.vPos.y + 10.f + MousevDir.y*40.f, 0.f);
		}
		D3DXMATRIX matScroll;
		D3DXMatrixTranslation(&matScroll,
			CScrollMgr::GetScrollPos(0),
			CScrollMgr::GetScrollPos(1), 0.f);

		matWorld = matScale * matRotZ * matTrans *playerPos.matWorld*matScroll;

		if (Main.szName == L"FireSword")
		{
			ITEM_DATA Damage = Main;
			ITEM_DATA SubHandData = dynamic_cast<CSubHand*>(CObjMgr::Get_Instance()->Get_SubHand())->Get_Main();
			Damage.iAtt = Main.iAtt + SubHandData.iAtt+CPlayer::PlusAtt;
			CAnimationImpl* pImpl = CAnimationImpl::Create(
				L"FireSword", L"Swing", FRAME(0.f, 5.f), 10.0f);

			CMeleeAttackEffect* pEffect = CMeleeAttackEffect::Create(
				pImpl, matWorld, Damage, Attackcos);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MELEEATTACK, pEffect);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYERSWING);
			CSoundMgr::Get_Instance()->PlaySound(L"MySwing.wav", CSoundMgr::PLAYERSWING);
		}

		m_bIsAttack = false;

		//m_RotAngle += 90.f;
	}
	if (Main.eLength == ITEMINTER::LONG)
	{// 원거리 공격이면 원거리 기술이 나간다.
		if (Main.szName == L"Bomerang")
		{// 부메랑이라면?
			if (AttackBomerang == false)
			{// 부메랑 공격중이 아니라면
				UNIT_INFO& playerPos = m_pPlayer->Get_Pos();

				ITEM_DATA Damage = Main;
				ITEM_DATA SubHandData = dynamic_cast<CSubHand*>(CObjMgr::Get_Instance()->Get_SubHand())->Get_Main();
				Damage.iAtt = Main.iAtt + SubHandData.iAtt + CPlayer::PlusAtt;
				Damage.vPos = Main.vPos + playerPos.vPos;

				D3DXVECTOR3 vLook = { 0.f,-1.f,0.f };
				D3DXVECTOR3 vAttackLook = { 1.f,0.f,0.f };

				Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
				MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
				MousevDir.x += CScrollMgr::GetScrollPos(0);
				MousevDir.y += CScrollMgr::GetScrollPos(1);
				D3DXVec3Normalize(&MousevDir, &MousevDir);
				D3DXVec3Normalize(&vLook, &vLook);
				float cos = D3DXVec3Dot(&MousevDir, &vLook);
				m_RotAngle2 = D3DXToDegree(acosf(cos));
				CObj* pBomerang = CAbstractFactory<CBomerangAct>::Create(MousevDir, Damage);
				CObjMgr::Get_Instance()->Add_Object(
					OBJID::ACTBOMERANG, pBomerang);

				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYERSWING);
				CSoundMgr::Get_Instance()->PlaySound(L"MySwing.wav", CSoundMgr::PLAYERSWING);

				AttackBomerang = true; // 부메랑 던지는 중에는 다시 못던지게 만들어줌
			}
		}
		else if (Main.szName == L"Revolver")
		{
			UNIT_INFO& playerPos = m_pPlayer->Get_Pos();

			ITEM_DATA Damage = Main;
			ITEM_DATA SubHandData = dynamic_cast<CSubHand*>(CObjMgr::Get_Instance()->Get_SubHand())->Get_Main();
			Damage.iAtt = Main.iAtt + SubHandData.iAtt;
			Damage.vPos = Main.vPos + playerPos.vPos;
			D3DXVECTOR3 vLook = { 0.f,-1.f,0.f };
			D3DXVECTOR3 vAttackLook = { 1.f,0.f,0.f };

			Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
			MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
			MousevDir.x += CScrollMgr::GetScrollPos(0);
			MousevDir.y += CScrollMgr::GetScrollPos(1);
			D3DXVec3Normalize(&MousevDir, &MousevDir);
			D3DXVec3Normalize(&vLook, &vLook);
			Damage.vPos += MousevDir*50.f;
			float cos = D3DXVec3Dot(&MousevDir, &vLook);
			m_RotAngle2 = D3DXToDegree(acosf(cos));
			CRevolverBullet* pBullet = CRevolverBullet::Create(Damage.vPos, MousevDir);
			CObjMgr::Get_Instance()->Add_Object(OBJID::REVOLVERBULLET, pBullet);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::REVOLVERBULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"MyRevolverAtt.wav", CSoundMgr::REVOLVERBULLET);
		}


		m_bIsAttack = false;
	}



	if (dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[16].szName == L"BatItem")
	{
		UNIT_INFO& BatInfo = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Bat();
		UNIT_INFO& playerPos = m_pPlayer->Get_Pos();
		D3DXMATRIX matWorld,matTrans;
		UNIT_INFO Damage;
		if (playerPos.vSize.x >= 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x-50.f + playerPos.vPos.x,
				BatInfo.vPos.y-45.f + playerPos.vPos.y,0.f };

		}
		else if (playerPos.vSize.x < 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x+100.f + playerPos.vPos.x,
				BatInfo.vPos.y-45.f + playerPos.vPos.y,0.f };
		}
		Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
		MousevDir.x += CScrollMgr::GetScrollPos(0);
		MousevDir.y += CScrollMgr::GetScrollPos(1);
		D3DXVec3Normalize(&MousevDir, &MousevDir);

		BatItemBullet* pBullet = BatItemBullet::Create(Damage.vPos, MousevDir);
		CObjMgr::Get_Instance()->Add_Object(OBJID::BATITEMBULLET, pBullet);

	}
	else if (dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[17].szName == L"BatItem")
	{
		UNIT_INFO& BatInfo = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Bat();
		UNIT_INFO& playerPos = m_pPlayer->Get_Pos();
		D3DXMATRIX matWorld, matTrans;
		UNIT_INFO Damage;
		if (playerPos.vSize.x >= 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x - 50.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };

		}
		else if (playerPos.vSize.x < 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x + 100.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };
		}
		Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
		MousevDir.x += CScrollMgr::GetScrollPos(0);
		MousevDir.y += CScrollMgr::GetScrollPos(1);
		D3DXVec3Normalize(&MousevDir, &MousevDir);

		BatItemBullet* pBullet = BatItemBullet::Create(Damage.vPos, MousevDir);
		CObjMgr::Get_Instance()->Add_Object(OBJID::BATITEMBULLET, pBullet);

	}
	else if (dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[18].szName == L"BatItem")
	{
		UNIT_INFO& BatInfo = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Bat();
		UNIT_INFO& playerPos = m_pPlayer->Get_Pos();
		D3DXMATRIX matWorld, matTrans;
		UNIT_INFO Damage;
		if (playerPos.vSize.x >= 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x - 50.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };

		}
		else if (playerPos.vSize.x < 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x + 100.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };
		}
		Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
		MousevDir.x += CScrollMgr::GetScrollPos(0);
		MousevDir.y += CScrollMgr::GetScrollPos(1);
		D3DXVec3Normalize(&MousevDir, &MousevDir);

		BatItemBullet* pBullet = BatItemBullet::Create(Damage.vPos, MousevDir);
		CObjMgr::Get_Instance()->Add_Object(OBJID::BATITEMBULLET, pBullet);

	}
	else if (dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[19].szName == L"BatItem")
	{
		UNIT_INFO& BatInfo = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Bat();
		UNIT_INFO& playerPos = m_pPlayer->Get_Pos();
		D3DXMATRIX matWorld, matTrans;
		UNIT_INFO Damage;
		if (playerPos.vSize.x >= 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x - 50.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };

		}
		else if (playerPos.vSize.x < 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x + 100.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };
		}
		Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
		MousevDir.x += CScrollMgr::GetScrollPos(0);
		MousevDir.y += CScrollMgr::GetScrollPos(1);
		D3DXVec3Normalize(&MousevDir, &MousevDir);

		BatItemBullet* pBullet = BatItemBullet::Create(Damage.vPos, MousevDir);
		CObjMgr::Get_Instance()->Add_Object(OBJID::BATITEMBULLET, pBullet);

	}


}

void CMainHand::EquipedSecond()
{
	if (Main.eLength == ITEMINTER::MELEE)
	{// 근접무기라면 근접공격을 나가야지

		AttackCount *= -1;
		UNIT_INFO& playerPos = m_pPlayer->Get_Pos();

		D3DXVECTOR3 vLook = { 0.f,-1.f,0.f };
		D3DXVECTOR3 vAttackLook = { 1.f,0.f,0.f };

		Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
		MousevDir.x += CScrollMgr::GetScrollPos(0);
		MousevDir.y += CScrollMgr::GetScrollPos(1);
		D3DXVec3Normalize(&MousevDir, &MousevDir);
		D3DXVec3Normalize(&vLook, &vLook);
		float cos = D3DXVec3Dot(&MousevDir, &vLook);
		m_RotAngle2 = D3DXToDegree(acosf(cos));


		D3DXVec3Normalize(&vAttackLook, &vAttackLook);
		float Attackcos = m_RotAngle2;


		D3DXMATRIX matRotZ, matTrans, matWorld, matScale;


		D3DXMatrixScaling(&matScale,
			(Main.vSize.x),
			(Main.vSize.y),
			0.f);


		if (AttackCount == -1)
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_RotAngle2 - 60.f)); // 자전
		else
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_RotAngle - 60.f)); // 자전

		if (playerPos.vSize.x >= 0.f)
		{
			D3DXMatrixTranslation(&matTrans,
				Main.vPos.x + MousevDir.x*40.f,
				Main.vPos.y + 10.f + MousevDir.y*40.f, 0.f);
		}
		else if (playerPos.vSize.x < 0.f)
		{
			D3DXMatrixTranslation(&matTrans,
				Main.vPos.x - MousevDir.x*40.f,
				Main.vPos.y + 10.f + MousevDir.y*40.f, 0.f);
		}
		D3DXMATRIX matScroll;
		D3DXMatrixTranslation(&matScroll,
			CScrollMgr::GetScrollPos(0),
			CScrollMgr::GetScrollPos(1), 0.f);

		matWorld = matScale * matRotZ * matTrans *playerPos.matWorld*matScroll;

		if (Main.szName == L"FireSword")
		{
			ITEM_DATA Damage = Main;
			ITEM_DATA SubHandData = dynamic_cast<CSubHand*>(CObjMgr::Get_Instance()->Get_SubHand())->Get_Main();
			Damage.iAtt = Main.iAtt + SubHandData.iAtt + CPlayer::PlusAtt;
			CAnimationImpl* pImpl = CAnimationImpl::Create(
				L"FireSword", L"Swing", FRAME(0.f, 5.f), 10.0f);

			CMeleeAttackEffect* pEffect = CMeleeAttackEffect::Create(
				pImpl, matWorld, Damage, Attackcos);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MELEEATTACK, pEffect);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYERSWING);
			CSoundMgr::Get_Instance()->PlaySound(L"MySwing.wav", CSoundMgr::PLAYERSWING);
		}

		m_bIsAttack = false;

		//m_RotAngle += 90.f;

	}
	if (Main.eLength == ITEMINTER::LONG)
	{// 원거리 공격이면 원거리 기술이 나간다.
		if (Main.szName == L"Bomerang")
		{// 부메랑이라면?
			if (AttackBomerang == false)
			{// 부메랑 공격중이 아니라면
				UNIT_INFO& playerPos = m_pPlayer->Get_Pos();

				ITEM_DATA Damage = Main;
				ITEM_DATA SubHandData = dynamic_cast<CSubHand*>(CObjMgr::Get_Instance()->Get_SubHand())->Get_Main();
				Damage.iAtt = Main.iAtt + SubHandData.iAtt + CPlayer::PlusAtt;
				Damage.vPos = Main.vPos + playerPos.vPos;

				D3DXVECTOR3 vLook = { 0.f,-1.f,0.f };
				D3DXVECTOR3 vAttackLook = { 1.f,0.f,0.f };

				Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
				MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
				MousevDir.x += CScrollMgr::GetScrollPos(0);
				MousevDir.y += CScrollMgr::GetScrollPos(1);
				D3DXVec3Normalize(&MousevDir, &MousevDir);
				D3DXVec3Normalize(&vLook, &vLook);
				float cos = D3DXVec3Dot(&MousevDir, &vLook);
				m_RotAngle2 = D3DXToDegree(acosf(cos));
				CObj* pBomerang = CAbstractFactory<CBomerangAct>::Create(MousevDir, Damage);
				CObjMgr::Get_Instance()->Add_Object(
					OBJID::ACTBOMERANG, pBomerang);

				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYERSWING);
				CSoundMgr::Get_Instance()->PlaySound(L"MySwing.wav", CSoundMgr::PLAYERSWING);

				AttackBomerang = true; // 부메랑 던지는 중에는 다시 못던지게 만들어줌
			}
		}
		else if (Main.szName == L"Revolver")
		{
			UNIT_INFO& playerPos = m_pPlayer->Get_Pos();

			ITEM_DATA Damage = Main;
			ITEM_DATA SubHandData = dynamic_cast<CSubHand*>(CObjMgr::Get_Instance()->Get_SubHand())->Get_Main();
			Damage.iAtt = Main.iAtt + SubHandData.iAtt;
			Damage.vPos = Main.vPos + playerPos.vPos;
			D3DXVECTOR3 vLook = { 0.f,-1.f,0.f };
			D3DXVECTOR3 vAttackLook = { 1.f,0.f,0.f };

			Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
			MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
			MousevDir.x += CScrollMgr::GetScrollPos(0);
			MousevDir.y += CScrollMgr::GetScrollPos(1);
			D3DXVec3Normalize(&MousevDir, &MousevDir);
			Damage.vPos += MousevDir*50.f;
			D3DXVec3Normalize(&vLook, &vLook);
			float cos = D3DXVec3Dot(&MousevDir, &vLook);
			m_RotAngle2 = D3DXToDegree(acosf(cos));
			CRevolverBullet* pBullet = CRevolverBullet::Create(Damage.vPos, MousevDir);
			CObjMgr::Get_Instance()->Add_Object(OBJID::REVOLVERBULLET, pBullet);

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::REVOLVERBULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"MyRevolverAtt.wav", CSoundMgr::REVOLVERBULLET);
		}


		m_bIsAttack = false;
	}
	if (dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[16].szName == L"BatItem")
	{
		UNIT_INFO& BatInfo = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Bat();
		UNIT_INFO& playerPos = m_pPlayer->Get_Pos();
		D3DXMATRIX matWorld, matTrans;
		UNIT_INFO Damage;
		if (playerPos.vSize.x >= 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x - 50.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };

		}
		else if (playerPos.vSize.x < 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x + 100.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };
		}
		Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
		MousevDir.x += CScrollMgr::GetScrollPos(0);
		MousevDir.y += CScrollMgr::GetScrollPos(1);
		D3DXVec3Normalize(&MousevDir, &MousevDir);

		BatItemBullet* pBullet = BatItemBullet::Create(Damage.vPos, MousevDir);
		CObjMgr::Get_Instance()->Add_Object(OBJID::BATITEMBULLET, pBullet);

	}
	else if (dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[17].szName == L"BatItem")
	{
		UNIT_INFO& BatInfo = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Bat();
		UNIT_INFO& playerPos = m_pPlayer->Get_Pos();
		D3DXMATRIX matWorld, matTrans;
		UNIT_INFO Damage;
		if (playerPos.vSize.x >= 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x - 50.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };

		}
		else if (playerPos.vSize.x < 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x + 100.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };
		}
		Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
		MousevDir.x += CScrollMgr::GetScrollPos(0);
		MousevDir.y += CScrollMgr::GetScrollPos(1);
		D3DXVec3Normalize(&MousevDir, &MousevDir);

		BatItemBullet* pBullet = BatItemBullet::Create(Damage.vPos, MousevDir);
		CObjMgr::Get_Instance()->Add_Object(OBJID::BATITEMBULLET, pBullet);

	}
	else if (dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[18].szName == L"BatItem")
	{
		UNIT_INFO& BatInfo = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Bat();
		UNIT_INFO& playerPos = m_pPlayer->Get_Pos();
		D3DXMATRIX matWorld, matTrans;
		UNIT_INFO Damage;
		if (playerPos.vSize.x >= 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x - 50.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };

		}
		else if (playerPos.vSize.x < 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x + 100.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };
		}
		Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
		MousevDir.x += CScrollMgr::GetScrollPos(0);
		MousevDir.y += CScrollMgr::GetScrollPos(1);
		D3DXVec3Normalize(&MousevDir, &MousevDir);

		BatItemBullet* pBullet = BatItemBullet::Create(Damage.vPos, MousevDir);
		CObjMgr::Get_Instance()->Add_Object(OBJID::BATITEMBULLET, pBullet);

	}
	else if (dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[19].szName == L"BatItem")
	{
		UNIT_INFO& BatInfo = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Bat();
		UNIT_INFO& playerPos = m_pPlayer->Get_Pos();
		D3DXMATRIX matWorld, matTrans;
		UNIT_INFO Damage;
		if (playerPos.vSize.x >= 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x - 50.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };

		}
		else if (playerPos.vSize.x < 0)
		{
			Damage.vPos = {
				BatInfo.vPos.x + 100.f + playerPos.vPos.x,
				BatInfo.vPos.y - 45.f + playerPos.vPos.y,0.f };
		}
		Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		MousevDir = (Mouse->Get_Pos()) - playerPos.vPos;
		MousevDir.x += CScrollMgr::GetScrollPos(0);
		MousevDir.y += CScrollMgr::GetScrollPos(1);
		D3DXVec3Normalize(&MousevDir, &MousevDir);

		BatItemBullet* pBullet = BatItemBullet::Create(Damage.vPos, MousevDir);
		CObjMgr::Get_Instance()->Add_Object(OBJID::BATITEMBULLET, pBullet);

	}

}

void CMainHand::Attack()
{

	switch (m_eCurState)
	{
	case STATE::UNEQUIPED:
		UnEquiped();
		break;
	case STATE::EQUIPEDFIRST:
		EquipedFirst();
		break;
	case STATE::EQUIPEDSECOND:
		EquipedSecond();
		break;
	default:
		break;
	}
}

void CMainHand::HandSetting()
{
	if (First.szName != dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[12].szName)
	{// 
		First = dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[12]; // 12번째 인덱스는 첫번째 주무기이다.
		m_eNextState = STATE::EQUIPEDFIRST;
		Main = First;
	}
	if (Second.szName != dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[13].szName)
	{
		Second = dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[13]; // 13번째 인덱스는 두번째 주무기이다.
		m_eNextState = STATE::EQUIPEDSECOND;
		Main = Second;
	}
	if (Main.szName == L"")
	{
		m_eNextState = STATE::UNEQUIPED;
	}
}

HRESULT CMainHand::Initialize()
{
	m_tUnit.Texture.wstrObjectKey = L"Player";
	m_tUnit.Texture.wstrStateKey = L"Hand";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLook = { 1.f,0.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.vLength = { 3.f,3.f,0.f };
	m_tUnit.vPos.x = 6.f;
	m_tUnit.vPos.y = 7.f;
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);

	m_tUnit.vSize.x = m_tUnit.vLength.x / pTexInfo->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / pTexInfo->tImgInfo.Height;


	m_eCurState = UNEQUIPED;
	m_eNextState = UNEQUIPED;

	Main.iAtt = 1;
	Main.szName = L"";
	Main.Texture.wstrObjectKey = L"Player";
	Main.Texture.wstrStateKey = L"Hand";
	Main.Texture.iIndex = 0;

	Main.vSize = { 1.f,1.f,0.f };
	Main.vLength = { 3.f,3.f,0.f };
	Main.vPos.x = m_tUnit.vPos.x;
	Main.vPos.y = m_tUnit.vPos.y;

	const TEX_INFO* MainInfo = m_pTextureMgr->GetTexInfo(
		Main.Texture.wstrObjectKey, Main.Texture.wstrStateKey, Main.Texture.iIndex);

	Main.vSize.x = Main.vLength.x / MainInfo->tImgInfo.Width;
	Main.vSize.y = Main.vLength.y / MainInfo->tImgInfo.Height;

	HandSize.x = Main.vSize.x;
	HandSize.y = Main.vSize.y;
	AttackCount = 1;

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 1.f; // 1장
	return S_OK;
}

HRESULT CMainHand::LateInitialize()
{
	return S_OK;
}

CMainHand * CMainHand::Create()
{
	CMainHand* m_pInstance = new CMainHand;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
