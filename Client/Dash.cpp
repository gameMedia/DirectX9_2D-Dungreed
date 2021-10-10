#include "stdafx.h"
#include "Dash.h"


CDash::CDash()
{
}


CDash::~CDash()
{
}

void CDash::Initialize()
{
}

void CDash::Execute(ICommandActor * _pPlayer)
{
	_pPlayer->ActStart(DASH);
}
