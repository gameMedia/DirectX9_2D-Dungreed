#pragma once
#include "ActCommand.h"

class CJump :
	public CActCommand
{
public:
	CJump();
	virtual ~CJump();


	// CActCommand��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Execute(ICommandActor* _pPlayer) override;

	


};

