#pragma once
class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString ConvertRelativePath(const CString& strFullPath);

	// ���� ��ο��� ObjectKey, StateKey, �����, �̹��� ������ �����ϴ� �Լ�.
	static void DirInfoExtraction(
		const wstring& wstrFullPath,
		list<PATH_INFO*>& rPathInfoLst);

	// ������ ������ ���ϴ� �Լ�.
	static int DirInFileCount(const wstring& wstrFilePath);
};

