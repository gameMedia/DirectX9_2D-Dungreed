#pragma once
#include "ActCommand.h"
class CAtt :
	public CActCommand
{
public:
	CAtt();
	virtual ~CAtt();

	// CActCommand��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Execute(ICommandActor * _pPlayer) override;
};

