#pragma once

#include "ICommandActor.h"

class ICommandActor;
class CActCommand
{
public:
	ENUM_STATE;
	ENUM_DIR;
public:
	virtual ~CActCommand() {};

public:
	virtual void Initialize() = 0;
	virtual void Execute(ICommandActor* _pPlayer) = 0;
};