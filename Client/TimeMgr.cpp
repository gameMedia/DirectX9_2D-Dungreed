#include "stdafx.h"
#include "TimeMgr.h"

INFLEMENT_SIGLETONE(CTimeMgr)

CTimeMgr::CTimeMgr()
	:m_fDeltaTime(0.f)
{
	// �ϵ��� �ִ� ���ػ� Ÿ�̸� ���� ���� ���´�
	// �� �� Ÿ�̸մ� CPU�� Clock �� ���
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