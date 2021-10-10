#include "stdafx.h"
#include "MoveRight.h"

CMoveRight::CMoveRight()
{
}


CMoveRight::~CMoveRight()
{
}

void CMoveRight::Initialize()
{
}

void CMoveRight::Execute(ICommandActor * _pPlayer)
{

	_pPlayer->ActStart(MOVE,RIGHT);
}
