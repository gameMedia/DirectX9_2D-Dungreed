#pragma once
#include "Field.h"

class CDungeon4 :
	public CField
{
private:
	explicit CDungeon4();
public:
	virtual ~CDungeon4();

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
	static CDungeon4* Create();

	// CField��(��) ���� ��ӵ�
	virtual void Set_Map() override;

	// CField��(��) ���� ��ӵ�
	virtual D3DXVECTOR3 & Get_Pos() override;

private:
	
};
