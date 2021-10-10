#pragma once
class CFrameMgr
{
	DEFINE_SINGLETONE(CFrameMgr)
private:
	CFrameMgr();
	~CFrameMgr();
public:
	bool LimitFrame(float Limit);
private:
	LARGE_INTEGER m_CurTime;
	LARGE_INTEGER m_OldTime;
	LARGE_INTEGER m_CpuTick;
	float		  m_fTimeCount;
};

