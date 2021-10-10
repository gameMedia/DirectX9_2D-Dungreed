#pragma once
#ifndef __FUNCTION_H__
#define __FUNCTION_H__


class CStrCmp
{
public:
	CStrCmp() {}
	CStrCmp(const TCHAR* pStr)
		:m_pString(pStr)
	{}
	~CStrCmp() {}

public:
	template<class T>
	bool operator()(T& rPair)
	{
		return !lstrcmp(m_pString, rPair.first);
	}
private:
	const TCHAR* m_pString;
};

template <typename T>
void SafeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void SafeDelete_Array(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}


#endif
