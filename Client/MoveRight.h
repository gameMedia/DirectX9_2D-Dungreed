#pragma once
#include "ActCommand.h"
class CMoveRight :
	public CActCommand
{
public:
	CMoveRight();
	virtual ~CMoveRight();

	// CActCommand��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Execute(ICommandActor * _pPlayer) override;
};

