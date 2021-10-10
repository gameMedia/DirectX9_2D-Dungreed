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
	static CStartButton* Create();

};