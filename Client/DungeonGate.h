#pragma once
#include "Obj.h"
class CDungeonGate :
	public CObj
{
private:
	explicit CDungeonGate();
public:
	virtual ~CDungeonGate();

public:
	// CObj��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	void RectColl(CObj* pObj=nullptr);
public:
	static CDungeonGate* Create();

};

