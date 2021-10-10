#include "stdafx.h"
#include "MoveLeft.h"

CMoveLeft::CMoveLeft()
{
}


CMoveLeft::~CMoveLeft()
{
}

void CMoveLeft::Initialize()
{
}

void CMoveLeft::Execute(ICommandActor * _pPlayer)
{
	_pPlayer->ActStart(MOVE,LEFT);
}
