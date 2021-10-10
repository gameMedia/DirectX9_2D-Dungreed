#include "stdafx.h"
#include "LineMgr.h"
#include "Line.h"
#include "ToolView.h"

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

void CLineMgr::Render(CToolView* _pToolView)
{
	for (auto& pLine : m_listLine)
	{
		pLine->Render(_pToolView);
	}
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

