#pragma once
#include "Obj.h"
class CTrapObject :
	public CObj
{
public:
	CTrapObject();
	virtual ~CTrapObject();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;
};

