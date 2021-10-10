#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(const CString & strFullPath)
{
	TCHAR szCurrentPath[MAX_STR] = L"";
	TCHAR szRelativePath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szCurrentPath);

	// PathRelativePathTo(A, B, FILE_ATTRIBUTE_DIRECTORY, C, FILE_ATTRIBUTE_DIRECTORY)
	// B���� C�� ã�ư� �� �ִ� ����θ� ���� A�� ����.
	// ��, B�� C�� ���� ����̺� �ȿ� �����ؾ��Ѵ�.
	PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

// wstrFullPath: D:\������\4������\95A 2��\Framework95A1\Texture\Stage
void CFileInfo::DirInfoExtraction(
	const wstring & wstrFullPath, 
	list<PATH_INFO*>& rPathInfoLst)
{
	// ���� �� ��� Ž���� �ʿ��� MFC Ŭ����.
	CFileFind	find;
	
	wstring wstrFindPath = wstrFullPath + L"\\*.*";

	// CFileFind::FindFile(���ϸ�)
	// �ش� ������ ã���� TRUE, �� ã���� FALSE ��ȯ.
	// �� �� FindNextFile�Լ��� ȣ�����־�� ã�� ���Ͽ� ������ �� �ִ�.

	// CFileFind::FindFile(*.*)
	// ������ ��� ���� ã�ƶ�. �ƹ� �͵� ���ٸ� FALSE.
	// �� �� FindNextFile�Լ��� ȣ�����־�� ���� ù��°�� ������ �� �ִ�.
	BOOL bContinue = find.FindFile(wstrFindPath.c_str());

	while (bContinue)
	{
		// FindNextFile�Լ��� FALSE ��ȯ�ϸ� ���̻� ã���� ����.
		bContinue = find.FindNextFile();

		if (find.IsDots()) // ã���� .�� ..���� ��Ŀ�� �� �ǳʶ�.
			continue;
		// find.GetFilePath()
		// D:\������\4������\95A 2��\Framework95A1\Texture\Stage\Effect
		else if (find.IsDirectory()) // ã�� ����� �������
			DirInfoExtraction(wstring(find.GetFilePath().GetString()), rPathInfoLst);
		else // ã�� ����� �����̶��
		{
			if(find.IsSystem()) // �����ִ� �ý��� �����̶�� �ǳʶ�.
				continue;

			PATH_INFO* pPathInfo = new PATH_INFO;

			// find.GetFilePath()
			/*
			D:\������\4������\95A 2��\Framework95A1\Texture\
			Stage\Effect\BossMultiAttack\BossMultiAttack0.png
			*/

			TCHAR szFilePath[MAX_STR] = L"";
			lstrcpy(szFilePath, find.GetFilePath().GetString());
			PathRemoveFileSpec(szFilePath);

			// szFilePath
			/*
			D:\������\4������\95A 2��\Framework95A1\Texture\
			Stage\Effect\BossMultiAttack
			*/
			pPathInfo->iImgCnt = DirInFileCount(szFilePath);

			///////////////////////////////////////////////////////////////////////////////


			// BossMultiAttack0
			wstring wstrFileTile = find.GetFileTitle().GetString();
			
			// wstring::substr(start, count): ���� ���ڿ����� start�� �������� count��ŭ ��ȯ.
			wstrFileTile = wstrFileTile.substr(0, wstrFileTile.length() - 1) + L"%d.png";


			/* szPathCombine
			D:\������\4������\95A 2��\Framework95A1\Texture\
			Stage\Effect\BossMultiAttack\BossMultiAttack%d.png
			*/
			TCHAR szPathCombine[MAX_STR] = L"";
			PathCombine(szPathCombine, szFilePath, wstrFileTile.c_str());
			// PathCombine(A, B, C)
			// B�� C�� ���յ� ���� ��θ� A�� ����. �� �� B�� C ���̿� \\�� �ڵ����� ����.
			
			pPathInfo->wstrRelativePath = ConvertRelativePath(szPathCombine);

			///////////////////////////////////////////////////////////////////////////////

			// szFilePath
			/*
			D:\������\4������\95A 2��\Framework95A1\Texture\
			Stage\Effect\BossMultiAttack
			*/

			// ��� ���� �����̸��� ���� �Լ�. ��� �� �����̸��� ���ٸ� ���� �������� ������.
			pPathInfo->wstrStateKey = PathFindFileName(szFilePath);

			///////////////////////////////////////////////////////////////////////////////

			PathRemoveFileSpec(szFilePath);
			pPathInfo->wstrObjectKey = PathFindFileName(szFilePath);

			rPathInfoLst.push_back(pPathInfo);
			break;
		}
	}
}

int CFileInfo::DirInFileCount(const wstring & wstrFilePath)
{
	// wstrFilePath
	/*
	D:\������\4������\95A 2��\Framework95A1\Texture\
	Stage\Effect\BossMultiAttack
	*/

	CFileFind find;

	wstring wstrFileFind = wstrFilePath + L"\\*.*";

	BOOL bContinue = find.FindFile(wstrFileFind.c_str());

	int iCount = 0;

	while (bContinue)
	{
		// FindNextFile�Լ��� FALSE ��ȯ�ϸ� ���̻� ã���� ����.
		bContinue = find.FindNextFile();

		if(find.IsDots() || find.IsSystem())
			continue;

		iCount++;
	}

	return iCount;
}
