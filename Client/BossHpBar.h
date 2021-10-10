#pragma once
#include "UI.h"
class CBossHpBar :
	public CUI
{
private:
	explicit CBossHpBar();
public:
	virtual ~CBossHpBar();

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
	static CBossHpBar* Create();

private:

	UNIT_INFO HpBarBase;
	UNIT_INFO BossIcon;
	INFO m_pMonster;


};