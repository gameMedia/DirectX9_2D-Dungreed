#pragma once

template<class T>
class CAbstractFactory
{
public: // 병신아..........................
	static T* Create(float x,float y)
	{
		T* pObj = T::Create();
		pObj->Set_Pos(x, y);
		return pObj;
	}
	static T* Create()
	{
		T* pObj = T::Create();
		return pObj;
	}
	static T* Create(D3DXVECTOR3 _Dir,ITEM_DATA _main)
	{
		T* pObj = T::Create(_Dir, _main);
		
		return pObj;
	}
	static T* CreateAIActor(float x, float y)
	{
		T* pObj = T::Create();
		pObj->Set_Pos(x, y);
		return pObj;
	}

	static CObj* Link_Attack(float x,float y,float fAngle) 
	{
		// angle넘겨줘서 파이어볼 이나 화살 발사 가능하게
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(x, y);
		pObj->Set_Angle(fAngle);
		return pObj;
	}
	static T* CreateAct()
	{
		T* pAct = new T;
		pAct->Initialize();
		return pAct;
	}

	static T* CreateUI()
	{
		T* pUi = T::Create();
		return pUi;
	}
};