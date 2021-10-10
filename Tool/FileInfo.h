#pragma once
class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString ConvertRelativePath(const CString& strFullPath);

	// 현재 경로에서 ObjectKey, StateKey, 상대경로, 이미지 개수를 추출하는 함수.
	static void DirInfoExtraction(
		const wstring& wstrFullPath,
		list<PATH_INFO*>& rPathInfoLst);

	// 파일의 개수를 구하는 함수.
	static int DirInFileCount(const wstring& wstrFilePath);
};

