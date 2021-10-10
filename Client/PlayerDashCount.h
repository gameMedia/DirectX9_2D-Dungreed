#pragma once
#include "UI.h"
class CPlayerDashCount :
	public CUI
{
private:
	explicit CPlayerDashCount();
public:
	virtual ~CPlayerDashCount();

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
	static CPlayerDashCount* Create();

private:
	INFO m_pPlayer;


	UNIT_INFO DashCountRender[12];
	UNIT_INFO m_pPlayerLifeBack; //HpBar�޹��
	UNIT_INFO m_LifeWave;		// ������ �ĵ�?
	UNIT_INFO m_pPlayerLifeBase; // HpBar�׵θ�


};