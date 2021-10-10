#include "stdafx.h"
#include "LineMgr.h"
#include "Line.h"

INFLEMENT_SIGLETONE(CLineMgr)

CLineMgr::CLineMgr()
	:DrawLineCheck(false)
{
}


CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize()
{

	PlayerPastPos = { 0.f,0.f,0.f };
}

void CLineMgr::SetLPoint(const D3DXVECTOR3 & vPos,
	int& _Alpha,
	int& _Red,
	int& _Green,
	int& _Blue,
	int& _iOption)
{
	if (DrawLineCheck == false)
		return;

	LINEINFO tStartIfno = {
	LINEPOS(float(vPos.x),float(vPos.y)),
	LINEPOS(float(vPos.x),float(vPos.y)),
	_Alpha,_Red,_Green,_Blue,_iOption };

	m_listLine.emplace_back(new CLine(tStartIfno));


}

void CLineMgr::SetRPoint(const D3DXVECTOR3 & vPos,
	int & _Alpha,
	int & _Red,
	int & _Green,
	int & _Blue,
	int & _iOption)
{
	if (DrawLineCheck == false)
		return;

	m_listLine.back()->Set_RPoint(LINEPOS(float(vPos.x), float(vPos.y)));


}

void CLineMgr::LateUpdate()
{
}

void CLineMgr::Render()
{
	//for (auto& pLine : m_listLine)
	//{
	//	pLine->Render();
	//}
}

void CLineMgr::Release()
{
	for (auto& pLine : m_listLine)
	{
		if (pLine)
		{
			SafeDelete(pLine);
		}
	}
	m_listLine.clear();
}

bool CLineMgr::LineCollision(D3DXVECTOR3 * vPos, float * pOutY)
{
	list<CLine*> m_listLine11 = m_listLine;
	if (m_listLine11.empty())
		return false; // 라인이 없으면 체크하지마라
	CLine* pTarget = nullptr;

	for (auto pLine : m_listLine11)
	{
		float LPointX = pLine->Get_Line()->LPoint.Linepos.x;
		float RPointX = pLine->Get_Line()->RPoint.Linepos.x;

		float LPointY = pLine->Get_Line()->LPoint.Linepos.y;
		float RPointY = pLine->Get_Line()->RPoint.Linepos.y;
		D3DXVECTOR3 vDir = {
			RPointX - LPointX
			,RPointY - LPointY
			,0.f }; // 선을 벡터로

		D3DXVECTOR3 vLNormal = {};
		D3DXVec3Cross(&vLNormal, &vDir, &D3DXVECTOR3(0.f, 0.f, 1.f));
		D3DXVECTOR3 vLObjDir = {
			(*vPos).x - LPointX
			,(*vPos).y - LPointY
			,0.f };
		D3DXVECTOR3 vRObjDir = {
			(*vPos).x - RPointX,
			(*vPos).y - RPointY,
			0.f
		};
		D3DXVec3Normalize(&vLNormal, &vLNormal);
		D3DXVec3Normalize(&vLObjDir, &vLObjDir);
		float Scalar = D3DXVec3Dot(&vLObjDir, &vLNormal);
		if (-0.05f < Scalar)
		{// 예각 일 때 선분 위에 있다
			if (
				((*vPos).x >= LPointX
				&& (*vPos).x <= RPointX))
			{
				// b = LPointY-aLPointX == y절편
				// 기울기 a
				float a = (RPointY - LPointY) / (RPointX - LPointX);
				// y절편 b
				float b = LPointY - a*LPointX;
				//y = ax + b;
				//0 = ax - y + b;
				float Dist = fabs((a*(*vPos).x) + (-1 * (*vPos).y) + b)
					/ sqrtf((a*a) + (-1 * -1));
				if (Dist < 50.f)
				{
					pTarget = pLine;
					break;
				}
			}
		}

	}

	if (nullptr == pTarget)
		return false;

	float x1 = pTarget->Get_Line()->LPoint.Linepos.x;
	float x2 = pTarget->Get_Line()->RPoint.Linepos.x;
	float y1 = pTarget->Get_Line()->LPoint.Linepos.y;
	float y2 = pTarget->Get_Line()->RPoint.Linepos.y;



	//if (pTarget->Get_Line()->iOption == 1)
	//{// 그냥 통과 불가능한 선

	//}
	//if (pTarget->Get_Line()->iOption == 2)
	//{// 점프로 통과 가능 선
	*pOutY = (y2 - y1) / (x2 - x1) * ((*vPos).x - x1) + y1;
	//	}

	return true;
}

bool CLineMgr::LineCollisionMove(D3DXVECTOR3 * vPos, float * pOutY)
{
	list<CLine*> m_listLine11 = m_listLine;
	if (m_listLine11.empty())
		return false;

	CLine* pTarget = nullptr;

	for (auto& pLine : m_listLine11)
	{
		float LPointX = pLine->Get_Line()->LPoint.Linepos.x;
		float RPointX = pLine->Get_Line()->RPoint.Linepos.x;

		float LPointY = pLine->Get_Line()->LPoint.Linepos.y;
		float RPointY = pLine->Get_Line()->RPoint.Linepos.y;
		if ((*vPos).x >= LPointX && (*vPos).x <= RPointX)
		{
			// b = LPointY-aLPointX == y절편
			// 기울기 a
			float a = (RPointY - LPointY) / (RPointX - LPointX);
			// y절편 b
			float b = LPointY - a*LPointX;
			//y = ax + b;
			//0 = ax - y + b;
			float Dist = fabs((a*(*vPos).x) + (-1 * (*vPos).y) + b)
				/ sqrtf((a*a) + (-1 * -1));
			if (Dist < 10.f)
			{
				pTarget = pLine;
				break;
			}
		}
	}

	if (nullptr == pTarget)
		return false;

	float x1 = pTarget->Get_Line()->LPoint.Linepos.x;
	float x2 = pTarget->Get_Line()->RPoint.Linepos.x;
	float y1 = pTarget->Get_Line()->LPoint.Linepos.y;
	float y2 = pTarget->Get_Line()->RPoint.Linepos.y;

	*pOutY = (y2 - y1) / (x2 - x1) * ((*vPos).x - x1) + y1;

	return true;
}

