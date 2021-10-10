#pragma once
#include "UI.h"
class CSetButton :
	public CUI
{
private:
	explicit CSetButton();
public:
	virtual ~CSetButton();

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

public:
	static CSetButton* Create();
 
};

