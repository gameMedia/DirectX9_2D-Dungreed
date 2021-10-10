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
	// CField을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize();
	virtual void Release() override;

public:
	static CReinForce* Create();

	// CField을(를) 통해 상속됨
	virtual void Set_Map() override;

	// CField을(를) 통해 상속됨
	virtual D3DXVECTOR3 & Get_Pos() override;

private:
	CBox* pwI;
};

