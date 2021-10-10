#pragma once
#include "UI.h"
class CInventoryIcon :
	public CUI
{
private:
	CInventoryIcon();
public:
	virtual ~CInventoryIcon();

public:
	// CUI을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	// 작업
	void MakeWorldMatrix();
public:
	static CInventoryIcon* Create();
};