bool CLineMgr::LineCollisionDash(D3DXVECTOR3 * vPos, float * pOutY)
{
	list<CLine*> m_listLine11 = m_listLine;
	if (m_listLine11.empty())
		return false; // 라인이 없으면 체크하지마라
	CLine* pTarget = nullptr;

	for (auto pLine : m_listLine11)
	{
		float LPointX = pLine->Get_Line()->LPoint.Linepos.x;
		float RPointX = pLine->Get_Line()->RPoint.Linepos.x;

		float LPointY = pLine->Get_Line()->LPoint.Linepos.y;
		float RPointY = pLine->Get_Line()->RPoint.Linepos.y;
		D3DXVECTOR3 vDir = {
			RPointX - LPointX
			,RPointY - LPointY
			,0.f }; // 선을 벡터로

		D3DXVECTOR3 vLNormal = {};
		D3DXVec3Cross(&vLNormal, &vDir, &D3DXVECTOR3(0.f, 0.f, 1.f));
		D3DXVECTOR3 vLObjDir = {
			(*vPos).x - LPointX
			,(*vPos).y - LPointY
			,0.f };
		D3DXVECTOR3 vRObjDir = {
			(*vPos).x - RPointX,
			(*vPos).y - RPointY,
			0.f
		};
		D3DXVec3Normalize(&vLNormal, &vLNormal);
		D3DXVec3Normalize(&vLObjDir, &vLObjDir);
		float Scalar = D3DXVec3Dot(&vLObjDir, &vLNormal);
		if (0.f > Scalar)
		{// 둔각 일 때 선분 아래에 있다
			if (
				((*vPos).x >= LPointX
					&& (*vPos).x <= RPointX))
			{
				// b = LPointY-aLPointX == y절편
				// 기울기 a
				float a = (RPointY - LPointY) / (RPointX - LPointX);
				// y절편 b
				float b = LPointY - a*LPointX;
				//y = ax + b;
				//0 = ax - y + b;
				float Dist = fabs((a*(*vPos).x) + (-1 * (*vPos).y) + b)
					/ sqrtf((a*a) + (-1 * -1));
				if (Dist < 50.f)
				{
					pTarget = pLine;
					break;
				}
			}
		}

	}

	if (nullptr == pTarget)
		return false;

	float x1 = pTarget->Get_Line()->LPoint.Linepos.x;
	float x2 = pTarget->Get_Line()->RPoint.Linepos.x;
	float y1 = pTarget->Get_Line()->LPoint.Linepos.y;
	float y2 = pTarget->Get_Line()->RPoint.Linepos.y;




	//}
	//if (pTarget->Get_Line()->iOption == 2)
	//{// 점프로 통과 가능 선
	if (pTarget->Get_Line()->iOption == 1)
	{// 그냥 통과 불가능한 선
		*pOutY = (y2 - y1) / (x2 - x1) * ((*vPos).x - x1) + y1;
		return true;
	}
	return false;
	//	}

}

HRESULT CLineMgr::SaveData(const TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, nullptr);

	// 실패시 INVALID_HANDLE_VALUE 를 반환 
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwBytes = 0;
	size_t Line_Size = 0;
	for (size_t j = 0; j < m_listLine.size(); ++j)
	{
		++Line_Size;
	}
	WriteFile(hFile, &Line_Size, sizeof(size_t), &dwBytes, nullptr);
	for (auto& pLine : m_listLine)
	{
		WriteFile(hFile, pLine->Get_Line(), sizeof(LINEINFO), &dwBytes, nullptr);
		/*
		1. 내가 개방한 핸들
		2. 내가 넣고자하는 데이터의 시작 주소
		3. 내가 넣고자하는 데이터의 크기
		4. 실제로 기록한 바이트 수를 리턴 받기 위한 출력용 인자.
		5. 비동기 입출력을 위한 구조체 포인터 변수.  우리가 사용할게 아님. null


		*/
	}
	CloseHandle(hFile);

	return S_OK;

}

HRESULT CLineMgr::LoadData(const TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	if (!m_listLine.empty())
		Release();

	size_t size_Line = 0;
	DWORD dwBytes = 0;
	LINEINFO pLine = {};

	ReadFile(hFile, &size_Line, sizeof(size_t), &dwBytes, nullptr);

	for (size_t j = 0; j < size_Line; ++j)
	{
		ReadFile(hFile, &pLine, sizeof(LINEINFO), &dwBytes, nullptr);

		if ((j - 1) == size_Line)
		{

			break;
		}

		m_listLine.push_back(new CLine(pLine));
	}

	CloseHandle(hFile);

	return S_OK;
}

