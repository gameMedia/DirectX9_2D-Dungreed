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
	// CUI을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	// 작업

public:
	static CBossHpBar* Create();

private:

	UNIT_INFO HpBarBase;
	UNIT_INFO BossIcon;
	INFO m_pMonster;


};