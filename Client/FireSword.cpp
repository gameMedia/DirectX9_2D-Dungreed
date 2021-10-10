#include "stdafx.h"
#include "FireSword.h"
#include "Inventory.h"

CFireSword::CFireSword()
	:m_bJump(true)
	, iJumpPower(6.f)
	, m_bJumpCheck(false)
	, JumpStop(false)
{
}


CFireSword::~CFireSword()
{
	Release();
}

int CFireSword::Update()
{
	CObj::MakeWorldMatrix();
	CObj::UpdateRect();
	GravityTile();
	float fY = 0.f;
	bool bIsColl = CLineMgr::Get_Instance()->LineCollision((&m_tUnit.vPos), &fY);


	if (m_bJumpCheck == false)
	{
		Current_tick = clock();
		m_bJumpCheck = true;
		JumpStop = false;
	}
	if (m_bJumpCheck == true && m_bJump == true)
	{
		Next_tick = ((clock() - Current_tick) + 100) / 100;
		JumpTotal = (iJumpPower*Next_tick) - (GRAVITY*Next_tick*Next_tick*0.5f);
		m_tUnit.vPos.y -= JumpTotal;

		if (bIsColl && m_tUnit.vPos.y > fY)
		{
			m_bJump = false;
			m_tUnit.vPos.y = fY;

		}
		else if (JumpStop == true)
		{
			m_bJump = false;
		}
	}

	if (m_bIsDead == true)
	{ // 씬이 넘거갈때 or 플레이어가 먹었을시 죽는다
		return OBJ_DEAD;
	}



	return 0;
}

void CFireSword::LateUpdate()
{
}

void CFireSword::Render()
{
	const TEX_INFO* FireSword = nullptr;
	FireSword = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);
	NULL_CHECK(FireSword);

	float CenterX = FireSword->tImgInfo.Width * 0.5f;
	float CenterY = FireSword->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		FireSword->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CFireSword::Initialize()
{
	m_tUnit.vPos = { 650.f,300.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"FireSword";
	m_tUnit.Texture.wstrStateKey = L"FireDragonKillerIdle";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLength = { 80.f,200.f,0.f };
	D3DXVECTOR3 vRenderLength = { 100.f,100.f,0.f };
	const TEX_INFO* NormalSword = nullptr;
	NormalSword = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);

	m_tUnit.vSize.x = vRenderLength.x / NormalSword->tImgInfo.Width;
	m_tUnit.vSize.y = vRenderLength.y / NormalSword->tImgInfo.Height;

	// item 정보
	itemdata.iAtt = 35;
	itemdata.iGold = 1000;
	itemdata.szName = L"FireSword";
	itemdata.Texture.wstrObjectKey = L"FireSword";
	itemdata.Texture.wstrStateKey = L"FireDragonKillerIdle";
	itemdata.Texture.iIndex = 0;
	itemdata.vLength = m_tUnit.vLength;
	itemdata.vSize = m_tUnit.vSize;
	itemdata.eType = ITEMTYPE::ATTACKWEAPON; // 주무기
	itemdata.eLength = ITEMINTER::MELEE; // 근접무기
	return S_OK;
}

void CFireSword::Release()
{
}

void CFireSword::RectColl(CObj* pObj)
{
	INVENINFO* InvenSlotInfo = dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_InventorySlot();
	for (int i = 0; i < 12; ++i)
	{
		if (InvenSlotInfo[i].HoldingItem == false)
		{// 만약 아이템 창에 아이템이 없다면
		 // 아이템창 비활성화
			dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Set_HoldingItem(true, i);
			// 아이템 정보 넘겨주기
			dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Set_InventorySlotItem(itemdata, i, true);

			m_bIsDead = true;// 아이템을 죽여주고
			break;
		}
	}
}

void CFireSword::GravityLine()
{
}

void CFireSword::GravityTile()
{
	if (m_bColl == true)
	{// 만약 타일과 충돌중이라면
		Current_tick = clock();

	}
}

CFireSword * CFireSword::Create()
{
	CFireSword* m_pInstance = new CFireSword;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
