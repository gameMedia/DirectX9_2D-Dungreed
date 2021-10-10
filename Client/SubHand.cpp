#include "stdafx.h"
#include "SubHand.h"

#include "Inventory.h"
#include "Player.h"
#include "Mouse.h"

CSubHand::CSubHand()
	: InitOneTimeHand(false)
	, OnOffInven(false)
{
}


CSubHand::~CSubHand()
{
}

void CSubHand::Set_FirstSubItem(ITEM_DATA _FirstInven)
{
	First = _FirstInven;
	m_eNextState = STATE::EQUIPEDFIRST;
}

void CSubHand::Set_SecondSubItem(ITEM_DATA _SecondInven)
{
	Second = _SecondInven;

}

int CSubHand::Update()
{
	HandSetting();

	OnOffInven = dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_OnOffInventory();
	Attack();
	return 0;
}

void CSubHand::LateUpdate()
{
	StateCheck();
	MakeWorldMatrix();
}

void CSubHand::Render()
{
	const TEX_INFO* MainItem = m_pTextureMgr->GetTexInfo(
		Main.Texture.wstrObjectKey, Main.Texture.wstrStateKey, Main.Texture.iIndex);
	NULL_CHECK(MainItem);

	float fCenterX = MainItem->tImgInfo.Width*0.5f;
	float fCenterY = MainItem->tImgInfo.Height;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(Main.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		MainItem->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//if (Main.szName != L"")
	//{
	//	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
	//		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);
	//	NULL_CHECK(pTexInfo);

	//	float fCenterX = pTexInfo->tImgInfo.Width*0.5f;
	//	float fCenterY = pTexInfo->tImgInfo.Height*0.5f;

	//	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	//	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
	//		pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
	//		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//}
}

HRESULT CSubHand::Initialize()
{
	m_tUnit.Texture.wstrObjectKey = L"Player";
	m_tUnit.Texture.wstrStateKey = L"Hand";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLook = { 1.f,0.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.vLength = { 3.f,3.f,0.f };
	m_tUnit.vPos.x = -6.f;
	m_tUnit.vPos.y = 10.f;
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
	return S_OK;
}

HRESULT CSubHand::LateInitialize()
{
	return S_OK;
}

void CSubHand::Release()
{
}

void CSubHand::MakeWorldMatrix()
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

	if (AttackCount == -1)
		D3DXMatrixRotationZ(&m_tUnit.matRotZ, D3DXToRadian(m_RotAngle2 - 60.f)); // 자전
	else
		D3DXMatrixRotationZ(&m_tUnit.matRotZ, D3DXToRadian(m_RotAngle - 60.f)); // 자전

	D3DXMatrixTranslation(&Main.matTrans,
		Main.vPos.x,
		Main.vPos.y, 0.f);



	Main.matWorld = Main.matScale *m_tUnit.matRotZ * Main.matTrans * m_tUnit.matParent;
}

void CSubHand::StateCheck()
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
			Main.vPos.x = -6.f;
			Main.vPos.y = 10.f;

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
		else
		{
			Main.vPos = m_tUnit.vPos;
			Main.vSize = { 1.f,1.f,0.f };
		}

	}
}

void CSubHand::UnEquiped()
{
	if (Main.szName == L"")
	{
		Main.iBlock = 0;   // 블락률 초기화
		Main.iPlusAtt = 0; // 플러스 공격력 초기화
	}
}

void CSubHand::EquipedFirst()
{

}

void CSubHand::EquipedSecond()
{
}

void CSubHand::Attack()
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

void CSubHand::HandSetting()
{
	if (First.szName != dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[14].szName)
	{// 
		First = dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[14]; // 14번째 인덱스는 첫번째 보조무기이다.
		m_eNextState = STATE::EQUIPEDFIRST;
		Main = First;
	}
	if (Second.szName != dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[15].szName)
	{
		Second = dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_ItemData()[15]; // 15번째 인덱스는 두번째 보조무기이다.
		m_eNextState = STATE::EQUIPEDSECOND;
		Main = Second;
	}
	//if (Main.szName == L"")
	//{
	//	m_eNextState = STATE::UNEQUIPED;
	//}
}

CSubHand * CSubHand::Create()
{
	CSubHand* m_pInstance = new CSubHand;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
