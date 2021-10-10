#pragma once
#include "UI.h"
class CEnemyHpBar :
	public CUI
{
private:
	explicit CEnemyHpBar();
public:
	virtual ~CEnemyHpBar();

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
	static CEnemyHpBar* Create();

private:
	INFO m_pMonster;


};