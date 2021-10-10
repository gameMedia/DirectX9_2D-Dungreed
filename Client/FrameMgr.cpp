#include "stdafx.h"
#include "FrameMgr.h"

INFLEMENT_SIGLETONE(CFrameMgr)

CFrameMgr::CFrameMgr()
	:m_fTimeCount(0.f)
{
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceCounter(&m_OldTime);

	QueryPerformanceFrequency(&m_CpuTick);
}


CFrameMgr::~CFrameMgr()
{
}

bool CFrameMgr::LimitFrame(float Limit)
{
	QueryPerformanceCounter(&m_CurTime);

	if (m_CurTime.QuadPart - m_OldTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceCounter(&m_CurTime);
		QueryPerformanceCounter(&m_OldTime);
		QueryPerformanceFrequency(&m_CpuTick);
	}
	m_fTimeCount += float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CpuTick.QuadPart;
	m_OldTime.QuadPart = m_CurTime.QuadPart;
	if ( (1.f / Limit) <= m_fTimeCount)
	{
		m_fTimeCount = 0.f;
		return true;
	}
	return false;
}
