#include "stdafx.h"
#include "PressDown.h"


CPressDown::CPressDown()
{
}


CPressDown::~CPressDown()
{
}

void CPressDown::Initialize()
{
}

void CPressDown::Execute(ICommandActor * _pPlayer)
{
	_pPlayer->ActStart(JUMP, DOWN);
}
