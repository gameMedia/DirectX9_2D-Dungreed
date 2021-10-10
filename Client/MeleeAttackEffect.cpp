#include "stdafx.h"
#include "MeleeAttackEffect.h"
#include "EffectImpl.h"


CMeleeAttackEffect::CMeleeAttackEffect(
	CEffectImpl* pImp,
	D3DXMATRIX pParentMatrix,
	const ITEM_DATA& _Data, const float& _Angle)
	:CEffect(pImp),
	m_pParentMatrix(pParentMatrix),
	Angle(_Angle),
	m_fRevAngleZ(0.f), m_fRevSpd(0.f)
{
	m_tItemData = _Data;
}


CMeleeAttackEffect::~CMeleeAttackEffect()
{
}

int CMeleeAttackEffect::Update()
{
	CObj::LateInitialize();

	D3DXMATRIX matScroll;
	D3DXMatrixTranslation(&matScroll,
		-CScrollMgr::GetScrollPos(0),
		-CScrollMgr::GetScrollPos(1), 0.f);

	D3DXMatrixScaling(&m_tEffect.matScale, m_tEffect.vSize.x, m_tEffect.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tEffect.matTrans, 
		m_tEffect.vPos.x, 
		m_tEffect.vPos.y , 0.f);

	m_tEffect.matWorld = 
		m_tEffect.matScale *
		m_tEffect.matTrans *
		(m_pParentMatrix)  *
		matScroll;

	if (m_bIsDead)
	{
		return OBJ_DEAD;
	}
	
	

	return m_pImplementor->Update();
}

void CMeleeAttackEffect::LateUpdate()
{
	
}

void CMeleeAttackEffect::Render()
{

	UpdateRect();

	m_pImplementor->Render(&m_tEffect.matWorld);
	/*CDeviceMgr::Get_Instance()->GetSprite()->End();



	CDeviceMgr::Get_Instance()->GetLine()->SetWidth(5.f);
	CDeviceMgr::Get_Instance()->GetLine()->Draw(m_vOrigin, 5, D3DCOLOR_ARGB(255, 255, 0, 0));

	CDeviceMgr::Get_Instance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);*/

}

HRESULT CMeleeAttackEffect::Initialize()
{
	m_tEffect.vPos = { 0.f, 0.f, 0.f }; // 부모의 x, y로부터 100, 100만큼 떨어져라!
	m_tEffect.vSize = { 1.f, 1.f, 0.f };
	m_tEffect.vLook = { 1.f, 0.f, 0.f };

	m_fRevSpd = 10.f; // 초당 90도씩 
	m_fRevAngleZ = 10;
	return S_OK;
}

HRESULT CMeleeAttackEffect::LateInitialize()
{
	return E_NOTIMPL;
}

void CMeleeAttackEffect::Release()
{
}

void CMeleeAttackEffect::UpdateRect()
{
//	cout << "m_pParentMatirx_11  " << (m_pParentMatrix)._11 << endl; // 이값이 -이면 왼쪽 + 이면 오른쪽
	
	D3DXVECTOR3 vPos = { m_pParentMatrix._41,m_pParentMatrix._42,0.f };
	D3DXVECTOR3 vLength = { 140.f,160.f,0.f };

	m_tRect.left = LONG(vPos.x - (vLength.x * 0.5f));
	m_tRect.top = LONG(vPos.y - (vLength.y * 0.5f));
	m_tRect.right = LONG(vPos.x + (vLength.x * 0.5f));
	m_tRect.bottom = LONG(vPos.y + (vLength.y * 0.5f));

	/*m_vOrigin[0] = {
		(-vLength.x*0.5f) + vPos.x - CScrollMgr::GetScrollPos(0),
		(-vLength.y*0.5f) + vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[1] = {
		(vLength.x*0.5f) + vPos.x - CScrollMgr::GetScrollPos(0),
		(-vLength.y*0.5f) + vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[2] = {
		(vLength.x*0.5f) + vPos.x - CScrollMgr::GetScrollPos(0),
		(vLength.y*0.5f) + vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[3] = {
		(-vLength.x*0.5f) + vPos.x - CScrollMgr::GetScrollPos(0),
		(vLength.y*0.5f) + vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[4] = {
		(-vLength.x*0.5f) + vPos.x - CScrollMgr::GetScrollPos(0),
		(-vLength.y*0.5f) + vPos.y - CScrollMgr::GetScrollPos(1) };*/


}

void CMeleeAttackEffect::RectColl(CObj * pObj)
{
	m_bIsDead = true;
}

CMeleeAttackEffect * CMeleeAttackEffect::Create(
	CEffectImpl * pImp,
	D3DXMATRIX pParentMatrix,
	const ITEM_DATA& _Data,
	const float& _Angle)
{
	NULL_CHECK_RETURN(pImp, nullptr);


	CMeleeAttackEffect* pInstance = new CMeleeAttackEffect(
		pImp, pParentMatrix, _Data,_Angle);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
