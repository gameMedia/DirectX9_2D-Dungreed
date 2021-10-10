#pragma once
#include "UI.h"
class CExitButton :
	public CUI
{
private:
	explicit CExitButton();
public:
	virtual ~CExitButton();

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
	static CExitButton* Create();
};

