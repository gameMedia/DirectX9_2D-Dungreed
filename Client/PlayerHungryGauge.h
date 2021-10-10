#pragma once
#include "UI.h"
class CPlayerHungryGauge :
	public CUI
{
private:
	explicit CPlayerHungryGauge();
public:
	virtual ~CPlayerHungryGauge();

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
	static CPlayerHungryGauge* Create();

private:
	INFO m_pPlayer;

	UNIT_INFO Icon;
	UNIT_INFO m_pPlayerLifeBack; //HpBar뒷배경
	UNIT_INFO m_LifeWave;		// 라이프 파도?
	UNIT_INFO m_pPlayerLifeBase; // HpBar테두리


};