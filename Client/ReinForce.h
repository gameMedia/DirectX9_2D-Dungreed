#pragma once
#include "Field.h"
class CBox;
class CReinForce :
	public CField
{
private:
	explicit CReinForce();
public:
	virtual ~CReinForce();

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
	static CReinForce* Create();

	// CField��(��) ���� ��ӵ�
	virtual void Set_Map() override;

	// CField��(��) ���� ��ӵ�
	virtual D3DXVECTOR3 & Get_Pos() override;

private:
	CBox* pwI;
};

