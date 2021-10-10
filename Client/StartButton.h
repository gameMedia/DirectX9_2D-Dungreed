#pragma once
#include "UI.h"
class CStartButton :
	public CUI
{
private:
	explicit CStartButton();
public:
	virtual ~CStartButton();

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
	static CStartButton* Create();

};