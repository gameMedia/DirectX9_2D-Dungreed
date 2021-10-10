#pragma once
#include "Obj.h"
class CTrapObject :
	public CObj
{
public:
	CTrapObject();
	virtual ~CTrapObject();

	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;
};

