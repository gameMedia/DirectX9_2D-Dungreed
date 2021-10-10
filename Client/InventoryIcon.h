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
	// CUI��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	// �۾�
	void MakeWorldMatrix();
public:
	static CInventoryIcon* Create();
};

