#pragma once

class ICommandActor
{
public:
	virtual void ActStart(int _State,int _Dir=0) abstract;
};