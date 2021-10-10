#include "stdafx.h"
#include "Jump.h"



CJump::CJump()
{
}


CJump::~CJump()
{
}

void CJump::Initialize()
{

}
void CJump::Execute(ICommandActor * _pPlayer)
{
	_pPlayer->ActStart(JUMP,UP);

	
}



