#pragma once
#include "ActCommand.h"
class CDash :
	public CActCommand
{
public:
	CDash();
	virtual ~CDash();

	// CActCommand��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Execute(ICommandActor * _pPlayer) override;
};

