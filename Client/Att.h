#pragma once
#include "ActCommand.h"
class CAtt :
	public CActCommand
{
public:
	CAtt();
	virtual ~CAtt();

	// CActCommand을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Execute(ICommandActor * _pPlayer) override;
};

