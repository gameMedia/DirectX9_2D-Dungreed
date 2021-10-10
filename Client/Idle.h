#pragma once
#include "ActCommand.h"
class CIdle :
	public CActCommand
{
public:
	CIdle();
	virtual ~CIdle();

	// CActCommand을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Execute(ICommandActor* _pPlayer) override;
};

