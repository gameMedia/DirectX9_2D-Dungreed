#pragma once
#include "Scene.h"
class CDungeon :
	public CScene
{
private:
	CDungeon();
public:
	virtual ~CDungeon();

public:
	// CScene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	void InitKey();

public:
	// �۾�
	void Set_Field(FIELDID::ID _field);
public:
	static CDungeon* Create();

private:
	ONOFFUIID::ID eID;
	FIELDTYPE::TYPE FieldType;
	FIELDID::ID FieldID;
	int Alpha;
};

