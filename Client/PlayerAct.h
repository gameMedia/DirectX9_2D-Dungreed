#pragma once
#include "stdafx.h"
#include "Player.h"

class CPlayer;
class CPlayerAct
{
public:
	ENUM_STATE;
public:
	virtual ~CPlayerAct() {};

public:
	void Set_Player(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }
public:
	virtual void Initialize() = 0;
	virtual void Execute(clock_t& _currentTick) = 0;
	virtual void Offset() =0;
protected:
	CPlayer* m_pPlayer;
	clock_t m_CurrentTick;
};