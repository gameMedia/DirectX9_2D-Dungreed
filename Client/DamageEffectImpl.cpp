#include "stdafx.h"
#include "DamageEffectImpl.h"


CDamageEffectImpl::CDamageEffectImpl(
	const wstring&  wstrObjectKey,
	const wstring&  wstrStateKey,
	int _iDamage)
	:CEffectImpl(wstrObjectKey),
	m_wstrStateKey(wstrStateKey),iDamage(_iDamage), Number(0)
{
	Set_SearchNumber();
}


CDamageEffectImpl::~CDamageEffectImpl()
{
}

int CDamageEffectImpl::Update()
{
	return 0;
}

void CDamageEffectImpl::Render(const D3DXMATRIX * pWorldMatrix)
{
	D3DXMATRIX matScale, matTrans, matWorld;
	int Size = (int)m_vecDamageNum.size() / 2;
	for (int i = 0; i < (int)m_vecDamageNum.size(); ++i)
	{
		const TEX_INFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
			m_wstrObjectKey, m_wstrStateKey, m_vecDamageNum[i]);
		NULL_CHECK(pTexInfo);


		D3DXMATRIX matScale, matTrans, matWorld, matParent;

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans,
			15.f * (Size - i), 0.f, 0.f);
		matParent = *pWorldMatrix;

		matWorld = matScale * matTrans * matParent;

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture,
			nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CDamageEffectImpl::Set_SearchNumber()
{
	int Number1,Damage1;
	Number1 = 0; // n승
	Damage1 = 0; // 데미지 계산

	while (true)
	{
		Damage1 = iDamage / pow(10, Number1);
		if (Damage1 <= 0)
		{
			Number = Number1;
			break;
		}
		++Number1;
	}

	m_vecDamageNum.reserve(Number);
	for (int i = 0; i < Number; ++i)
	{
		int CollNum = Coll_Num(i);
		m_vecDamageNum.emplace_back(CollNum);
	}
}

int CDamageEffectImpl::Coll_Num(int _number)
{
	int CollNum = iDamage;

	CollNum /= (int)pow(10, _number);

	int One_Number = CollNum % 10; // 1의 자리수
	return One_Number;
}

CDamageEffectImpl * CDamageEffectImpl::Create(
	const wstring&  wstrObjectKey,
	const wstring&  wstrStateKey,
	int _iDamage)
{
	return new CDamageEffectImpl(wstrObjectKey,wstrStateKey,_iDamage);
}
