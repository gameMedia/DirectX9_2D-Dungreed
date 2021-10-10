#include "stdafx.h"
#include "TimeMgr.h"

INFLEMENT_SIGLETONE(CTimeMgr)

CTimeMgr::CTimeMgr()
	:m_fDeltaTime(0.f)
{
	// 하드웨어에 있는 고해상도 타이머 누적 값을 얻어온다
	// 이 때 타이먼느 CPU의 Clock 수 기반
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceCounter(&m_OldTime);


	QueryPerformanceFrequency(&m_CpuTick);
}


CTimeMgr::~CTimeMgr()
{
}

float CTimeMgr::GetDelta()
{
	return m_fDeltaTime;
}

void CTimeMgr::UpdateTime()
{
	QueryPerformanceCounter(&m_CurTime);

	if (m_CurTime.QuadPart - m_OldTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceCounter(&m_CurTime);
		QueryPerformanceCounter(&m_OldTime);
		QueryPerformanceFrequency(&m_CpuTick);
	}

	m_fDeltaTime = float(m_CurTime.QuadPart - m_OldTime.QuadPart)/ m_CpuTick.QuadPart;

	m_OldTime.QuadPart = m_CurTime.QuadPart;
}
