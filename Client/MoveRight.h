#pragma once
#include "ActCommand.h"
class CMoveRight :
	public CActCommand
{
public:
	CMoveRight();
	virtual ~CMoveRight();

	// CActCommand을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Execute(ICommandActor * _pPlayer) override;
};

