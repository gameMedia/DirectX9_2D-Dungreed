#pragma once

class CPlayer;
class CBox 
{
private:
	CBox();
public:
	virtual ~CBox();

	// CObj을(를) 통해 상속됨
	 int Update() ;
	 void LateUpdate() ;
	 void Render() ;
	 HRESULT Initialize() ;
	 void Release() ;
	 void MakeWorldMatrix();
	 void UpdateRect();
public:
	static CBox* Create();
private:
	RECT PlayerRect;

	UNIT_INFO m_tUnit;
	RECT m_tRect;
	bool InitiOneTime;
};

