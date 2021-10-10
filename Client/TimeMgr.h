#pragma once
class CTimeMgr
{
	DEFINE_SINGLETONE(CTimeMgr)
private:
	explicit CTimeMgr();
	~CTimeMgr();
public:
	float GetDelta();

public:
	void UpdateTime();

private:
	LARGE_INTEGER	m_CurTime;
	LARGE_INTEGER	m_OldTime;
	LARGE_INTEGER	m_CpuTick;
	float			m_fDeltaTime;
};

