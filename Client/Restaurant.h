#pragma once
#include "Field.h"
class CMyRestaurant;
class CRestaurant :
	public CField
{
private:
	explicit CRestaurant();
public:
	virtual ~CRestaurant();

public:
	// CField��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize();
	virtual void Release() override;

public:
	static CRestaurant* Create();

	// CField��(��) ���� ��ӵ�
	virtual void Set_Map() override;

	// CField��(��) ���� ��ӵ�
	virtual D3DXVECTOR3 & Get_Pos() override;

private:
	CMyRestaurant* pwI;
	UNIT_INFO ShopImage;
};
