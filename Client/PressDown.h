#pragma once
#include "ActCommand.h"
class CPressDown :
	public CActCommand
{
public:
	CPressDown();
	virtual ~CPressDown();

	// CActCommand��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Execute(ICommandActor * _pPlayer) override;
};

