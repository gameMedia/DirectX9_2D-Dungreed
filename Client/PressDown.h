#pragma once
#include "ActCommand.h"
class CPressDown :
	public CActCommand
{
public:
	CPressDown();
	virtual ~CPressDown();

	// CActCommand을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Execute(ICommandActor * _pPlayer) override;
};

