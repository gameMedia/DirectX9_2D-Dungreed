#pragma once
#include "ActCommand.h"
class CIdle :
	public CActCommand
{
public:
	CIdle();
	virtual ~CIdle();

	// CActCommand��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Execute(ICommandActor* _pPlayer) override;
};

