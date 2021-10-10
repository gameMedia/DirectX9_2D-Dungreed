#pragma once
#include "ActCommand.h"
class CDash :
	public CActCommand
{
public:
	CDash();
	virtual ~CDash();

	// CActCommand을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Execute(ICommandActor * _pPlayer) override;
};

