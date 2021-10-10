#pragma once
#include "Field.h"
class CBossStage :
	public CField
{
private:
	explicit CBossStage();
public:
	virtual ~CBossStage();

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
	static CBossStage* Create();

	// CField��(��) ���� ��ӵ�
	virtual void Set_Map() override;

	// CField��(��) ���� ��ӵ�
	virtual D3DXVECTOR3 & Get_Pos() override;
};

