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
		return false; // ������ ������ üũ��������
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
			,0.f }; // ���� ���ͷ�

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
		{// ���� �� �� ���� ���� �ִ�
			if (
				((*vPos).x >= LPointX
				&& (*vPos).x <= RPointX))
			{
				// b = LPointY-aLPointX == y����
				// ���� a
				float a = (RPointY - LPointY) / (RPointX - LPointX);
				// y���� b
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
	//{// �׳� ��� �Ұ����� ��

	//}
	//if (pTarget->Get_Line()->iOption == 2)
	//{// ������ ��� ���� ��
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
			// b = LPointY-aLPointX == y����
			// ���� a
			float a = (RPointY - LPointY) / (RPointX - LPointX);
			// y���� b
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
		return false; // ������ ������ üũ��������
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
			,0.f }; // ���� ���ͷ�

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
		{// �а� �� �� ���� �Ʒ��� �ִ�
			if (
				((*vPos).x >= LPointX
					&& (*vPos).x <= RPointX))
			{
				// b = LPointY-aLPointX == y����
				// ���� a
				float a = (RPointY - LPointY) / (RPointX - LPointX);
				// y���� b
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
	//{// ������ ��� ���� ��
	if (pTarget->Get_Line()->iOption == 1)
	{// �׳� ��� �Ұ����� ��
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

	// ���н� INVALID_HANDLE_VALUE �� ��ȯ 
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
		1. ���� ������ �ڵ�
		2. ���� �ְ����ϴ� �������� ���� �ּ�
		3. ���� �ְ����ϴ� �������� ũ��
		4. ������ ����� ����Ʈ ���� ���� �ޱ� ���� ��¿� ����.
		5. �񵿱� ������� ���� ����ü ������ ����.  �츮�� ����Ұ� �ƴ�. null


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

