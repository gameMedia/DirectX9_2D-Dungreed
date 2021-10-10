#pragma once
#include "ActCommand.h"

class CJump :
	public CActCommand
{
public:
	CJump();
	virtual ~CJump();


	// CActCommand을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Execute(ICommandActor* _pPlayer) override;

	


};

