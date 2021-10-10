#pragma once
#include "UI.h"
class CPlayerHpBar :
	public CUI
{
private:
	explicit CPlayerHpBar();
public:
	virtual ~CPlayerHpBar();

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
	static CPlayerHpBar* Create();

private:
	INFO m_pPlayer;

	UNIT_INFO m_pPlayerLifeBack; //HpBar뒷배경
	UNIT_INFO m_LifeWave;		// 라이프 파도?
	UNIT_INFO m_pPlayerLifeBase; // HpBar테두리
	

};