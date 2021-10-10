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
	static CPlayerHungryGauge* Create();

private:
	INFO m_pPlayer;

	UNIT_INFO Icon;
	UNIT_INFO m_pPlayerLifeBack; //HpBar�޹��
	UNIT_INFO m_LifeWave;		// ������ �ĵ�?
	UNIT_INFO m_pPlayerLifeBase; // HpBar�׵θ�


};