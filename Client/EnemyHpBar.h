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
	static CEnemyHpBar* Create();

private:
	INFO m_pMonster;


};