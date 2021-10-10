#pragma once
#include "ActCommand.h"
class CMoveLeft :
	public CActCommand
{
public:
	CMoveLeft();
	virtual ~CMoveLeft();

	// CActCommand을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Execute(ICommandActor * _pPlayer) override;
};

