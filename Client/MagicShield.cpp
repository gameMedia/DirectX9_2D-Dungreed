#include "stdafx.h"
#include "MagicShield.h"
#include "Inventory.h"

CMagicShield::CMagicShield()
	:m_bJump(true)
	, iJumpPower(6.f)
	, m_bJumpCheck(false)
	, JumpStop(false)
{
}


CMagicShield::~CMagicShield()
{
	Release();
}

int CMagicShield::Update()
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
	{ // ���� �ѰŰ��� or �÷��̾ �Ծ����� �״´�
		return OBJ_DEAD;
	}



	return 0;
}

void CMagicShield::LateUpdate()
{
}

void CMagicShield::Render()
{
	const TEX_INFO* MagicShield = nullptr;
	MagicShield = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);
	NULL_CHECK(MagicShield);

	float CenterX = MagicShield->tImgInfo.Width * 0.5f;
	float CenterY = MagicShield->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		MagicShield->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CMagicShield::Initialize()
{
	m_tUnit.vPos = { 650.f,300.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"MagicShield";
	m_tUnit.Texture.wstrStateKey = L"Idle";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLength = { 200.f,200.f,0.f };
	D3DXVECTOR3 vRenderLength = { 50.f,50.f,0.f };
	const TEX_INFO* NormalSword = nullptr;
	NormalSword = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);

	m_tUnit.vSize.x = vRenderLength.x / NormalSword->tImgInfo.Width;
	m_tUnit.vSize.y = vRenderLength.y / NormalSword->tImgInfo.Height;

	// item ����
	itemdata.iAtt = 0;
	itemdata.iGold = 1000;
	itemdata.iBlock = 5;
	itemdata.szName = L"MagicShield";
	itemdata.Texture.wstrObjectKey = L"MagicShield";
	itemdata.Texture.wstrStateKey = L"Idle";
	itemdata.Texture.iIndex = 0;
	itemdata.vLength = m_tUnit.vLength;
	itemdata.vSize = m_tUnit.vSize;
	itemdata.eType = ITEMTYPE::SUBWEAPON; // �ֹ���
	itemdata.eLength = ITEMINTER::END;
	return S_OK;
}

void CMagicShield::Release()
{
}

void CMagicShield::RectColl(CObj * pObj)
{
	INVENINFO* InvenSlotInfo = dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_InventorySlot();
	for (int i = 0; i < 12; ++i)
	{
		if (InvenSlotInfo[i].HoldingItem == false)
		{// ���� ������ â�� �������� ���ٸ�
		 // ������â ��Ȱ��ȭ
			dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Set_HoldingItem(true, i);
			// ������ ���� �Ѱ��ֱ�
			dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Set_InventorySlotItem(itemdata, i, true);

			m_bIsDead = true;// �������� �׿��ְ�
			break;
		}
	}
}

void CMagicShield::GravityLine()
{
}

void CMagicShield::GravityTile()
{
	if (m_bColl == true)
	{// ���� Ÿ�ϰ� �浹���̶��
		Current_tick = clock();

	}
}

CMagicShield * CMagicShield::Create()
{
	CMagicShield* m_pInstance = new CMagicShield;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
