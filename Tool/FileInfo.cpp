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
	// B에서 C로 찾아갈 수 있는 상대경로를 만들어서 A에 저장.
	// 단, B와 C는 같은 드라이브 안에 존재해야한다.
	PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

// wstrFullPath: D:\김태윤\4개월차\95A 2반\Framework95A1\Texture\Stage
void CFileInfo::DirInfoExtraction(
	const wstring & wstrFullPath, 
	list<PATH_INFO*>& rPathInfoLst)
{
	// 파일 및 경로 탐색에 필요한 MFC 클래스.
	CFileFind	find;
	
	wstring wstrFindPath = wstrFullPath + L"\\*.*";

	// CFileFind::FindFile(파일명)
	// 해당 파일을 찾으면 TRUE, 못 찾으면 FALSE 반환.
	// 이 때 FindNextFile함수를 호출해주어야 찾은 파일에 접근할 수 있다.

	// CFileFind::FindFile(*.*)
	// 하위의 모든 것을 찾아라. 아무 것도 없다면 FALSE.
	// 이 때 FindNextFile함수를 호출해주어야 가장 첫번째에 접근할 수 있다.
	BOOL bContinue = find.FindFile(wstrFindPath.c_str());

	while (bContinue)
	{
		// FindNextFile함수가 FALSE 반환하면 더이상 찾을게 없다.
		bContinue = find.FindNextFile();

		if (find.IsDots()) // 찾은게 .과 ..같은 마커일 때 건너뜀.
			continue;
		// find.GetFilePath()
		// D:\김태윤\4개월차\95A 2반\Framework95A1\Texture\Stage\Effect
		else if (find.IsDirectory()) // 찾은 대상이 폴더라면
			DirInfoExtraction(wstring(find.GetFilePath().GetString()), rPathInfoLst);
		else // 찾은 대상이 파일이라면
		{
			if(find.IsSystem()) // 숨어있는 시스템 파일이라면 건너뜀.
				continue;

			PATH_INFO* pPathInfo = new PATH_INFO;

			// find.GetFilePath()
			/*
			D:\김태윤\4개월차\95A 2반\Framework95A1\Texture\
			Stage\Effect\BossMultiAttack\BossMultiAttack0.png
			*/

			TCHAR szFilePath[MAX_STR] = L"";
			lstrcpy(szFilePath, find.GetFilePath().GetString());
			PathRemoveFileSpec(szFilePath);

			// szFilePath
			/*
			D:\김태윤\4개월차\95A 2반\Framework95A1\Texture\
			Stage\Effect\BossMultiAttack
			*/
			pPathInfo->iImgCnt = DirInFileCount(szFilePath);

			///////////////////////////////////////////////////////////////////////////////


			// BossMultiAttack0
			wstring wstrFileTile = find.GetFileTitle().GetString();
			
			// wstring::substr(start, count): 현재 문자열에서 start를 시작으로 count만큼 반환.
			wstrFileTile = wstrFileTile.substr(0, wstrFileTile.length() - 1) + L"%d.png";


			/* szPathCombine
			D:\김태윤\4개월차\95A 2반\Framework95A1\Texture\
			Stage\Effect\BossMultiAttack\BossMultiAttack%d.png
			*/
			TCHAR szPathCombine[MAX_STR] = L"";
			PathCombine(szPathCombine, szFilePath, wstrFileTile.c_str());
			// PathCombine(A, B, C)
			// B와 C가 결합된 최종 경로를 A에 저장. 이 때 B와 C 사이에 \\가 자동으로 삽입.
			
			pPathInfo->wstrRelativePath = ConvertRelativePath(szPathCombine);

			///////////////////////////////////////////////////////////////////////////////

			// szFilePath
			/*
			D:\김태윤\4개월차\95A 2반\Framework95A1\Texture\
			Stage\Effect\BossMultiAttack
			*/

			// 경로 상의 파일이름을 얻어내는 함수. 경로 상에 파일이름이 없다면 말단 폴더명을 가져옴.
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
	D:\김태윤\4개월차\95A 2반\Framework95A1\Texture\
	Stage\Effect\BossMultiAttack
	*/

	CFileFind find;

	wstring wstrFileFind = wstrFilePath + L"\\*.*";

	BOOL bContinue = find.FindFile(wstrFileFind.c_str());

	int iCount = 0;

	while (bContinue)
	{
		// FindNextFile함수가 FALSE 반환하면 더이상 찾을게 없다.
		bContinue = find.FindNextFile();

		if(find.IsDots() || find.IsSystem())
			continue;

		iCount++;
	}

	return iCount;
}
