#pragma once
#include "ActCommand.h"
class CMoveLeft :
	public CActCommand
{
public:
	CMoveLeft();
	virtual ~CMoveLeft();

	// CActCommand��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Execute(ICommandActor * _pPlayer) override;
};

