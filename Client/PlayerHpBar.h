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
	static CPlayerHpBar* Create();

private:
	INFO m_pPlayer;

	UNIT_INFO m_pPlayerLifeBack; //HpBar�޹��
	UNIT_INFO m_LifeWave;		// ������ �ĵ�?
	UNIT_INFO m_pPlayerLifeBase; // HpBar�׵θ�
	

};